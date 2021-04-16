#include "SceneHierarchyPanel.h"

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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static bool DrawFloatControl(const std::string& label, float* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragFloat("##Value", value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawFloatControl(const std::string& label, float* value, float min, float max, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::SliderFloat("##Value", value, min, max);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawUInt8_tControl(const std::string& label, uint8_t* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragInt("##Value", (int*)value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawUInt16_tControl(const std::string& label, uint16_t* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragInt("##Value", (int*)value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawUInt32_tControl(const std::string& label, uint32_t* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragInt("##Value", (int*)value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawUInt64_tControl(const std::string& label, uint64_t* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragInt("##Value", (int*)value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	static bool DrawIntControl(const std::string& label, int* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::DragInt("##Value", value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
	}

	template<typename T>
	static T* DrawComponentPointerField(const std::string& label, T* comPtr, Ref<Scene>& context, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		std::string name = "None";
		if (comPtr != nullptr)
		{
			Entity entity{ (entt::entity)comPtr->EnttOwnerID, context.get() };
			if (entity.IsValid())
				name = entity.GetComponent<TagComponent>().Tag;
		}

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::Text("%d", comPtr == nullptr ? -1 : comPtr->UUID);
		ImGui::NextColumn();

		if (ImGui::Button(name.c_str()))
			ImGui::OpenPopup("Pointer");

		if (ImGui::BeginPopup("Pointer"))
		{
			SceneHierarchyPanel::ForEachWhoHas<T>(context, [&](auto ent)
				{
					Entity entity{ ent, context.get() };
					if (ImGui::MenuItem(entity.GetComponent<TagComponent>().Tag.c_str()))
					{
						comPtr = entity.GetComponentP<T>();
						ImGui::CloseCurrentPopup();
					}
				});

			if (ImGui::MenuItem("NONE: nullptr"))
			{
				comPtr = nullptr;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Columns(1);

		ImGui::PopID();
		return comPtr;
	}

	static void DrawEntityReferenceField(const std::string& label, Entity& entRef, const Ref<Scene>& context, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		if (ImGui::Button(entRef.Exists() == false ? "None" : entRef.GetComponent<TagComponent>().Tag.c_str()))
			ImGui::OpenPopup("Pointer");

		if (ImGui::BeginPopup("Pointer"))
		{
			SceneHierarchyPanel::ForEachWhoHas<TagComponent>(context, [&](auto ent)
				{
					Entity entity{ ent, context.get() };
					if (ImGui::MenuItem(entity.GetComponent<TagComponent>().Tag.c_str()))
					{
						entRef = entity;
						ImGui::CloseCurrentPopup();
					}
				});

			if (ImGui::MenuItem("NONE: nullptr"))
			{
				entRef.Invalidate();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static ScriptableEntity* DrawScriptPointerField(const std::string& label, ScriptableEntity* se, uint64_t scriptID, const Ref<Scene>& context, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		if(ImGui::Button(se == nullptr ? "None" : se->GetEntity().GetComponent<TagComponent>().Tag.c_str()))
			ImGui::OpenPopup("ScriptPointer");

		if (ImGui::BeginPopup("ScriptPointer"))
		{
			SceneHierarchyPanel::ForEachWhoHas<NativeScriptComponent>(context, [&](auto ent) 
				{
					Entity entity{ ent, context.get() };
					NativeScriptComponent& entityNSC = entity.GetComponent<NativeScriptComponent>();
					if (entityNSC.Instance != nullptr)
					{
						if (scriptID != 0 && entityNSC.Instance->GetScriptID() == scriptID)
						{
							if (ImGui::MenuItem(entity.GetComponent<TagComponent>().Tag.c_str()))
							{
								se = entityNSC.Instance;
								ImGui::CloseCurrentPopup();
								return;
							}
						}
					}
				});

			if (ImGui::MenuItem("NONE: nullptr"))
			{
				se = nullptr;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Columns(1);

		ImGui::PopID();
		return se;
	}

	static bool DrawCheckbox(const std::string& label, bool* value, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		bool used = ImGui::Checkbox("##Value", value);

		ImGui::Columns(1);

		ImGui::PopID();

		return used;
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
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			DrawCheckbox("Primary", &component.Primary);

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
				if (DrawFloatControl("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (DrawFloatControl("Near Clip", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (DrawFloatControl("Far Clip", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (DrawFloatControl("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float aspectRatio = camera.GetOrthographicAspectRatio();
				if (DrawFloatControl("Aspect Ratio", &aspectRatio, 0.0f, 3.0f))
					camera.SetOrthographicAspectRatio(aspectRatio);

				float orthoNear = camera.GetOrthographicNearClip();
				if (DrawFloatControl("Near Clip", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (DrawFloatControl("Far Clip", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				DrawCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio, 150.0f);
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
									DrawUInt8_tControl(data.m_Label, static_cast<uint8_t*>(data.m_Data));
									break;
								case DataType::UINT16_T:
									DrawUInt16_tControl(data.m_Label, static_cast<uint16_t*>(data.m_Data));
									break;
								case DataType::UINT32_T:
									DrawUInt32_tControl(data.m_Label, static_cast<uint32_t*>(data.m_Data));
									break;
								case DataType::UINT64_T:
									DrawUInt64_tControl(data.m_Label, static_cast<uint64_t*>(data.m_Data));
									break;
								case DataType::INT:
									DrawIntControl(data.m_Label, static_cast<int*>(data.m_Data));
									break;
								case DataType::FLOAT:
									DrawFloatControl(data.m_Label, static_cast<float*>(data.m_Data));
									break;
								default:
									break;
							}
							break;
						case PropertyType::EntityReference:
							DrawEntityReferenceField(data.m_Label, *static_cast<Entity*>(data.m_Data), m_Context);
							break;
						case PropertyType::Script:
							*static_cast<ScriptableEntity**>(data.m_Data) = DrawScriptPointerField(data.m_Label, *static_cast<ScriptableEntity**>(data.m_Data), data.m_ScriptID, m_Context);
							break;
						default:
							break;
						}
					}
				}
			});

		DrawComponent<PhysicsComponent2D>("Physics Component", entity, [](PhysicsComponent2D& physics) 
			{
				DrawFloatControl("Mass", &physics.Mass, 175.0f);
				DrawFloatControl("Air Resistance Coefficient", &physics.AirResistanceCoefficient, 175.0f);
				DrawCheckbox("Use Gravity", &physics.Gravity, 175.0f);
				if (physics.Gravity)
					DrawFloatControl("Gravity Acceleration", &physics.GravityAcceleration, 175.0f);
			});
	}

}