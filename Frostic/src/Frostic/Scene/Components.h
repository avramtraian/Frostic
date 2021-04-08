#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Frostic/Renderer/Texture.h"
#include "Frostic/Core/Timestep.h"

#include <vector>
#include "Frostic/Core/FEArray.h"
#include "Frostic/Core/FEVector.h"
#include "Frostic/Core/FEString.h"

#define NULL_COMPONENT false

#define FECOMPONENT \
	bool Active = true; \
	uint32_t EnttOwnerID = 0; \
	Scene* Context = nullptr

namespace Frostic {
	
	struct TagComponent
	{
		FECOMPONENT;

		uint64_t UUID = 0;
		std::string Tag;

		TagComponent() {}
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		FECOMPONENT;

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		FECOMPONENT;

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::string TexturePath;
		Ref<Texture2D> Texture;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		FECOMPONENT;

		Frostic::SceneCamera Camera;
		bool Primary = true; // TODO: Move to Scene
		bool FixedAspectRatio = false;
		
		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		FECOMPONENT;

		ScriptableEntity* Instance = nullptr;
		bool Initialized = false;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);
		
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}