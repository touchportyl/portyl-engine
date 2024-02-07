#include "layer_code.h"

#include <iostream>
#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>

#include "FlexEngine/Loader/shaders.h"

namespace FlexEngine
{
  struct File
  {
    std::filesystem::path path;
    std::string name;
    std::string extension;
    std::string data;

    File(const std::filesystem::path& path)
      : path(path), name(path.filename().string()), extension(path.extension().string())
    {
      // automatically read the file
      std::ifstream file(path);
      std::string _data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      data = _data;
    }
  };

  std::filesystem::path file_path; // todo: change to a vector of file paths

  std::filesystem::path base_path = std::filesystem::current_path();

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

    if (glfwGetKey(Application::Get().GetGLFWWindow(), GLFW_KEY_R))
    {
      // setup dock node
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

      // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
      //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
      //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
      auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.35f, nullptr, &dockspace_id);
      auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

      // we now dock our windows into the docking node we made above
      ImGui::DockBuilderDockWindow("Content Browser", dock_id_left);
      ImGui::DockBuilderDockWindow("Down", dock_id_down);
      ImGui::DockBuilderFinish(dockspace_id);
    }

    #pragma region Main Menu Bar

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 12.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(18.0f, 12.0f));

    if (ImGui::BeginMainMenuBar())
    {

      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New", "Ctrl+N")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {}
        if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Get().Close(); }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit"))
      {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View"))
      {
        if (ImGui::MenuItem("Viewport", "Ctrl+1")) {}
        if (ImGui::MenuItem("Content Browser", "Ctrl+2")) {}
        if (ImGui::MenuItem("Down", "Ctrl+3")) {}
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    //ImGui::PopStyleVar(2);

    #pragma endregion


    // create imgui windows
    #pragma region Content Browser

    ImGui::Begin("Content Browser");

    static std::filesystem::path base_path = std::filesystem::current_path();

    if (ImGui::Button("Reload"))
    {
      // pull filesystem from path
      base_path = std::filesystem::current_path();
    }

    ImGui::Separator();

    // display the current path
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
      // if the path is a directory, display it as a button
      if (entry.is_directory())
      {
        if (ImGui::Button(entry.path().filename().generic_string().c_str()))
        {
          base_path = entry.path();
        }
      }
      else
      {
        if (ImGui::Button(entry.path().filename().generic_string().c_str()))
        {
          // get file
          file_path = entry.path();
          //std::ifstream file(file_path);
          //std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        }
      }
    }

    ImGui::End();

    #pragma endregion

    // if file is a .txt file, display the contents
    if (file_path.extension() == ".md")
    {
      ImGui::Begin(file_path.filename().string().c_str());

      // simply read file
      std::ifstream file(file_path);

      std::string line;
      while (std::getline(file, line))
      {
        ImGui::Text(line.c_str());
      }

      // print the entire file as an editable text box
      //ImGui::InputTextMultiline("##source",
      //  &file_contents[0], file_contents.size(),
      //  ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), ImGuiInputTextFlags_AllowTabInput);

      ImGui::End();
    }

  }

}