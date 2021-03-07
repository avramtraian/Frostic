#pragma once

#include "Scene.h"

namespace Frostic {

	class SceneManagerEditor
	{
	public:
		using RuntimeCallbackFunction = std::function<void(Ref<Scene>&)>;

		static void Initialize(const RuntimeCallbackFunction& start, const RuntimeCallbackFunction& stop);

		static void StartRuntimeScene(const Ref<Scene>& sceneToStart);
		static void StopRuntimeScene();

		static bool IsRuntime() { return m_Runtime; }
		static Ref<Scene>& GetBaseScene() { return m_BaseScene; }
		static Ref<Scene>& GetRuntimeScene() { return m_RuntimeScene; }
	private:
		static Ref<Scene> m_BaseScene;
		static Ref<Scene> m_RuntimeScene;

		static RuntimeCallbackFunction m_StartRuntimeCallback;
		static RuntimeCallbackFunction m_StopRuntimeCallback;

		static bool m_Runtime;
	};

	class SceneManagerRuntime
	{
	public:
		void LoadScene(uint32_t buildIndex);
		void UnloadCurrentScene();
	};

}