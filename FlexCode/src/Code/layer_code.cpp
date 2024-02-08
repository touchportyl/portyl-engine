#include "layer_code.h"

#include <iostream>
#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/imgui_stdlib.h>

#include "FlexEngine/Loader/shaders.h"

namespace FlexEngine
{
  struct File
  {
    std::filesystem::path path{};
    std::string name{};
    std::string extension{};
    std::string data{};
    bool is_open{ true };  // flag for closing the file
    bool is_dirty{ false }; // flag for checking if the file has been modified
    bool to_dock{ true };  // flag for docking the window

    // overload the == operator to compare files
    bool operator==(const File& other)
    {
      return path == other.path;
    }
    bool operator!=(const File& other)
    {
      return path != other.path;
    }

    File(const std::filesystem::path& path)
      : path(path), name(path.filename().string()), extension(path.extension().string())
    {
      // automatically read the file
      std::ifstream ifs(path);
      if (!ifs)
      {
        std::cerr << "Error: Could not open file " << path << std::endl;
        abort();
      }

      std::string _data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      data = _data;
    }
  };

  std::vector<File> files;

  std::filesystem::path base_path = std::filesystem::current_path();

  File* window_popup_save_file;

  bool IsEditable(const std::string& extension)
  {
    bool flag = false;
    std::vector<std::string> common_extensions = { ".txt", ".md", ".h", ".c", ".hpp", ".cpp", ".ini" };

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
  bool IsEditable(const std::filesystem::path path)
  {
    return IsEditable(path.extension().string());
  }

  CodeLayer::CodeLayer()
    : Layer("Flex Code")
  {
  }

  void CodeLayer::OnAttach()
  {
  }

  void CodeLayer::OnDetach()
  {
  }

  void CodeLayer::OnUpdate()
  {
  }

  void CodeLayer::OnImGuiRender()
  {
    // https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp

    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    if (!(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable))
    {
      std::cerr << "ImGui Docking and/or Viewports are not enabled!" << std::endl;
      abort();
    }

    // setup dockspace
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    //if (glfwGetKey(Application::Get().GetGLFWWindow(), GLFW_KEY_R))
    //{
    //  // setup dock node
    //  ImGui::DockBuilderRemoveNode(dockspace_id);
    //  ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    //  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
    //
    //  // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
    //  //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
    //  //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
    //  auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
    //  auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
    //
    //  // we now dock our windows into the docking node we made above
    //  ImGui::DockBuilderDockWindow("Content Browser", dock_id_left);
    //  ImGui::DockBuilderDockWindow("Down", dock_id_down);
    //  ImGui::DockBuilderFinish(dockspace_id);
    //}

    #pragma region Main Menu Bar

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 12.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(18.0f, 12.0f));

    if (ImGui::BeginMainMenuBar())
    {
    
      if (ImGui::BeginMenu("File"))
      {
        //if (ImGui::MenuItem("New", "Ctrl+N")) {}
        //if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        //if (ImGui::MenuItem("Reload", "Ctrl+R")) {}
        if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Get().Close(); }
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

    ImGui::Separator();

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
              File file(entry.path());
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
      assert(window_popup_save_file && "window_popup_save_file is null");

      ImGui::Text("Do you want to save changes to %s?", window_popup_save_file->name.c_str());
      if (ImGui::Button("Save", ImVec2(80, 0)))
      {
        // save the file
        std::ofstream out(window_popup_save_file->path);
        if (!out)
        {
          std::cerr << "Error: Could not open file " << window_popup_save_file->path << std::endl;
          abort();
        }
        out << window_popup_save_file->data;

        // close the file
        window_popup_save_file->is_dirty = false;
        window_popup_save_file->is_open = false;
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Don't Save", ImVec2(80, 0)))
      {
        // close the file
        window_popup_save_file->is_dirty = false;
        window_popup_save_file->is_open = false;
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(80, 0)))
      {
        // cancel the saving process
        window_popup_save_file->is_open = true;
        window_popup_save_file = nullptr;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    #pragma endregion
  }

}