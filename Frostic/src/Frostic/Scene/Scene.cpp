#include "fepch.h"
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
		tag.UUID = uuid == 0 ? Math::Random::Range<uint64_t>(1, 100000000000000) : uuid;
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.HasComponent<NativeScriptComponent>())
		{
			if (entity.GetComponent<NativeScriptComponent>().Instance != nullptr)
				entity.GetComponent<NativeScriptComponent>().DestroyScript(&entity.GetComponent<NativeScriptComponent>());
		}
		m_Registry.destroy(entity);
	}

	Ref<Scene> Scene::CopyScene()
	{
		Ref<Scene> scene = CreateRef<Scene>();

		m_Registry.each_reverse([&](auto ent) {
			Entity entity { ent, this };

			Entity createdEntity = scene->CreateEntity(entity.GetComponent<TagComponent>().Tag, entity.GetComponent<TagComponent>().UUID);
			createdEntity.GetComponent<TagComponent>().Active = entity.GetComponent<TagComponent>().Active;

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
			if (entity.HasComponent<PhysicsComponent2D>())
			{
				auto& physics = createdEntity.AddOrGetComponent<PhysicsComponent2D>();
				physics.Active = entity.GetComponent<PhysicsComponent2D>().Active;
				physics.Force = entity.GetComponent<PhysicsComponent2D>().Force;
				physics.Acceleration = entity.GetComponent<PhysicsComponent2D>().Acceleration;
				physics.Velocity = entity.GetComponent<PhysicsComponent2D>().Velocity;
				physics.AirResistanceCoefficient = entity.GetComponent<PhysicsComponent2D>().AirResistanceCoefficient;
				physics.Mass = entity.GetComponent<PhysicsComponent2D>().Mass;
				physics.Gravity = entity.GetComponent<PhysicsComponent2D>().Gravity;
				physics.GravityAcceleration = entity.GetComponent<PhysicsComponent2D>().GravityAcceleration;
			}
			if (entity.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = createdEntity.AddOrGetComponent<NativeScriptComponent>();
				nsc.Active = entity.GetComponent<NativeScriptComponent>().Active;
				nsc.InstantiateScript = *entity.GetComponent<NativeScriptComponent>().InstantiateScript;
				nsc.DestroyScript = *entity.GetComponent<NativeScriptComponent>().DestroyScript;
				if (entity.GetComponent<NativeScriptComponent>().Instance == nullptr)
					nsc.Instance = nullptr;
				else
				{
					nsc.Instance = nsc.InstantiateScript();
					FE_CORE_ASSERT(nsc.Instance != nullptr, "Instance is nullptr!");
					nsc.Instance->m_EntityUUID = entity.GetComponent<NativeScriptComponent>().Instance->m_EntityUUID;

					for (size_t i = 0; i < nsc.Instance->_m_Properties.size(); i++)
					{
						ScriptableEntity::_PropertyData& data = nsc.Instance->_m_Properties[i];
						ScriptableEntity::_PropertyData& dataToCopy = entity.GetComponent<NativeScriptComponent>().Instance->_m_Properties[i];
						switch (data.m_PropertyType)
						{
						case PropertyType::Data:
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
							break;
						case PropertyType::EntityReference:
							if (dataToCopy.m_Data != nullptr && static_cast<Entity*>(dataToCopy.m_Data)->Exists())
								data.m_EntityUUID = static_cast<Entity*>(dataToCopy.m_Data)->GetComponent<TagComponent>().UUID;
							break;
						case PropertyType::Script:
							if (*static_cast<ScriptableEntity**>(dataToCopy.m_Data) != nullptr)
							{
								ScriptableEntity* se = *static_cast<ScriptableEntity**>(dataToCopy.m_Data);
								data.m_EntityUUID = se->m_Entity.GetComponent<TagComponent>().UUID;
							}
							break;
						default:
							break;
						}
					}
				}
			}
		});

		scene->m_Registry.view<NativeScriptComponent>().each([&](entt::entity entity, NativeScriptComponent& nsc)
			{
				if (nsc.Instance != nullptr)
					nsc.Instance->m_Entity = scene->GetEntityByUUID(nsc.Instance->m_EntityUUID);
			});

		scene->m_Registry.view<NativeScriptComponent>().each([&](entt::entity entity, NativeScriptComponent& nsc) 
			{
				if (nsc.Instance != nullptr)
				{
					for (size_t i = 0; i < nsc.Instance->_m_Properties.size(); i++)
					{
						ScriptableEntity::_PropertyData& data = nsc.Instance->_m_Properties[i];

						switch (data.m_PropertyType)
						{
							case PropertyType::EntityReference:
								if (data.m_EntityUUID != 0)
									*static_cast<Entity*>(data.m_Data) = scene->GetEntityByUUID(data.m_EntityUUID);
								break;
							case PropertyType::Script:
								if (data.m_EntityUUID != 0)
									*static_cast<ScriptableEntity**>(data.m_Data) = scene->GetEntityByUUID(data.m_EntityUUID).GetComponent<NativeScriptComponent>().Instance;
								break;
						default:
							break;
						}
					}
				}
			});

		scene->m_ViewportWidth = m_ViewportWidth;
		scene->m_ViewportHeight = m_ViewportHeight;

		return scene;
	}

	Entity Scene::GetEntityByUUID(uint64_t uuid, bool mightFail)
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
		FE_CORE_ASSERT(entityToReturn != NULL_ENTITY || mightFail, "Invalid UUID search!");
		return entityToReturn;
	}

	void Scene::DestroyScripts()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.Instance != nullptr)
					nsc.DestroyScript(&nsc);
			});
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
				{
					if (nsc.Instance != nullptr)
					{
						if (nsc.Initialized)
							nsc.Instance->Tick(ts);
						else
						{
							nsc.Initialized = true;
							nsc.Instance->Begin();
						}
					}
				});
		}

		// Update Physics
		{
			m_Registry.view<PhysicsComponent2D>().each([&ts](auto entity, auto& physics)
				{
					physics.OnUpdate(ts);
					physics.OnUpdatePosition(ts);
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

		m_Registry.view<CameraComponent>().each([&](auto ent, CameraComponent& cc)
			{
				Entity entity{ ent, cc.Context };
				if (entity.HasComponent<TransformComponent>())
				{
					TransformComponent tc;
					tc.Translation = entity.GetComponent<TransformComponent>().Translation;
					tc.Rotation = entity.GetComponent<TransformComponent>().Rotation;
					tc.Scale = { cc.Camera.GetOrthographicSize() * cc.Camera.GetOrthographicAspectRatio(), cc.Camera.GetOrthographicSize(), 1.0f };
					Specs.Transform = tc.GetTransform();
					Specs.EntityID = (uint32_t)entt::null;
					Specs.Texture = nullptr;
					Specs.Color = { 0.0f, 0.5f, 0.0f, 0.1f };

					Renderer2D::DrawSprite(Specs);
					return;
				}
			});

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Registry.view<CameraComponent>().each([&](auto ent, CameraComponent& cc) 
			{
				if (cc.Primary)
				{
					m_ViewportWidth = width;
					m_ViewportHeight = m_ViewportWidth / cc.Camera.GetOrthographicAspectRatio();
					DEBUG_ONLY(glm::clamp(m_ViewportWidth, (uint32_t)0, width));
					DEBUG_ONLY(glm::clamp(m_ViewportHeight, (uint32_t)0, height));
					return;
				}
			});
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

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, PhysicsComponent2D& component)
	{

	}

}