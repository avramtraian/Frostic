#include "frpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Frostic/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Frostic {
	
	Scene::Scene()
	{

	}

	Scene::Scene(const Scene& other)
	{
		m_Registry.assign(other.m_Registry.data(), other.m_Registry.data() + other.m_Registry.size(), other.m_Registry.destroyed());
		m_ViewportWidth = other.m_ViewportWidth;
		m_ViewportHeight = other.m_ViewportHeight;
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	Ref<Scene> Scene::CopyScene()
	{
		Ref<Scene> scene = CreateRef<Scene>();

		m_Registry.each([&](auto ent) {
			Entity entity { ent, this };

			Entity createdEntity = scene->CreateEntity(entity.GetComponent<TagComponent>().Tag);
			createdEntity.GetComponent<TagComponent>().Active = entity.GetComponent<TagComponent>().Active;

			createdEntity.GetComponent<TransformComponent>().Active = entity.GetComponent<TransformComponent>().Active;
			createdEntity.GetComponent<TransformComponent>().Translation = entity.GetComponent<TransformComponent>().Translation;
			createdEntity.GetComponent<TransformComponent>().Rotation = entity.GetComponent<TransformComponent>().Rotation;
			createdEntity.GetComponent<TransformComponent>().Scale = entity.GetComponent<TransformComponent>().Scale;

			if (entity.HasComponent<SpriteRendererComponent>())
			{
				createdEntity.AddOrGetComponent<SpriteRendererComponent>();
				createdEntity.GetComponent<SpriteRendererComponent>().Active = entity.GetComponent<SpriteRendererComponent>().Active;
				createdEntity.GetComponent<SpriteRendererComponent>().Color = entity.GetComponent<SpriteRendererComponent>().Color;
				createdEntity.GetComponent<SpriteRendererComponent>().Texture = entity.GetComponent<SpriteRendererComponent>().Texture;
				createdEntity.GetComponent<SpriteRendererComponent>().TexturePath = entity.GetComponent<SpriteRendererComponent>().TexturePath;
			}
			if (entity.HasComponent<CameraComponent>())
			{
				createdEntity.AddOrGetComponent<CameraComponent>();
				createdEntity.GetComponent<CameraComponent>().Active = entity.GetComponent<CameraComponent>().Active;
				createdEntity.GetComponent<CameraComponent>().Camera = entity.GetComponent<CameraComponent>().Camera;
				createdEntity.GetComponent<CameraComponent>().FixedAspectRatio = entity.GetComponent<CameraComponent>().FixedAspectRatio;
				createdEntity.GetComponent<CameraComponent>().Primary = entity.GetComponent<CameraComponent>().Primary;
			}
		});

		return scene;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Rendering sprites
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), glm::inverse(cameraTransform));

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				Entity ent = { entity, this };
				if (ent.GetComponent<TransformComponent>().Active && ent.GetComponent<SpriteRendererComponent>().Active)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Specs.EntityID = (uint32_t)entity;
					Specs.Transform = transform.GetTransform();
					Specs.Texture = sprite.Texture;
					Specs.Color = sprite.Color;

					Renderer2D::DrawSprite(Specs);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			Entity ent = { entity, this };
			if (ent.GetComponent<TransformComponent>().Active && ent.GetComponent<SpriteRendererComponent>().Active)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Specs.EntityID = (uint32_t)entity;
				Specs.Transform = transform.GetTransform();
				Specs.Texture = sprite.Texture;
				Specs.Color = sprite.Color;

				Renderer2D::DrawSprite(Specs);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize the non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component)
	{

	}

}