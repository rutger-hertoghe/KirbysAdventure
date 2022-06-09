#pragma once
#include <unordered_map>

class Texture;

class TextureManager final
{
public:
	explicit TextureManager();
	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;
	~TextureManager();

	static Texture* Get(const std::string& filename);

private:
	static TextureManager* m_pTextureManager;
	std::unordered_map<std::string, Texture*> m_pTextureMap;

	void LoadBackgroundTextures();
	void LoadSpriteTextures();
	// Uses folder path, not texture path. Name is also without .png extension.
	void AddTexture(const std::string& name, const std::string& path); 
};

