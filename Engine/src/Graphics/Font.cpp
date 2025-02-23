
#include <Graphics/Font.h>

#include <Core/FileSystem.h>

#include <LSD/JSON.h>

namespace esengine {

Font::Font(lsd::StringView fontPath) {
	File file = globals::fileSystem->load(globals::fileSystem->assetsPath() / fontPath.data(), OpenMode::read, false);

	lsd::String fontFileData(file.size(), '\0');
	file.read(fontFileData.data(), fontFileData.size());

	lsd::Json fontData = lsd::Json::parse(fontFileData);

	std::filesystem::path fntPth = fontPath.data();
	m_texture = Texture(fntPth.remove_filename().append(fontData.at("img").string().data()).c_str());

	const auto& cellSiz = fontData.at("cell").array();
	m_cellSize.x = cellSiz[0].unsignedInt();
	m_cellSize.y = cellSiz[1].unsignedInt();

	const auto& bounds = fontData.at("bds").array();
	m_capLine = bounds[0].unsignedInt();
	m_baseLine = bounds[1].unsignedInt();

	const auto& charDim = fontData.at("chr").array();
	m_chars.reserve(charDim.size());

	auto charDimIt = charDim.begin();

	for (auto y = 0; y < m_texture.dimension().y; y += m_cellSize.y) {
		for (auto x = 0; x < m_texture.dimension().x; x += m_cellSize.x) {
			m_chars.emplaceBack(SDL_FRect {
				lsd::implicitCast<float>(x + charDimIt->unsignedInt()),
				lsd::implicitCast<float>(y),
				lsd::implicitCast<float>((++charDimIt)->unsignedInt()),
				lsd::implicitCast<float>(m_cellSize.y)
			});

			++charDimIt;
		}
	}
}

const SDL_FRect& Font::at(int c) const noexcept {
	return m_chars[c];
}

} //namespace esengine
