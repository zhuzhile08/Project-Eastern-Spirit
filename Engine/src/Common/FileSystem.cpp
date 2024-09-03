#include <Common/FileSystem.h>

#include <LSD/Array.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

namespace esengine {

namespace globals {

FileSystem* fileSystem;

}

namespace {

void flushAndCloseFile(std::FILE* file) {
	std::fflush(file);
	std::fclose(file);
}

constexpr const char* enumToOpenMode(OpenMode m) {
	constexpr lsd::Array<const char*, 15> openModes {
		"rt",
		"wt",
		"at",
		"",
		"r+t",
		"w+t",
		"a+t",
		"",
		"rb",
		"wb",
		"ab",
		"",
		"r+b",
		"w+b",
		"a+b"
	};

	return openModes[static_cast<std::size_t>(m)];
}

}


FileSystem::FileSystem(char** argv) : m_absPathBase(argv[0]) {
	m_absPathBase.remove_filename();
}

bool FileSystem::exists(const std::filesystem::path& path) const {
	return m_files.contains(path);
}

bool FileSystem::contains(const std::filesystem::path& path, OpenMode mode) const {
	string_type p(path.native());

	auto mStr = enumToOpenMode(mode);

#ifdef _WIN32
	wchar_t m[4];
	std::mbstowcs(m, mStr, 4);
	p.append(m);
#else
	p.append(mStr);
#endif

	return m_files.contains(path);
}

std::filesystem::path FileSystem::absolutePath(const std::filesystem::path& path) const { 
	return m_absPathBase/path; 
}

std::filesystem::path FileSystem::localPath(const std::filesystem::path& path) const {
	return std::filesystem::relative(path, m_absPathBase);
}

std::filesystem::path FileSystem::assetsPath() const {
	return absolutePath("Data/");
}

File FileSystem::load(const std::filesystem::path& path, OpenMode mode, bool buffered) {
	// @todo implement as multithreaded
	// return signal (bool mutex?)
	// run on different thread and set bool to true when done

	string_type p(path.native());

	auto mStr = enumToOpenMode(mode);

#ifdef _WIN32
	wchar_t m[4];
	std::mbstowcs(m, mStr, mStr);
	p.append(m);
#else
	p.append(mStr);
#endif

	auto it = m_files.find(p);

	if (it != m_files.end()) {
		return File(it->second.get(), (buffered ? globals::fileSystem->unusedBuffer() : nullptr), path.c_str());
	}

	while (m_files.size() >= maxFiles) {
		for (auto it = m_files.begin(); it != m_files.end(); it++) {
			if (it->second.count() == 1) {
				m_files.erase(it);
				break;
			}
		}
	}

	auto& file = m_files.tryEmplace(
		p,
		std::fopen(absolutePath(path).string().c_str(), mStr),
		flushAndCloseFile
	).first->second;

	assert(file && "Failed to load file!");

	return File(file, (buffered ? globals::fileSystem->unusedBuffer() : nullptr), path.c_str());
}

char* FileSystem::unusedBuffer() {
	if (m_buffers.empty()) m_buffers.pushBack(new char[bufferSize]);

	auto r = m_buffers.back();
	m_buffers.popBack();
	return r;
}
	
void FileSystem::returnBuffer(char* buffer) {
	memset(buffer, '\0', bufferSize);
	m_buffers.pushBack(buffer);
}

File FileSystem::tmpFile() {
	auto s = 
#ifdef _WIN32
		std::to_wstring(
#else
		std::to_string(
#endif
			std::time(nullptr)); // could be funny
	m_files.emplace(s, std::tmpfile());
	return File(m_files.at(s), nullptr, { });
}

/*
BasicFile<char>::BasicFile(const std::filesystem::path& path, OpenMode mode, bool buffered) : 
	m_stream(globals::fileSystem->load(path, enumToOpenMode(mode))),
	m_path(path),
	m_buffered(buffered){
	if (m_buffered) { 
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}
}
BasicFile<char>::BasicFile(const std::filesystem::path& path, const char* mode, bool buffered) : 
	m_stream(globals::fileSystem->load(path, mode)),
	m_path(path),
	m_buffered(buffered) {
	if (m_buffered) { 
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}
}
BasicFile<wchar_t>::BasicFile(const std::filesystem::path& path, OpenMode mode, bool buffered) : 
	m_stream(globals::fileSystem->load(path, enumToOpenMode(mode))), 
	m_path(path), 
 	m_buffered(buffered) {
	if (m_buffered) { 
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}

	std::fwide(m_stream.get(), 1);
}
BasicFile<wchar_t>::BasicFile(const std::filesystem::path& path, const char* mode, bool buffered) : 
	m_stream(globals::fileSystem->load(path, mode)), 
	m_path(path),
	m_buffered(buffered) {
	if (m_buffered) { 
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}

	std::fwide(m_stream.get(), 1);
}
*/

BasicFile<char>::~BasicFile<char>() {
	if (m_buffer) globals::fileSystem->returnBuffer(m_buffer);
}
void BasicFile<char>::close() {
	if (m_buffer) globals::fileSystem->returnBuffer(m_buffer);
}

void BasicFile<char>::disableBuffering() {
	if (m_buffered) {
		std::fflush(m_stream.get());
		std::setbuf(m_stream.get(), nullptr);
		globals::fileSystem->returnBuffer(m_buffer);
	}
}
void BasicFile<char>::enableBuffering() {
	if (!m_buffered) {
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}
}

int BasicFile<char>::get() {
	return std::fgetc(m_stream.get());
}
BasicFile<char>& BasicFile<char>::get(char& c) {
	c = std::fgetc(m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::get(char* string, std::size_t count) {
	std::fgets(string, static_cast<std::uint32_t>(count), m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::putback(int c) {
	std::ungetc(c, m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::unget() {
	std::fseek(m_stream.get(), -1, 0);
	return *this;
}
BasicFile<char>& BasicFile<char>::read(char* string, std::size_t count) {
	std::fread(string, sizeof(char), count, m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::read(void* string, std::size_t size, std::size_t count) {
	std::fread(string, size, count, m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::put(char c) {
	std::fputc(c, m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::write(const void* string, std::size_t size, std::size_t count) {
	std::fwrite(string, size, count, m_stream.get());
	return *this;
}
BasicFile<char>& BasicFile<char>::write(const char* string, std::size_t count) {
	std::fwrite(string, sizeof(char), count, m_stream.get());
	return *this;
}

std::fpos_t BasicFile<char>::tellg() const {
	return std::ftell(m_stream.get());
}
std::fpos_t BasicFile<char>::tellp() const {
	return std::ftell(m_stream.get());
}
BasicFile<char>& BasicFile<char>::seekg(std::fpos_t pos) {
	std::fseek(m_stream.get(), pos, SEEK_SET);
	return *this;
}
BasicFile<char>& BasicFile<char>::seekg(std::fpos_t off, SeekDirection dir) {
	std::fseek(m_stream.get(), off, static_cast<int>(dir));
	return *this;
}
BasicFile<char>& BasicFile<char>::seekp(std::fpos_t pos) {
	std::fseek(m_stream.get(), pos, SEEK_SET);
	return *this;
}
BasicFile<char>& BasicFile<char>::seekp(std::fpos_t off, SeekDirection dir) {
	std::fseek(m_stream.get(), off, static_cast<int>(dir));
	return *this;
}
std::size_t BasicFile<char>::size() const {
	auto p = std::ftell(m_stream.get());
	std::fseek(m_stream.get(), 0, SEEK_END);
	auto r = std::ftell(m_stream.get());
	std::fseek(m_stream.get(), p, SEEK_SET);
	return r;
}

BasicFile<char>& BasicFile<char>::flush() {
	std::fflush(m_stream.get());
	return *this;
}
int BasicFile<char>::sync() {
	return std::fflush(m_stream.get());
}

bool BasicFile<char>::good() const {
	return m_stream.get();
}
bool BasicFile<char>::eof() const {
	return std::feof(m_stream.get()) != 0;
}
void BasicFile<char>::clear() {
	std::clearerr(m_stream.get());
}

void BasicFile<char>::swap(BasicFile<char>& file) {
	std::swap(flush().m_stream, file.flush().m_stream);
	std::swap(m_buffer, file.m_buffer);
	m_path.swap(file.m_path);
	std::swap(m_buffered, file.m_buffered);
}

void BasicFile<char>::rename(const std::filesystem::path& newPath) {
	std::filesystem::rename(globals::fileSystem->absolutePath(m_path), globals::fileSystem->absolutePath(newPath));
}
std::filesystem::path BasicFile<char>::absolutePath() const {
	return globals::fileSystem->absolutePath(m_path);
}


BasicFile<wchar_t>::~BasicFile() {
	if (m_buffer) globals::fileSystem->returnBuffer(m_buffer);
}
void BasicFile<wchar_t>::close() {
	if (m_buffer) globals::fileSystem->returnBuffer(m_buffer);
}

void BasicFile<wchar_t>::disableBuffering() {
	if (m_buffered) {
		std::fflush(m_stream.get());
		std::setbuf(m_stream.get(), nullptr);
	}
}
void BasicFile<wchar_t>::enableBuffering() {
	if (!m_buffered) {
		std::setvbuf(m_stream.get(), globals::fileSystem->unusedBuffer(), _IOFBF, FileSystem::bufferSize);
	}
}

int BasicFile<wchar_t>::get() {
	return std::fgetwc(m_stream.get());
}
BasicFile<wchar_t>& BasicFile<wchar_t>::get(wchar_t& c) {
	c = std::fgetwc(m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::get(wchar_t* string, std::size_t count) {
	std::fgetws(string, static_cast<std::uint32_t>(count), m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::putback(int c) {
	std::ungetwc(c, m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::unget() {
	std::fseek(m_stream.get(), -1, 0);
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::read(wchar_t* string, std::size_t count) {
	std::fread(string, sizeof(wchar_t), count, m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::read(void* string, std::size_t size, std::size_t count) {
	std::fread(string, size, count, m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::put(wchar_t c) {
	std::fputwc(c, m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::write(const void* string, std::size_t size, std::size_t count) {
	std::fwrite(string, size, count, m_stream.get());
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::write(const wchar_t* string, std::size_t count) {
	std::fwrite(string, sizeof(wchar_t), count, m_stream.get());
	return *this;
}

std::fpos_t BasicFile<wchar_t>::tellg() const {
	return std::ftell(m_stream.get());
}
std::fpos_t BasicFile<wchar_t>::tellp() const {
	return std::ftell(m_stream.get());
}
BasicFile<wchar_t>& BasicFile<wchar_t>::seekg(std::fpos_t pos) {
	std::fseek(m_stream.get(), pos, SEEK_SET);
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::seekg(std::fpos_t off, SeekDirection dir) {
	std::fseek(m_stream.get(), off, static_cast<int>(dir));
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::seekp(std::fpos_t pos) {
	std::fseek(m_stream.get(), pos, SEEK_SET);
	return *this;
}
BasicFile<wchar_t>& BasicFile<wchar_t>::seekp(std::fpos_t off, SeekDirection dir) {
	std::fseek(m_stream.get(), off, static_cast<int>(dir));
	return *this;
}
std::size_t BasicFile<wchar_t>::size() const {
	auto p = std::ftell(m_stream.get());
	std::fseek(m_stream.get(), 0, SEEK_END);
	auto r = std::ftell(m_stream.get());
	std::fseek(m_stream.get(), p, SEEK_SET);
	return r;
}

BasicFile<wchar_t>& BasicFile<wchar_t>::flush() {
	std::fflush(m_stream.get());
	return *this;
}
int BasicFile<wchar_t>::sync() {
	return std::fflush(m_stream.get());
}

bool BasicFile<wchar_t>::good() const {
	return (std::ferror(m_stream.get()) != 0) && (m_stream);
}
bool BasicFile<wchar_t>::eof() const {
	return std::feof(m_stream.get()) != 0;
}
void BasicFile<wchar_t>::clear() {
	std::clearerr(m_stream.get());
}

void BasicFile<wchar_t>::swap(BasicFile& file) {
	std::swap(flush().m_stream, file.flush().m_stream);
	std::swap(m_buffer, file.m_buffer);
	m_path.swap(file.m_path);
	std::swap(m_buffered, file.m_buffered);
}

void BasicFile<wchar_t>::rename(const std::filesystem::path& newPath) {
	std::filesystem::rename(globals::fileSystem->absolutePath(m_path), globals::fileSystem->absolutePath(newPath));
}
std::filesystem::path BasicFile<wchar_t>::absolutePath() const {
	return globals::fileSystem->absolutePath(m_path);
}

} // namespace esengine
