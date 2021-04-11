#include "frpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "SceneCamera.h"
#include "Components.h"

#include "Frostic/Assets/AssetLibrary.h"
#include "Frostic/Script/ScriptManager.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3 rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4 rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Frostic {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& values)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << values.x << values.y << values.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& values)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << values.x << values.y << values.z << values.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entities
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<TagComponent>().UUID; // The operator overload takes care

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << tag.Tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Active" << YAML::Value << tc.Active;
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;
			out << YAML::Key << "Camera";
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Active" << YAML::Value << cc.Active;
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Active" << YAML::Value << spriteRenderer.Active;
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;
			out << YAML::Key << "TexturePath" << YAML::Value << spriteRenderer.TexturePath;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap; // NativeScriptComponent

			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			out << YAML::Key << "Active" << YAML::Value << nsc.Active;
			out << YAML::Key << "Instance";
			out << YAML::BeginMap; // Instance;
			if (nsc.Instance != nullptr)
			{
				out << YAML::Key << "EntityUUID" << YAML::Value << nsc.Instance->GetEntityUUID();
				for (size_t i = 0; i < nsc.Instance->_m_Properties.size(); i++)
				{
					ScriptableEntity::_PropertyData& data = nsc.Instance->_m_Properties[i];
					out << YAML::Key << "PropertyData[" + std::to_string(i) + "]";
					out << YAML::BeginMap; // PropertyData[i]
					switch (data.m_PropertyType)
					{
						case PropertyType::Data:
							switch (data.m_DataType)
							{
								case DataType::UINT8_T:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<uint8_t*>(data.m_Data);
									break;
								case DataType::UINT16_T:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<uint16_t*>(data.m_Data);
									break;
								case DataType::UINT32_T:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<uint32_t*>(data.m_Data);
									break;
								case DataType::UINT64_T:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<uint64_t*>(data.m_Data);
									break;
								case DataType::INT:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<int*>(data.m_Data);
									break;
								case DataType::FLOAT:
									out << YAML::Key << "Data" << YAML::Value << *static_cast<float*>(data.m_Data);
									break;
								default:
									break;
							}
							break;
						case PropertyType::EntityReference:
							if (data.m_Data != nullptr && static_cast<Entity*>(data.m_Data)->Exists())
								out << YAML::Key << "EntityUUID" << YAML::Value << static_cast<Entity*>(data.m_Data)->GetComponent<TagComponent>().UUID;
							else
								out << YAML::Key << "EntityUUID" << YAML::Value << 0;
							break;
						case PropertyType::Script:
							if (*static_cast<ScriptableEntity**>(data.m_Data) != nullptr)
							{
								ScriptableEntity* se = *static_cast<ScriptableEntity**>(data.m_Data);
								out << YAML::Key << "EntityUUID" << YAML::Value << se->GetEntity().GetComponent<TagComponent>().UUID;
							}
							else
								out << YAML::Key << "EntityUUID" << YAML::Value << 0;
							break;
						default:
							break;
					}
					out << YAML::EndMap; // PropertyData[i]
				}
			}
			out << YAML::EndMap; // Instance

			out << YAML::EndMap; // NativeScriptComponent
		}

		out << YAML::EndMap; // Entities
	}

	static void SerializeEditorCamera(YAML::Emitter& out, const EditorCamera& camera)
	{
		out << YAML::Key << "EditorCamera" << YAML::BeginMap; // EditorCamera

		out << YAML::Key << "ProjectionType" << (int)camera.GetProjectionType();
		out << YAML::Key << "Translation" << camera.GetTranslation();
		out << YAML::Key << "Rotation" << camera.GetRotation();
		out << YAML::Key << "PerspectiveFOV" << camera.GetPerspectiveFOV();
		out << YAML::Key << "PerspectiveNearClip" << camera.GetNearClip();
		out << YAML::Key << "PerspectiveFarClip" << camera.GetPerspectiveFarClip();

		out << YAML::EndMap; // EditorCamera
	}

	void SceneSerializer::Serialize(const std::string& filepath, const EditorCamera& camera)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Scene
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each_reverse([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;

		SerializeEditorCamera(out, camera);

		out << YAML::EndMap; // Scene

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		FE_CORE_ASSERT(false, "Not implemented!");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath, EditorCamera& camera)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			FE_CORE_ERROR("Unable to find keyword 'Scene'");
			FE_CORE_ERROR("File may be corrupted!");
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		FE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				FE_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name, uuid);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Active = transformComponent["Active"].as<bool>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Active = cameraComponent["Active"].as<bool>();
					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteComponent = entity["SpriteRendererComponent"];
				if (spriteComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Active = spriteComponent["Active"].as<bool>();
					src.Color = spriteComponent["Color"].as<glm::vec4>();
					src.TexturePath = spriteComponent["TexturePath"].as<std::string>();
					src.Texture = AssetLibrary::GetOrLoad<TextureAsset>(src.TexturePath)->GetTexture();
					if (AssetLibrary::RemoveIfInvalid<TextureAsset>(src.TexturePath))
						src.Texture = nullptr;
				}

				auto nsComponent = entity["NativeScriptComponent"];
				if (nsComponent)
				{
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
					nsc.Active = nsComponent["Active"].as<bool>();
					if (nsc.Instance != nullptr)
					{
						nsc.Instance->m_EntityUUID = nsComponent["Instance"]["EntityUUID"].as<uint64_t>();
						for (size_t i = 0; i < nsc.Instance->_m_Properties.size(); i++)
						{
							auto property = nsComponent["Instance"]["Property[" + std::to_string(i) + "]"];
							ScriptableEntity::_PropertyData& data = nsc.Instance->_m_Properties[i];
							switch (data.m_PropertyType)
							{
							case PropertyType::Data:
								switch (data.m_DataType)
								{
								case DataType::UINT8_T:
								{
									uint8_t dataToCopy = property["Data"].as<uint8_t>();
									memcpy(data.m_Data, &dataToCopy, sizeof(uint8_t));
									break;
								}
								case DataType::UINT16_T:
								{
									uint16_t dataToCopy = property["Data"].as<uint16_t>();
									memcpy(data.m_Data, &dataToCopy, sizeof(uint16_t));
									break;
								}
								case DataType::UINT32_T:
								{
									uint32_t dataToCopy = property["Data"].as<uint32_t>();
									memcpy(data.m_Data, &dataToCopy, sizeof(uint32_t));
									break;
								}
								case DataType::UINT64_T:
								{
									uint64_t dataToCopy = property["Data"].as<uint64_t>();
									memcpy(data.m_Data, &dataToCopy, sizeof(uint64_t));
									break;
								}
								case DataType::INT:
								{
									int dataToCopy = property["Data"].as<int>();
									memcpy(data.m_Data, &dataToCopy, sizeof(int));
									break;
								}
								case DataType::FLOAT:
								{
									float dataToCopy = property["Data"].as<float>();
									memcpy(data.m_Data, &dataToCopy, sizeof(float));
									break;
								}
								default:
									break;
								}
								break;
							case PropertyType::EntityReference:
								data.m_EntityUUID = property["EntityUUID"].as<uint64_t>();
								break;
							case PropertyType::Script:
								data.m_EntityUUID = property["EntityUUID"].as<uint64_t>();
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}

		m_Scene->m_Registry.view<NativeScriptComponent>().each([&](entt::entity ent, NativeScriptComponent& nsc)
			{
				if (nsc.Instance != nullptr)
					nsc.Instance->m_Entity = m_Scene->GetEntityByUUID(nsc.Instance->m_EntityUUID);
			});

		m_Scene->m_Registry.view<NativeScriptComponent>().each([&](entt::entity ent, NativeScriptComponent& nsc)
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
								*static_cast<Entity*>(data.m_Data) = m_Scene->GetEntityByUUID(data.m_EntityUUID);
							break;
						case PropertyType::Script:
							if (data.m_EntityUUID != 0)
								*static_cast<ScriptableEntity**>(data.m_Data) = m_Scene->GetEntityByUUID(data.m_EntityUUID).GetComponent<NativeScriptComponent>().Instance;
							break;
						default:
							break;
						}
					}
				}
			});

		auto editorCamera = data["EditorCamera"];
		if (editorCamera)
		{
			FE_CORE_TRACE("Deserializing Editor Camera");
			camera.SetProjectionType((EditorCamera::ProjectionType)editorCamera["ProjectionType"].as<int>());
			camera.SetTranslation(editorCamera["Translation"].as<glm::vec3>());
			camera.SetRotation(editorCamera["Rotation"].as<glm::vec3>());
			camera.SetPerspectiveFOV(editorCamera["PerspectiveFOV"].as<float>());
			camera.SetNearClip(editorCamera["PerspectiveNearClip"].as<float>());
			camera.SetPerspectiveFarClip(editorCamera["PerspectiveFarClip"].as<float>());
		}

		FE_CORE_TRACE("Deserializing complete!");
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		/*/ Not implemented
		FE_CORE_ASSERT(false, "Not implemented!");
		return false;*/

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			FE_CORE_ERROR("Unable to find keyword 'Scene'");
			FE_CORE_ERROR("File may be corrupted!");
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		FE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				FE_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name, uuid);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Active = transformComponent["Active"].as<bool>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Active = cameraComponent["Active"].as<bool>();
					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteComponent = entity["SpriteRendererComponent"];
				if (spriteComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Active = spriteComponent["Active"].as<bool>();
					src.Color = spriteComponent["Color"].as<glm::vec4>();
					src.TexturePath = spriteComponent["TexturePath"].as<std::string>();
					src.Texture = AssetLibrary::GetOrLoad<TextureAsset>(src.TexturePath)->GetTexture();
					if (AssetLibrary::RemoveIfInvalid<TextureAsset>(src.TexturePath))
						src.Texture = nullptr;
				}
			}
		}

		FE_CORE_TRACE("Deserializing complete!");
		return true;
	}

}