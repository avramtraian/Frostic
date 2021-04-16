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

	struct PhysicsComponent2D
	{
		FECOMPONENT;

		glm::vec2 Force = glm::vec2(0.0f);
		glm::vec2 Acceleration = glm::vec2(0.0f);
		glm::vec2 Velocity = glm::vec2(0.0f);
		float AirResistanceCoefficient = 0.0f;
		float Mass = 1.0f;
		bool Gravity = false;
		float GravityAcceleration = -10.0f;

		void AddForce(float amount, const glm::vec2& direction)
		{
			Force.x += amount * direction.x;
			Force.y += amount * direction.y;
		}
		void SetForce(const glm::vec2& newForce)
		{
			Force.x = newForce.x;
			Force.y = newForce.y;
		}
		void AddAcceleration(float amount, const glm::vec2& direction)
		{
			Acceleration.x += amount * direction.x;
			Acceleration.y += amount * direction.y;
		}
		void SetAcceleration(const glm::vec2& newAcceleration)
		{
			Acceleration.x = newAcceleration.x;
			Acceleration.y = newAcceleration.y;
		}
		void AddVelocity(float amount, const glm::vec2& direction)
		{
			Velocity.x += amount * direction.x;
			Velocity.y += amount * direction.y;
		}
		void SetVelocity(const glm::vec2& newVelocity)
		{
			Velocity.x = newVelocity.x;
			Velocity.y = newVelocity.y;
		}

		void Update(Timestep ts)
		{
			if (Gravity)
				AddForce(GravityAcceleration * Mass, { 0.0f, 1.0f });

			if (AirResistanceCoefficient != 0.0f)
			{
				glm::vec2 airForce;
				airForce.x = glm::abs(1.225f * AirResistanceCoefficient * Velocity.x * Velocity.x / 2.0f);
				airForce.y = glm::abs(1.225f * AirResistanceCoefficient * Velocity.y * Velocity.y / 2.0f);
				glm::clamp(airForce.x, 0.0f, Force.x);
				glm::clamp(airForce.y, 0.0f, Force.y);

				if (Velocity.x > 0.0f)
					AddForce(-airForce.x, { 1.0f, 0.0f });
				else
					AddForce(airForce.x, { 1.0f, 0.0f });
				if (Velocity.y > 0.0f)
					AddForce(-airForce.y, { 0.0f, 1.0f });
				else
					AddForce(airForce.y, { 0.0f, 1.0f });
			}

			Acceleration.x = Force.x / Mass;
			Acceleration.y = Force.y / Mass;

			Force.x = 0.0f;
			Force.y = 0.0f;

			Velocity.x += Acceleration.x * ts;
			Velocity.y += Acceleration.y * ts;
		}

		void UpdatePosition(Timestep ts)
		{
			Entity entity{ (entt::entity)EnttOwnerID, Context };
			if (entity.HasComponent<TransformComponent>())
			{
				TransformComponent& tc = entity.GetComponent<TransformComponent>();
				tc.Translation.x += Velocity.x;
				tc.Translation.y += Velocity.y;
			}
		}

		PhysicsComponent2D() = default;
		PhysicsComponent2D(const PhysicsComponent2D& other) = default;
	};

	struct BoxCollider2D
	{
		FECOMPONENT;
	};

}