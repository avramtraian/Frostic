#include "frpch.h"
#include "AssetLibrary.h"

namespace Frostic {

	std::unordered_map<std::string, Ref<TextureAsset>> AssetLibrary::m_Textures;

	void AssetLibrary::AddTexture(const Ref<TextureAsset>& textureAsset)
	{
		// TODO: Implement!
		FR_CORE_ASSERT(false, "Not implemented!");
	}

	Ref<TextureAsset> AssetLibrary::LoadTexture(const std::string& filepath)
	{
		FR_CORE_ASSERT(!ExistsTexture(filepath), "Texture already exists!");
		m_Textures[filepath] = CreateRef<TextureAsset>(filepath);
		return m_Textures[filepath];
	}

	Ref<TextureAsset> AssetLibrary::GetTexture(const std::string& filepath)
	{
		FR_CORE_ASSERT(ExistsTexture(filepath), "Texture doesn't exists!");
		return m_Textures[filepath];
	}

	bool AssetLibrary::ExistsTexture(const std::string& filepath)
	{
		return m_Textures.find(filepath) != m_Textures.end();
	}

}