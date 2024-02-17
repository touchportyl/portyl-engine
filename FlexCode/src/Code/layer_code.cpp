#include "layer_code.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/imgui_stdlib.h>

using namespace FlexEngine;

namespace FlexCode
{

  struct EditableFile
  {
    std::filesystem::path path{}; // file path object
    std::string name{};           // file name without the extension
    std::string extension{};      // file extension with the dot
    std::string data{};           // data read from the file
    bool is_open = true;         // flag for closing the file
    bool is_dirty = false;       // flag for checking if the file has been modified
    bool to_dock = true;         // flag for docking the window

    // overload the == and != operators to compare files
    bool operator==(const EditableFile& other)
    {
      return path == other.path;
    }
    bool operator!=(const EditableFile& other)
    {
      return path != other.path;
    }

    EditableFile(const std::filesystem::path& path)
      : path(path), name(path.filename().generic_string()), extension(path.extension().generic_string())
    {
      std::ifstream ifs(path);
      FLX_ASSERT(ifs, "Could not open file " + path.string());

      // automatically read the file
      std::string _data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      data = _data;
    }
  };


  std::vector<EditableFile> files; // vector of files
  std::filesystem::path base_path = std::filesystem::current_path(); // base path for the content browser
  EditableFile* window_popup_save_file = nullptr; // pointer to the file that is being saved

  EditableFile* last_focused_file = nullptr; // pointer to the most recent focused file
  EditableFile* focused_file = nullptr; // pointer to the file that is currently focused, nullptr if none


  static bool IsEditable(const std::string& extension)
  {
    bool flag = false;
    std::vector<std::string> common_extensions = {
      ".txt", ".md", ".log", ".json", ".xml", ".yaml", ".yml", ".toml", ".csv",
      ".h", ".c", ".hpp", ".cpp", ".cc", ".cs", ".cxx", ".hxx", ".hh", ".inl",
      ".ini", ".cfg", ".conf", ".config", ".properties", ".prefs", ".settings",
    };

    // check against common extensions
    for (const auto& ext : common_extensions)
    {
      if (extension == ext)
      {
        flag = true;
        break;
      }
    }

    return flag;
  }
  static bool IsEditable(const std::filesystem::path& path)
  {
    return IsEditable(path.extension().string());
  }

  static void SaveFile(EditableFile& file)
  {
    std::ofstream out(file.path);
    FLX_ASSERT(out, "Could not open file " + file.path.string());
    out << file.data;
    file.is_dirty = false;
  }

  static void CloseFile(EditableFile& file)
  {
    file.is_open = false;
  }


  CodeLayer::CodeLayer()
    : Layer("Flex Code") {}

  void CodeLayer::OnAttach()
  {
    FLX_FLOW_FUNCTION();
  }

  void CodeLayer::OnDetach()
  {
    FLX_FLOW_FUNCTION();
  }

  void CodeLayer::OnUpdate()
  {

    // get the current active file
    ImGuiContext* context = ImGui::GetCurrentContext();
    if (context)
    {
      ImGuiWindow* focusedWindow = context->NavWindow;
      if (focusedWindow)
      {
        focused_file = nullptr; // default to nullptr

        // find the file that is currently focused
        for (auto& file : files)
        {
          std::string window_name = focusedWindow->Name;

          // match found
          if (file.name == window_name)
          {
            // cache an updated focused file variable
            // and the last focused file which doesn't get set to nullptr
            last_focused_file = focused_file = &file;
            break;
          }
        }
      }
    }
    
    // focused file shortcuts
    if (focused_file)
    {
      if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
      {
        if (Input::GetKeyDown(GLFW_KEY_S)) { SaveFile(*focused_file); }
        if (Input::GetKeyDown(GLFW_KEY_W)) { CloseFile(*focused_file); }
      }
    }

  }

  void CodeLayer::OnImGuiRender()
  {
    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    FLX_ASSERT(
      io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable,
      "ImGui Docking and/or Viewports are not enabled!"
    );

    // setup dockspace
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    #pragma region Main Menu Bar

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 12.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(18.0f, 12.0f));

    if (ImGui::BeginMainMenuBar())
    {
    
      if (ImGui::BeginMenu("File"))
      {
        //if (ImGui::MenuItem("New", "Ctrl+N")) {}
        if (ImGui::MenuItem("Save", "Ctrl+S")) { if (focused_file) SaveFile(*focused_file); }
        //if (ImGui::MenuItem("Reload", "Ctrl+R")) {}
        if (ImGui::MenuItem("Close File", "Ctrl+W")) { if (focused_file) CloseFile(*focused_file); }
        if (ImGui::MenuItem("Exit", "Ctrl+Q"))
        {
          // check if any files are still open
          // if any files are dirty, ask to save
          for (auto& file : files)
          {
            if (file.is_dirty)
            {
              window_popup_save_file = &file;
              ImGui::OpenPopup("Save Changes?");
              return;
            }
          }
          Application::Get().Close();
        }
        ImGui::EndMenu();
      }
    
      //if (ImGui::BeginMenu("Edit"))
      //{
      //  if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
      //  if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
      //  ImGui::EndMenu();
      //}
    
      ImGui::EndMainMenuBar();
    }

    //ImGui::PopStyleVar(2);

    #pragma endregion

    #pragma region Content Browser

    ImGui::Begin("Content Browser");

    if (ImGui::Button("Reset path"))
    {
      // pull filesystem from path
      base_path = std::filesystem::current_path();
    }

    ImGui::SeparatorText(std::string("Path: " + base_path.generic_string()).c_str());

    // if the base path has a parent path, display a button to go up a directory
    if (base_path.has_parent_path())
    {
      if (ImGui::Button("../"))
      {
        base_path = base_path.parent_path();
      }
    }

    // iterate through the current path and return a list of files
    for (const auto& entry : std::filesystem::directory_iterator(base_path))
    {
      // directory paths
      if (entry.is_directory())
      {
        if (ImGui::Button(entry.path().filename().generic_string().c_str()))
        {
          base_path = entry.path();
        }
      }
      // file paths
      else
      {
        if (ImGui::Button(entry.path().filename().generic_string().c_str()))
        {
          // get file if editable
          if (IsEditable(entry.path()))
          {
            // check if file is already open
            bool is_open = false;
            for (auto& file : files)
            {
              if (file.path == entry.path())
              {
                is_open = true;
                break;
              }
            }

            if (!is_open)
            {
              EditableFile file(entry.path());
              files.push_back(file);
            }
          }
        }
      }
    }

    ImGui::End();

    #pragma endregion

    #pragma region File Handler

    // if there are duplicate files open, append the relative path to the file name
    for (auto& file : files)
    {
      int count = 0;
      for (auto& other : files)
      {
        // ignore the same file
        if (file == other) continue;

        // use '|' because it is not a valid character in a file name
        //if (file.name.substr(0, file.name.find('|') - 1) == other.name.substr(0, file.name.find('|') - 1))
        if (file.name == other.name)
        {
          other.name += " | (" + other.path.parent_path().generic_string() + ')';
          other.to_dock = true;
          count++;
        }
      }
      if (count > 0)
      {
        file.name += " | (" + file.path.parent_path().generic_string() + ')';
        file.to_dock = true;
      }
    }
    
    // display all stored files
    // handle file closing, saving, and editing
    for (auto& file : files)
    {
      // handle file closing
      if (!file.is_open)
      {
        // if file is dirty, ask to save
        if (file.is_dirty)
        {
          window_popup_save_file = &file;
          ImGui::OpenPopup("Save Changes?");
        }
        // if file is not dirty, close the file
        else
        {
          files.erase(std::find(files.begin(), files.end(), file));
        }
      }

      // if file is an editable file, display the contents
      if (IsEditable(file.extension))
      {
        // begin window
        ImGui::Begin(file.name.c_str(), &file.is_open,
          (file.is_dirty ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags_None)
        );

        // if to_dock flag is set, dock it to the dockspace
        if (file.to_dock)
        {
          ImGui::DockBuilderDockWindow(file.name.c_str(), dockspace_id);
          ImGui::DockBuilderFinish(dockspace_id);
          file.to_dock = false;
        }

        // print the entire file as an editable text box
        // uses imgui_stdlib.h to allow for std::string input
        ImGui::InputTextMultiline("##source",
          &file.data,
          ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()),
          ImGuiInputTextFlags_AllowTabInput
        );

        // imgui if input text has been modified
        if (ImGui::IsItemEdited())
        {
          file.is_dirty = true;
        }

        ImGui::End();
      }
    }

    #pragma endregion

    #pragma region Saving Popup Modal

    // always center this window when appearing
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    // popup modal for saving files
    if (ImGui::BeginPopupModal("Save Changes?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
      // assert that the file is not null
      FLX_ASSERT(window_popup_save_file, "window_popup_save_file is null");

      ImGui::Text("Do you want to save changes to %s?", window_popup_save_file->name.c_str());

      // set the width of the buttons
      float width = 120.f;

      if (ImGui::Button("Save", ImVec2(width, 0)))
      {
        // save the file
        SaveFile(*window_popup_save_file);

        // close the file
        CloseFile(*window_popup_save_file);
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Don't Save", ImVec2(width, 0)))
      {
        // close the file
        window_popup_save_file->is_dirty = false; // must be set to false to avoid the save prompt
        CloseFile(*window_popup_save_file);
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(width, 0)))
      {
        // cancel the saving process
        #pragma warning(suppress: 6011) // dereferencing null pointer
        window_popup_save_file->is_open = true;
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    #pragma endregion
  }

}