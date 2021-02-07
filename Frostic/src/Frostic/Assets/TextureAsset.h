#pragma once

#include "Frostic/Core/Core.h"

#include "Frostic/Renderer/Texture.h"
#include "AssetCore.h"

namespace Frostic {

	class TextureAsset
	{
	public:
		// TextureAsset() = default;
		TextureAsset(const std::string& filepath);

		const Ref<Texture2D>& GetTexture() const { return m_Texture; }
		Ref<Texture2D>& GetTexture() { return m_Texture; }

		static AssetType GetStaticType() { return AssetType::Texture; }
	private:
		Ref<Texture2D> m_Texture;
	};

}