#include "FlexEngine.h"


namespace ChronoShift
{

	struct Folder
	{
		std::filesystem::path path;
		std::string name;
		std::vector<std::filesystem::path> files;
		std::unordered_map<std::filesystem::path, Folder*> subfolders;

		Folder() = default;

		Folder(const std::filesystem::path& folder_path)
			: path(folder_path), name(folder_path.string())
		{}
	};

	class AssetBrowser
	{
	public:
		AssetBrowser();

		void LoadAllDirectories();
		void EditorUI();

	private:
		std::filesystem::path m_current_directory = "assets";
		const std::filesystem::path m_root_directory = "assets";

		std::filesystem::path m_selected_path = "";

		Folder m_root_folder;
		std::unordered_map<std::filesystem::path, Folder> m_directories;

		void AddToDirectoryStructure(const std::filesystem::path& entry);
		void RenderFolder(Folder& folder);

	};


}