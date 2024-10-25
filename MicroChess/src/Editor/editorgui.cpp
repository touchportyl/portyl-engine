/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// editorgui.cpp
//
// This is the Encapsulation of ImGUI functions to be of ease for other programmers to use.
// Lemme know if you need anything, and I'll try to create something easy to use!
//
//
// AUTHORS
// [100%] Rocky Sutarius (rocky.sutarius@digipen.edu)
//   - Main Author
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#pragma once
#include "editorgui.h"

using namespace FlexEngine;
namespace ChronoShift
{
	int EditorGUI::m_id = 0;

	void EditorGUI::DragFloat2(Vector2& data, std::string title,
		//std::string label1, std::string label2, 
		float width, float drag_speed)
	{
		if (title != "")
		{
			TextField(title);
		}
		PushID();
		ImGui::PushItemWidth(width);
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::DragFloat("", &data.x, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		PopID();
		PushID();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("", &data.y, drag_speed);
		ImGui::PopItemWidth();
		PopID();
	}

	void EditorGUI::DragFloat3(Vector3& data, std::string title,
		//std::string label1, std::string label2, std::string label3,
		float width, float drag_speed)
	{
		if (title != "")
		{
			TextField(title);
		}
		PushID();
		ImGui::PushItemWidth(width);
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::DragFloat("", &data.x, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		PopID();
		PushID();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("", &data.y, drag_speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		PopID();
		PushID();
		ImGui::Text("Z"); ImGui::SameLine();
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("", &data.z, drag_speed);
		ImGui::PopItemWidth();
		PopID();
	}

	void EditorGUI::DragInt(int& data, std::string title, float width, float drag_speed)
	{
		PushID();
		ImGui::PushItemWidth(width);
		ImGui::Text(title.c_str()); ImGui::SameLine();
		ImGui::DragInt("", &data, drag_speed);
		ImGui::PopItemWidth();
		PopID();
	}

	void EditorGUI::EntityReference(FlexECS::Entity& entity, std::string title)
	{
		PushID();
		std::string entity_name = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<FlexEngine::FlexECS::Scene::StringIndex>());

		ImGui::BeginGroup();
		ImGui::Text(title.c_str()); ImGui::SameLine();
		if (ImGui::Button(entity_name.c_str()))
		{
			Editor::GetInstance()->SelectEntity(entity);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
			{
				FlexECS::Entity dropped_entity = *(FlexECS::EntityID*)payload->Data;
				entity = dropped_entity;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::EndGroup();
		PopID();
	}

	void EditorGUI::ShaderPath(std::string& path, std::string title)
	{
		PushID();

		std::filesystem::path current_texture = path;
		std::string filename = current_texture.filename().string();
		if (filename == "") filename = "(no shader)";

		ImGui::Text("Shader");
		ImGui::SameLine();
		ImGui::Button(filename.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_PATH")) {
				const char* dropped_path = (const char*)payload->Data;
				std::string new_file_path(dropped_path);
				path = new_file_path; // Store the file path in the component
			}
			ImGui::EndDragDropTarget();
		}

		PopID();
	}

	void EditorGUI::TexturePath(std::string& path, std::string title)
	{
		PushID();
		
		std::filesystem::path current_texture = path;
		std::string filename = current_texture.filename().string();
		if (filename == "") filename = "(no sprite)";

		ImGui::Text("Sprite");
		ImGui::SameLine();
		ImGui::Button(filename.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_PATH")) {
				const char* dropped_path = (const char*)payload->Data;
				std::string new_file_path(dropped_path);
				path = new_file_path; // Store the file path in the component
			}
			ImGui::EndDragDropTarget();
		}

		if (filename != "(no sprite)")
		{
			std::string asset_key = current_texture.string();
			AssetVariant* asset_ptr = AssetManager::Get(asset_key);
			if (asset_ptr)
			{
				Asset::Texture& texture = std::get<Asset::Texture>(*asset_ptr);
				ImGui::Image(texture.GetTextureImGui(), ImVec2(60.0f, 60.0f));
			}
		}

		PopID();
	}

	void EditorGUI::Color3(Vector3& data, std::string title)
	{
		PushID();
		ImGui::ColorEdit3(title.c_str(), data.data);
		PopID();
	}

	void EditorGUI::EditableTextField(std::string& data, std::string title)
	{
		PushID();
		char text_buffer[128];
		strncpy_s(text_buffer, data.c_str(), sizeof(text_buffer));
		text_buffer[sizeof(text_buffer) - 1] = '\0';
		if (ImGui::InputText(title.c_str(), text_buffer, IM_ARRAYSIZE(text_buffer)))
		{
			data = std::string(text_buffer);
		}
		PopID();
	}

	void EditorGUI::TextField(const std::string& data)
	{
		PushID();
		ImGui::Text(data.c_str());
		PopID();
	}

	void EditorGUI::Checkbox(bool& data, std::string title)
	{
		PushID();
		ImGui::Checkbox(title.c_str(), &data);
		PopID();
	}

	void EditorGUI::Mat44(FlexEngine::Matrix4x4& data, std::string title)
	{
		PushID();
		
		// Create a label for the matrix
		ImGui::Text("%s", title.c_str());

		ImGui::PushItemWidth(60.0f);

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				PushID();
				ImGui::DragFloat("##", &data(row, col), 0.1f);
				PopID();

				if (col < 3) ImGui::SameLine();
			}
		}
		ImGui::PopItemWidth();

		PopID();
	}




	void EditorGUI::StartFrame()
	{
		m_id = 0;
	}

	void EditorGUI::EndFrame()
	{
		m_id = 0;
	}

	int EditorGUI::PushID()
	{
		ImGui::PushID(m_id);
		return m_id++;
	}

	void EditorGUI::PopID()
	{
		ImGui::PopID();
	}


}
