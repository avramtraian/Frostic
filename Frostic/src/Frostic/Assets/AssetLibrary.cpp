#include "fepch.h"
#include "AssetLibrary.h"

namespace Frostic {

	std::unordered_map<std::string, Ref<TextureAsset>> AssetLibrary::m_Textures;

	bool AssetLibrary::AddTexture(const Ref<TextureAsset>& textureAsset)
	{
		// TODO: Implement!
		FE_CORE_ASSERT(false, "Not implemented!");
		return false;
	}

	Ref<TextureAsset> AssetLibrary::LoadTexture(const std::string& filepath)
	{
		FE_CORE_ASSERT(!ExistsTexture(filepath), "Texture already exists!");
		m_Textures[filepath] = CreateRef<TextureAsset>(filepath);
		return m_Textures[filepath];
	}

	Ref<TextureAsset> AssetLibrary::GetTexture(const std::string& filepath)
	{
		FE_CORE_ASSERT(ExistsTexture(filepath), "Texture doesn't exists!");
		return m_Textures[filepath];
	}

	bool AssetLibrary::RemoveTexture(const std::string& filepath)
	{
		FE_CORE_ASSERT(ExistsTexture(filepath), "Texture doesn't exists!");
		m_Textures[filepath] = nullptr;
		m_Textures.erase(filepath);
		return true;
	}

	bool AssetLibrary::RemoveTextureIfInvalid(const std::string& filepath)
	{
		FE_CORE_ASSERT(ExistsTexture(filepath), "Texture doesn't exists!");
		if (!m_Textures[filepath]->GetTexture()->IsValid())
			return RemoveTexture(filepath);
		else
			return false;
	}

	bool AssetLibrary::RemoveTexture(const Ref<TextureAsset>& textureAsset)
	{
		// TODO: Implement!
		FE_CORE_ASSERT(false, "Not implemented!");
		return false;
	}

	bool AssetLibrary::ExistsTexture(const std::string& filepath)
	{
		return m_Textures.find(filepath) != m_Textures.end();
	}
}