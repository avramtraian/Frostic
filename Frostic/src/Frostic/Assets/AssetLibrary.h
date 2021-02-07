#pragma once

#include <string>
#include <unordered_map>

#include "Frostic/Renderer/Texture.h"

#include "TextureAsset.h"

namespace Frostic {
	
	class AssetLibrary
	{
	public:
		template<typename T>
		static Ref<T> Load(const std::string& filepath)
		{
			if (T::GetStaticType() == AssetType::Texture)
				return LoadTexture(filepath);
			else
				FR_CORE_ASSERT(false, "Unknown asset template type!");
			return nullptr;
		}

		template<typename T>
		static Ref<T> Get(const std::string& filepath)
		{
			if (T::GetStaticType() == AssetType::Texture)
				return GetTexture(filepath);
			else
				FR_CORE_ASSERT(false, "Unknown asset template type!");
			return nullptr;
		}

		template<typename T>
		static Ref<T> GetOrLoad(const std::string& filepath)
		{
			if (T::GetStaticType() == AssetType::Texture)
			{
				if (ExistsTexture(filepath))
					return GetTexture(filepath);
				else
					return LoadTexture(filepath);
			}
			else
			{
				FR_CORE_ASSERT(false, "Unknown asset template type!");
				return nullptr;
			}
		}
	private:
		static void AddTexture(const Ref<TextureAsset>& textureAsset);

		static Ref<TextureAsset> LoadTexture(const std::string& filepath);

		static Ref<TextureAsset> GetTexture(const std::string& filepath);

		static bool ExistsTexture(const std::string& filepath);
	private:
		static std::unordered_map<std::string, Ref<TextureAsset>> m_Textures;
	};

}