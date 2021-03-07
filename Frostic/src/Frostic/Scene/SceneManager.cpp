#include "frpch.h"
#include "SceneManager.h"

namespace Frostic {

	Ref<Scene> SceneManagerEditor::m_BaseScene;
	Ref<Scene> SceneManagerEditor::m_RuntimeScene;

	SceneManagerEditor::RuntimeCallbackFunction SceneManagerEditor::m_StartRuntimeCallback;
	SceneManagerEditor::RuntimeCallbackFunction SceneManagerEditor::m_StopRuntimeCallback;

	bool SceneManagerEditor::m_Runtime = false;

	void SceneManagerEditor::Initialize(const RuntimeCallbackFunction& start, const RuntimeCallbackFunction& stop)
	{
		m_StartRuntimeCallback = start;
		m_StopRuntimeCallback = stop;
	}

	void SceneManagerEditor::StartRuntimeScene(const Ref<Scene>& sceneToStart)
	{
		m_BaseScene = sceneToStart;
		m_RuntimeScene = m_BaseScene->CopyScene();
		m_Runtime = true;
		m_StartRuntimeCallback(m_RuntimeScene);
	}

	void SceneManagerEditor::StopRuntimeScene()
	{
		m_RuntimeScene = nullptr;
		m_Runtime = false;
		m_StopRuntimeCallback(m_BaseScene);
		m_BaseScene = nullptr;
	}

	void SceneManagerRuntime::LoadScene(uint32_t buildIndex)
	{

	}

	void SceneManagerRuntime::UnloadCurrentScene()
	{

	}

}