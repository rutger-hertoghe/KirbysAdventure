#include "pch.h"
#include "TextureManager.h"
#include <filesystem>
#include <iostream>
#include "Texture.h"

TextureManager* TextureManager::m_pTextureManager{ nullptr };

TextureManager::TextureManager()
{
	if (m_pTextureManager)
	{
		delete m_pTextureManager;
	}
	m_pTextureManager = this;

	LoadBackgroundTextures();
	LoadSpriteTextures();
}

TextureManager::~TextureManager()
{
	for (std::pair<std::string, Texture*> pMappedTexture : m_pTextureMap)
	{
		delete pMappedTexture.second;
	}
}

Texture* TextureManager::Get(const std::string& filename)
{
	return m_pTextureManager->m_pTextureMap[filename];
}

void TextureManager::LoadBackgroundTextures()
{
	const std::string path{ "resources/backgrounds" };

	AddTexture("part1_background", path);
	AddTexture("part2_background", path);
	AddTexture("part3_background", path);
	AddTexture("part1_farbackground", path);
	AddTexture("part2_farbackground", path);
	AddTexture("part3_farbackground", path);
	AddTexture("part1_foreground", path);
	AddTexture("part2_foreground", path);
	AddTexture("part3_foreground", path);
	AddTexture("part1_midground", path);
	AddTexture("part2_midground", path);
	AddTexture("part3_midground", path);
	AddTexture("part1_full", path);
	AddTexture("part2_full", path);
	AddTexture("part3_full", path);
}

void TextureManager::LoadSpriteTextures()
{
	const std::string path{ "resources/sprites" };


}

void TextureManager::AddTexture(const std::string& name, const std::string& path)
{
	m_pTextureMap.insert(std::pair<std::string, Texture*>(name, new Texture{ path + '/' + name + ".png"}));
}
