/**************************
 * @file FileSystem.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief A Filesystem responsible for managing and interfacing to the engines loaded files using an standard-esque interface
 * 
 * @date 2024-31-08
 * 
 * @copyright Copyright (c) 2023
 *************************/

#pragma once

#include <LSD/SharedPointer.h>
#include <LSD/Vector.h>
#include <LSD/String.h>
#include <LSD/UnorderedSparseMap.h>

#include <type_traits>
#include <algorithm>
#include <filesystem>

namespace esengine {

class FilesystemError : public std::runtime_error {
public:
	FilesystemError(const lsd::String& message) : std::runtime_error(message.cStr()) {
		m_message.append(message).pushBack('!');
	}
	FilesystemError(const char* message) : std::runtime_error(message) {
		m_message.append(message).pushBack('!');
	}
	FilesystemError(const FilesystemError&) = default;
	FilesystemError(FilesystemError&&) = default;

	FilesystemError& operator=(const FilesystemError&) = default;
	FilesystemError& operator=(FilesystemError&&) = default;

	const char* what() const noexcept override {
		return m_message.cStr();
	}

private:
	lsd::String m_message { "Program terminated with FilesystemError: " };
};


enum class OpenMode {
	read,
	write,
	append,
	extend = 0x04,
	binary = 0x08
};

enum class SeekDirection {
	begin = SEEK_SET,
	current = SEEK_CUR,
	end = SEEK_END
};

enum class FileState {
	good = 0x00000000,
	bad = 0x0000001,
	fail = 0x00000002,
	eof = 0x00000004
};


template <class> class BasicFile;

template <> class BasicFile<char> {
public: 
	using literal_type = char;
	using file_type = lsd::SharedPointer<std::FILE>;

	BasicFile() = default;
	BasicFile(const BasicFile&) = default;
	BasicFile(BasicFile&&) = default;
	BasicFile& operator=(const BasicFile&) = default;
	BasicFile& operator=(BasicFile&&) = default;

	~BasicFile();
	void close();

	void disableBuffering();
	void enableBuffering();

	int get();
	BasicFile& get(char& c);
	BasicFile& get(char* string, std::size_t count);
	BasicFile& putback(int c);
	BasicFile& unget();
	BasicFile& read(char* string, std::size_t count);
	BasicFile& read(void* string, std::size_t size, std::size_t count);

	BasicFile& put(char c);
	BasicFile& write(const void* string, std::size_t size, std::size_t count);
	BasicFile& write(const char* string, std::size_t count);

	BasicFile& flush();
	int sync();

	std::fpos_t tellg() const;
	std::fpos_t tellp() const;
	BasicFile& seekg(std::fpos_t pos);
	BasicFile& seekg(std::fpos_t off, SeekDirection dir);
	BasicFile& seekp(std::fpos_t pos);
	BasicFile& seekp(std::fpos_t off, SeekDirection dir);
	std::size_t size() const;

	bool good() const;
	bool eof() const;
	bool fail() const;
	bool operator!() const {
		return fail();
	}
	operator bool() const {
		return !fail();
	}
	void clear();

	void swap(BasicFile& file);

	void rename(const std::filesystem::path& newPath);
	[[nodiscard]] std::filesystem::path absolutePath() const;
	[[nodiscard]] std::filesystem::path path() const noexcept {
		return m_path;
	}
	[[nodiscard]] bool buffered() const noexcept {
		return m_buffered;
	}
	[[nodiscard]] const file_type& stream() const noexcept {
		return m_stream;
	}
	[[nodiscard]] file_type& stream() noexcept {
		return m_stream;
	}

private:
	file_type m_stream = nullptr;
	char* m_buffer = nullptr;

	std::filesystem::path m_path;

	bool m_buffered;

	BasicFile(file_type file, char* buffer, lsd::BasicStringView<std::filesystem::path::string_type::value_type> path) : m_stream(file), m_buffer(buffer), m_path(path.data()) { 
		if (m_buffer) enableBuffering();
	}

	friend class FileSystem;
};

template <> class BasicFile<wchar_t> {
public:
	using literal_type = wchar_t;
	using file_type = lsd::SharedPointer<std::FILE>;

	BasicFile() = default;
	BasicFile(const BasicFile&) = default;
	BasicFile(BasicFile&&) = default;
	BasicFile& operator=(const BasicFile&) = default;
	BasicFile& operator=(BasicFile&&) = default;

	~BasicFile();
	void close();

	void disableBuffering();
	void enableBuffering();

	int get();
	BasicFile& get(wchar_t& c);
	BasicFile& get(wchar_t* string, std::size_t count);
	BasicFile& putback(int c);
	BasicFile& unget();
	BasicFile& read(wchar_t* string, std::size_t count);
	BasicFile& read(void* string, std::size_t size, std::size_t count);

	BasicFile& put(wchar_t c);
	BasicFile& write(const void* string, std::size_t size, std::size_t count);
	BasicFile& write(const wchar_t* string, std::size_t count);

	BasicFile& flush();
	int sync();

	std::fpos_t tellg() const;
	std::fpos_t tellp() const;
	BasicFile& seekg(std::fpos_t pos);
	BasicFile& seekg(std::fpos_t off, SeekDirection dir);
	BasicFile& seekp(std::fpos_t pos);
	BasicFile& seekp(std::fpos_t off, SeekDirection dir);
	std::size_t size() const;
	
	bool good() const;
	bool eof() const;
	bool fail() const;
	bool operator!() const {
		return fail();
	}
	operator bool() const {
		return !fail();
	}
	void clear();

	void swap(BasicFile& file);

	void rename(const std::filesystem::path& newPath);
	[[nodiscard]] std::filesystem::path absolutePath() const;
	[[nodiscard]] std::filesystem::path path() const noexcept {
		return m_path;
	}
	[[nodiscard]] bool buffered() const noexcept {
		return m_buffered;
	}
	[[nodiscard]] const file_type& stream() const noexcept {
		return m_stream;
	}
	[[nodiscard]] file_type& stream() noexcept {
		return m_stream;
	}

private:
	file_type m_stream = nullptr;
	char* m_buffer = nullptr;

	std::filesystem::path m_path = { };

	bool m_buffered;

	BasicFile(file_type file, char* buffer, lsd::BasicStringView<std::filesystem::path::string_type::value_type> path) : m_stream(file), m_buffer(buffer), m_path(path.data()) { 
		if (m_buffer) enableBuffering();
	}

	friend class FileSystem;
};

using File = BasicFile<char>;
using WFile = BasicFile<wchar_t>;


class FileSystem {
public:
	static constexpr std::size_t bufferSize = BUFSIZ;
	static constexpr std::size_t maxFiles = FOPEN_MAX;

	using string_type = std::filesystem::path::string_type;

	FileSystem(char** argv);

	std::filesystem::path absolutePath(const std::filesystem::path& path) const;
	std::filesystem::path localPath(const std::filesystem::path& path) const;
	std::filesystem::path assetsPath() const;

	bool exists(const std::filesystem::path& path) const;
	bool contains(const std::filesystem::path& path, OpenMode mode) const;

	[[nodiscard]] File load(const std::filesystem::path& path, OpenMode mode, bool buffered);
	[[nodiscard]] File tmpFile();

private:
	[[nodiscard]] char* unusedBuffer();
	
	void returnBuffer(char* buffer);

	lsd::UnorderedSparseMap<string_type, lsd::SharedPointer<std::FILE>> m_files;
	lsd::Vector<char*> m_buffers;

	std::filesystem::path m_absPathBase;

	template <class> friend class BasicFile;
};

} // namespace esengine
