#include "frpch.h"
#include "Scene.h"

#include "Frostic/Math/Random.h"

#include "Components.h"
#include "Entity.h"
#include "Frostic/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Frostic {
	
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name, uint64_t uuid)
	{
		Entity entity = { m_Registry.create_safe(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		Math::Random::Init();
		tag.UUID = uuid == 0 ? Math::Random::Range<uint64_t>(0, 10000000000) : uuid;
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

		m_Registry.each_reverse([&](auto ent) {
			Entity entity { ent, this };

			Entity createdEntity = scene->CreateEntity(entity.GetComponent<TagComponent>().Tag);
			createdEntity.GetComponent<TagComponent>().Active = entity.GetComponent<TagComponent>().Active;
			createdEntity.GetComponent<TagComponent>().UUID = entity.GetComponent<TagComponent>().UUID;

			if (entity.HasComponent<TransformComponent>())
			{
				auto& tc = createdEntity.AddOrGetComponent<TransformComponent>();
				tc.Active = entity.GetComponent<TransformComponent>().Active;
				tc.Translation = entity.GetComponent<TransformComponent>().Translation;
				tc.Rotation = entity.GetComponent<TransformComponent>().Rotation;
				tc.Scale = entity.GetComponent<TransformComponent>().Scale;
			}

			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& src = createdEntity.AddOrGetComponent<SpriteRendererComponent>();
				src.Active = entity.GetComponent<SpriteRendererComponent>().Active;
				src.Color = entity.GetComponent<SpriteRendererComponent>().Color;
				src.Texture = entity.GetComponent<SpriteRendererComponent>().Texture;
				src.TexturePath = entity.GetComponent<SpriteRendererComponent>().TexturePath;
			}
			if (entity.HasComponent<CameraComponent>())
			{
				auto& cc = createdEntity.AddOrGetComponent<CameraComponent>();
				cc.Active = entity.GetComponent<CameraComponent>().Active;
				cc.Camera = entity.GetComponent<CameraComponent>().Camera;
				cc.FixedAspectRatio = entity.GetComponent<CameraComponent>().FixedAspectRatio;
				cc.Primary = entity.GetComponent<CameraComponent>().Primary;
			}
			if (entity.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = createdEntity.AddOrGetComponent<NativeScriptComponent>();
				nsc.Active = entity.GetComponent<NativeScriptComponent>().Active;
				nsc.InstantiateScript = entity.GetComponent<NativeScriptComponent>().InstantiateScript;
				nsc.DestroyScript = entity.GetComponent<NativeScriptComponent>().DestroyScript;
				if (entity.GetComponent<NativeScriptComponent>().Instance == nullptr)
					nsc.Instance = nullptr;
				else
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_EntityUUID = entity.GetComponent<NativeScriptComponent>().Instance->m_EntityUUID;

					for (size_t i = 0; i < nsc.Instance->m_DataReferences.size(); i++)
					{
						ScriptableEntity::PropertyData& data = nsc.Instance->m_DataReferences[i];
						ScriptableEntity::PropertyData& dataToCopy = entity.GetComponent<NativeScriptComponent>().Instance->m_DataReferences[i];
						switch (data.m_DataType)
						{
							case DataType::UINT8_T:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(uint8_t));
								break;
							case DataType::UINT16_T:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(uint16_t));
								break;
							case DataType::UINT32_T:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(uint32_t));
								break;
							case DataType::UINT64_T:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(uint64_t));
								break;
							case DataType::INT:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(int));
								break;
							case DataType::FLOAT:
								memcpy(data.m_Data, dataToCopy.m_Data, sizeof(float));
								break;
							default:
								break;
						}
					}
					for (size_t i = 0; i < nsc.Instance->m_EntityReferences.size(); i++)
					{
						ScriptableEntity::PropertyEntity& data = nsc.Instance->m_EntityReferences[i];
						ScriptableEntity::PropertyEntity& dataToCopy = entity.GetComponent<NativeScriptComponent>().Instance->m_EntityReferences[i];

						if (dataToCopy.m_Data != nullptr && dataToCopy.m_Data->Exists())
							data.UUID = dataToCopy.m_Data->GetComponent<TagComponent>().UUID;
					}
				}
			}
		});

		scene->m_Registry.view<NativeScriptComponent>().each([&](entt::entity entity, NativeScriptComponent& nsc)
			{
				if (nsc.Instance != nullptr)
				{
					nsc.Instance->m_Entity = scene->GetEntityByUUID(nsc.Instance->m_EntityUUID);
					for (size_t i = 0; i < nsc.Instance->m_EntityReferences.size(); i++)
					{
						ScriptableEntity::PropertyEntity& data = nsc.Instance->m_EntityReferences[i];

						if (data.UUID != 0)
							*data.m_Data = scene->GetEntityByUUID(data.UUID);
					}
				}
			});

		scene->m_ViewportWidth = m_ViewportWidth;
		scene->m_ViewportHeight = m_ViewportHeight;

		return scene;
	}

	Entity Scene::GetEntityByUUID(uint64_t uuid)
	{
		Entity entityToReturn;
		m_Registry.view<TagComponent>().each([&](entt::entity entity, TagComponent& tc)
			{
				if (tc.UUID == uuid)
				{
					entityToReturn = Entity(entity, this);
					return;
				}
			});
		FE_CORE_ASSERT(entityToReturn != NULL_ENTITY, "Invalid UUID search!");
		return entityToReturn;
	}

	void Scene::DestroyScripts()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				nsc.DestroyScript(&nsc);
			});
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
				{
					if (!nsc.Initialized)
					{
						nsc.Initialized = true;
						nsc.Instance->Begin();
					}

					nsc.Instance->Tick(ts);
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