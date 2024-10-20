#include "assetbrowser.h"
#include <filesystem>

namespace ChronoShift
{
	AssetBrowser::AssetBrowser()
	{
		LoadAllDirectories();
	}

	void AssetBrowser::LoadAllDirectories()
	{
		m_directories.clear();

		for (const auto& entry : std::filesystem::recursive_directory_iterator(m_root_directory))
		{
			AddToDirectoryStructure(entry);
		}
		for (auto& [folder_path, folder] : m_directories)
		{
			if (folder_path.parent_path() == "")
			{
				m_root_folder.subfolders[folder_path] = &folder;
			}
		}
	}

	void AssetBrowser::AddToDirectoryStructure(const std::filesystem::path& entry)
	{
		auto relative_path = std::filesystem::relative(entry, m_root_directory);
		auto parent_path = relative_path.parent_path();

		Folder* current_folder = nullptr;
		//check which folder this entry is within
		for (auto& [folder_name, folder] : m_directories)
		{
			if (parent_path == folder.path)
			{
				current_folder = &folder;
				break;
			}
		}

		if (std::filesystem::is_directory(entry))
		{
			//Add to directory list if havent yet
			if (m_directories.find(relative_path) == m_directories.end())
			{
				m_directories[relative_path] = Folder(relative_path);
				m_directories[relative_path].name = relative_path.filename().string();

			}
			
			//Add to the parent.subfolder if its a is directory
			if (m_directories.find(parent_path) != m_directories.end())
			{
				m_directories[parent_path].subfolders[relative_path] = &m_directories[relative_path];
			}
		}
		else
		{
			//Check if top level file
			if (parent_path.string() == "") 
			{
				m_root_folder.files.push_back(entry);
			}
			else
			{
				// Add file to folder normally
				m_directories[parent_path].files.push_back(entry);
			}

		}
	}

	void AssetBrowser::RenderFolder(Folder& folder)
	{
		if (ImGui::TreeNode(folder.name.c_str()))
		{
			// Render directories first
			for (auto& [subfolder_path, subfolder] : folder.subfolders)
			{
				RenderFolder(*subfolder);
			}

			// Render the rest of the files in the folder
			for (const auto& file : folder.files)
			{
				ImGui::Selectable(file.filename().string().c_str());
			}
			ImGui::TreePop();
		}
	}

	void AssetBrowser::EditorUI()
	{
		ImGui::Begin("Asset Browser");

		for (auto& [path, folder] : m_root_folder.subfolders)
		{
			RenderFolder(*folder);
		}

		for (const auto& file : m_root_folder.files)
		{
			ImGui::Selectable(file.filename().string().c_str());
		}
		ImGui::End();
	}
}