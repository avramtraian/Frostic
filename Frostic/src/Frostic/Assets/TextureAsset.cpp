#include "fepch.h"
#include "TextureAsset.h"

namespace Frostic {

	TextureAsset::TextureAsset(const std::string& filepath)
	{
		m_Texture = Texture2D::Create(filepath);
	}

}