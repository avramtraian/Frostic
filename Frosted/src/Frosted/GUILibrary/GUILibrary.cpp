#include "GUILibrary.h"

#include "Frostic/Scene/Components.h"
#include "../../Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Frostic::GUI {

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/, float columnWidth /*= 100.0f*/)
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

	bool DrawFloatControl(const std::string& label, float* value, float columnWidth /*= 100.0f*/)
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

	bool DrawFloatRangeControl(const std::string& label, float* value, float min, float max, float columnWidth /*= 100.0f*/)
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

	bool DrawUInt8_tControl(const std::string& label, uint8_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawUInt16_tControl(const std::string& label, uint16_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawUInt32_tControl(const std::string& label, uint32_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawUInt64_tControl(const std::string& label, uint64_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawInt8_tControl(const std::string& label, int8_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawInt16_tControl(const std::string& label, int16_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawInt32_tControl(const std::string& label, int32_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawInt64_tControl(const std::string& label, int64_t* value, float columnWidth /*= 100.0f*/)
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

	bool DrawIntControl(const std::string& label, int* value, float columnWidth /*= 100.0f*/)
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

	void DrawEntityReferenceField(const std::string& label, Entity& entRef, const Ref<Scene>& context, float columnWidth /*= 100.0f*/)
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

	ScriptableEntity* DrawScriptPointerField(const std::string& label, ScriptableEntity* se, uint64_t scriptID, const Ref<Scene>& context, float columnWidth /*= 100.0f*/)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		if (ImGui::Button(se == nullptr ? "None" : se->GetEntity().GetComponent<TagComponent>().Tag.c_str()))
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

	bool DrawCheckbox(const std::string& label, bool* value, float columnWidth /*= 100.0f*/)
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

	void DrawFEArray(const std::string& label, void* data, DataType dataType, float columnWidth /*= 100.0f*/)
	{

	}

	void DrawFEString(const std::string& label, FEString& string, float columnWidth /*= 100.0f*/)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		char buffer[256];
		memset(&buffer, 0, 256);
		memcpy(&buffer, string.Data(), strlen(string.Data()));
		ImGui::InputText("##string", buffer, 256);
		string.Resize(strlen(buffer) + 1);
		memcpy(string.Data(), &buffer, strlen(buffer));
		string[string.Size() - 1] = 0;

		ImGui::Columns(1);

		ImGui::PopID();
	}

}