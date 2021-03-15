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
		using LoadCallbackFunction = std::function<void(Ref<Scene>&)>;
		using UnloadCallbackFunction = std::function<void(int)>;

		static void Initialize(const LoadCallbackFunction& load, const UnloadCallbackFunction& unload);

		static void LoadScene(uint32_t buildIndex);
		static void UnloadCurrentScene();

		static uint32_t GetActiveSceneBuildIndex() { return m_BuildIndex; }
	private:
		static uint32_t m_BuildIndex;

		static LoadCallbackFunction m_LoadCallback;
		static UnloadCallbackFunction m_UnloadCallback;
	};

}