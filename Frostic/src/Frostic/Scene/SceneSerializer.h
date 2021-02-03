#pragma once

#include "Scene.h"

namespace Frostic {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath, const EditorCamera& camera);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath, EditorCamera& camera);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};

}