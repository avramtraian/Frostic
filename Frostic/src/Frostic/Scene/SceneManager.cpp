#include "fepch.h"
#include "SceneManager.h"

#include "SceneSerializer.h"

namespace Frostic {

	/////////////////////////////////////////////////////////////////
	/////////////////////  EDITOR  //////////////////////////////////
	/////////////////////////////////////////////////////////////////

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
		m_RuntimeScene->DestroyScripts();
		m_RuntimeScene = nullptr;
		m_Runtime = false;
		m_StopRuntimeCallback(m_BaseScene);
		m_BaseScene = nullptr;
	}

	/////////////////////////////////////////////////////////////////
	/////////////////////  RUNTIME  /////////////////////////////////
	/////////////////////////////////////////////////////////////////

	uint32_t SceneManagerRuntime::m_BuildIndex;

	SceneManagerRuntime::LoadCallbackFunction SceneManagerRuntime::m_LoadCallback;
	SceneManagerRuntime::UnloadCallbackFunction SceneManagerRuntime::m_UnloadCallback;

	void SceneManagerRuntime::Initialize(const LoadCallbackFunction& load, const UnloadCallbackFunction& unload)
	{
		m_LoadCallback = load;
		m_UnloadCallback = unload;
	}

	void SceneManagerRuntime::LoadScene(uint32_t buildIndex)
	{
		UnloadCurrentScene();
		Ref<Scene> scene = CreateRef<Scene>();

		SceneSerializer serializer(scene);
		// TODO: REMOVE!
		EditorCamera camera;
		// TODO: Change (and implement) to 'DeserializeRuntime'
		serializer.Deserialize(FILEPATH("Frosted/assets/scenes/FlappyBird.frostic"), camera);

		m_BuildIndex = buildIndex;

		m_LoadCallback(scene);
	}

	void SceneManagerRuntime::UnloadCurrentScene()
	{
		m_UnloadCallback(0);
	}

}