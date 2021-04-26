#include "SceneHierarchyPanel.h"

#include "../Frosted/GUILibrary/GUILibrary.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Frostic/Core/Core.h"

#include "Frostic/Scene/Components.h"
#include "Frostic/Assets/AssetLibrary.h"

#include "Frostic/Utils/PlatformUtils.h"

#include "Frostic/Script/ScriptManager.h"

namespace Frostic {
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::SetSelectionContext(Entity selectionContext)
	{
		m_SelectionContext = selectionContext;
	}

	void SceneHierarchyPanel::SetSelectionContextFromID(int id)
	{
		if (id != -1)
			m_SelectionContext = { (entt::entity)id, m_Context.get() };
		else
			m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();

		bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
			m_Context->DestroyEntity(entity);
		}
	}

	

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			T& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.5f);
			bool active = entity.GetComponent<T>().Active;
			if (ImGui::Checkbox("##Active", &active))
			{
				entity.GetComponent<T>().Active = active;
			}
			
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentsSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentsSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Physics Component"))
			{
				m_SelectionContext.AddComponent<PhysicsComponent2D>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Native Script Component"))
			{
				m_SelectionContext.AddComponent<NativeScriptComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			GUI::DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			GUI::DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			GUI::DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			GUI::DrawCheckbox("Primary", &component.Primary);

			const char* projectionTypeString[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);

			ImGui::Text("Projection");
			ImGui::NextColumn();

			if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeString[i];
					if (ImGui::Selectable(projectionTypeString[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeString[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Columns(1);

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (GUI::DrawFloatControl("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (GUI::DrawFloatControl("Near Clip", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (GUI::DrawFloatControl("Far Clip", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (GUI::DrawFloatControl("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float aspectRatio = camera.GetOrthographicAspectRatio();
				if (GUI::DrawFloatRangeControl("Aspect Ratio", &aspectRatio, 0.0f, 3.0f))
					camera.SetOrthographicAspectRatio(aspectRatio);

				float orthoNear = camera.GetOrthographicNearClip();
				if (GUI::DrawFloatControl("Near Clip", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (GUI::DrawFloatControl("Far Clip", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				GUI::DrawCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio, 150.0f);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Color");
			ImGui::NextColumn();
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.TexturePath.c_str());

			ImGui::Columns(1);
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - 100.0f);
			ImGui::Text("Texture Path");
			ImGui::NextColumn();
			ImGui::PushItemWidth(ImGui::CalcItemWidth());
			if (!AssetLibrary::Exists<TextureAsset>(std::string(buffer)))
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });
			else
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

			if (ImGui::InputText("##TexturePath", buffer, sizeof(buffer)))
			{
				component.TexturePath = std::string(buffer);
				component.Texture = AssetLibrary::GetOrLoad<TextureAsset>(std::string(buffer))->GetTexture();
				if (!component.Texture->IsValid())
				{
					AssetLibrary::Remove<TextureAsset>(component.TexturePath);
					component.Texture = nullptr;
				}
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::Button("..."))
			{
				component.TexturePath = FileDialogs::OpenFile("Frostic Texture(*.*)\0 * .*\0");
				component.Texture = AssetLibrary::GetOrLoad<TextureAsset>(component.TexturePath)->GetTexture();
				if (!component.Texture->IsValid())
				{
					AssetLibrary::Remove<TextureAsset>(component.TexturePath);
					component.Texture = nullptr;
				}
			}
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		});

		DrawComponent<NativeScriptComponent>("Native Component", entity, [&](NativeScriptComponent& nsc)
			{
				if (ImGui::Button(nsc.Instance != nullptr ? ScriptManager::GetNameFromID(nsc.Instance->GetScriptID()).c_str() : "Select Script"))
					ImGui::OpenPopup("SelectScript");

				if (ImGui::BeginPopup("SelectScript"))
				{
					ScriptManager::Each([&](uint64_t id,ScriptManager::Data& data) 
						{
							if (ImGui::MenuItem(data.Name.c_str()))
							{
								if (nsc.Instance != nullptr)
									nsc.DestroyScript(&nsc);

								nsc.InstantiateScript = ScriptManager::CreateInstantiateScriptByID(id);
								nsc.DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
								nsc.Instance = nsc.InstantiateScript();
								FE_CORE_ASSERT_AND_RETURN(nsc.Instance != nullptr, "Instance was null!");
								nsc.Instance->m_Entity.m_Scene = m_Context.get();
								nsc.Instance->m_Entity.m_EntityHandle = (entt::entity)(uint32_t)entity;
								nsc.Instance->m_EntityUUID = entity.GetComponent<TagComponent>().UUID;

								ImGui::CloseCurrentPopup();
							}
						});

					if (ImGui::MenuItem("None"))
					{
						if (nsc.Instance != nullptr)
							nsc.DestroyScript(&nsc);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				if (nsc.Instance != nullptr)
				{
					for (ScriptableEntity::_PropertyData& data : nsc.Instance->_m_Properties)
					{
						switch (data.m_PropertyType)
						{
						case PropertyType::Data:
							switch (data.m_DataType)
							{
							case DataType::UINT8_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawUInt8_tControl(data.m_Label, static_cast<uint8_t*>(data.m_Data));
								break;
							case DataType::UINT16_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawUInt16_tControl(data.m_Label, static_cast<uint16_t*>(data.m_Data));
								break;
							case DataType::UINT32_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawUInt32_tControl(data.m_Label, static_cast<uint32_t*>(data.m_Data));
								break;
							case DataType::UINT64_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawUInt64_tControl(data.m_Label, static_cast<uint64_t*>(data.m_Data));
								break;
							case DataType::INT8_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawInt8_tControl(data.m_Label, static_cast<int8_t*>(data.m_Data));
								break;
							case DataType::INT16_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawInt16_tControl(data.m_Label, static_cast<int16_t*>(data.m_Data));
								break;
							case DataType::INT32_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawInt32_tControl(data.m_Label, static_cast<int32_t*>(data.m_Data));
								break;
							case DataType::INT64_T:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawInt64_tControl(data.m_Label, static_cast<int64_t*>(data.m_Data));
								break;
							case DataType::INT:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawIntControl(data.m_Label, static_cast<int*>(data.m_Data));
								break;
							case DataType::FLOAT:
								if (data.m_Acces == AccesModifiers::ReadAndWrite)
									GUI::DrawFloatControl(data.m_Label, static_cast<float*>(data.m_Data));
								break;
							default:
								break;
							}
							break;
						case PropertyType::EntityReference:
							if (data.m_Acces == AccesModifiers::ReadAndWrite)
								GUI::DrawEntityReferenceField(data.m_Label, *static_cast<Entity*>(data.m_Data), m_Context);
							break;
						case PropertyType::Script:
							if (data.m_Acces == AccesModifiers::ReadAndWrite)
								*static_cast<ScriptableEntity**>(data.m_Data) = GUI::DrawScriptPointerField(data.m_Label, *static_cast<ScriptableEntity**>(data.m_Data), data.m_ScriptID, m_Context);
							break;
						case PropertyType::DataStructure:
						{
							switch (data.m_DataStructureType)
							{
								case DataStructureType::FEStringType:
									if (data.m_Acces == AccesModifiers::ReadAndWrite)
									{
										FEString* string = static_cast<FEString*>(data.m_Data);
										GUI::DrawFEString(data.m_Label, *string);
									}
									else
									{
										ImGui::Columns(2);
										ImGui::SetColumnWidth(0, 100.0f);
										ImGui::Text(data.m_Label.c_str());
										ImGui::NextColumn();
										ImGui::Text(static_cast<FEString*>(data.m_Data)->Data());
										ImGui::Columns(1);
									}
									break;
								case DataStructureType::FEArrayType:
									if (data.m_Acces == AccesModifiers::ReadAndWrite)
										GUI::DrawFEArray(data.m_Label, data.m_Data, data.m_DataType);
									break;
								default:
									break;
							}
							break;
						}
						default:
							break;
						}
					}
				}
			});

		DrawComponent<PhysicsComponent2D>("Physics Component", entity, [](PhysicsComponent2D& physics) 
			{
				GUI::DrawFloatControl("Mass", &physics.Mass, 175.0f);
				GUI::DrawFloatControl("Air Resistance Coefficient", &physics.AirResistanceCoefficient, 175.0f);
				GUI::DrawCheckbox("Use Gravity", &physics.Gravity, 175.0f);
				if (physics.Gravity)
					GUI::DrawFloatControl("Gravity Acceleration", &physics.GravityAcceleration, 175.0f);
			});
	}

}