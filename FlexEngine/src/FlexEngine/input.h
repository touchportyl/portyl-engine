#pragma once

#include <array>
#include <algorithm>

#include <GLFW/glfw3.h>

#include "glm.hpp"

// GLFW key codes
// https://www.glfw.org/docs/3.3/group__keys.html

namespace FlexEngine
{
  class Input
  {
  // public functions
  public:
    static bool GetKeyDown(int key)             { return m_key_down[key]; }
    static bool GetKeyUp(int key)               { return m_key_up[key]; }
    static bool GetKey(int key)                 { return m_key[key]; }
    static bool AnyKeyDown()                    { return std::any_of(m_key_down.begin(), m_key_down.end(), [](bool b) { return b; }); }
    static bool AnyKeyUp()                      { return std::any_of(m_key_up.begin(), m_key_up.end(), [](bool b) { return b; }); }
    static bool AnyKey()                        { return std::any_of(m_key.begin(), m_key.end(), [](bool b) { return b; }); }

    static glm::vec2 GetCursorPosition()        { return m_cursor_position; }
    static glm::vec2 GetCursorPositionDelta()   { return m_cursor_position_delta; }

    // renaming for convenience
    static glm::vec2 GetMousePosition()         { return GetCursorPosition(); }
    static glm::vec2 GetMousePositionDelta()    { return GetCursorPositionDelta(); }

    static bool GetMouseButtonDown(int button)  { return m_mouse_button_down[button]; }
    static bool GetMouseButtonUp(int button)    { return m_mouse_button_up[button]; }
    static bool GetMouseButton(int button)      { return m_mouse_button[button]; }
    static bool AnyMouseButtonDown()            { return std::any_of(m_mouse_button_down.begin(), m_mouse_button_down.end(), [](bool b) { return b; }); }
    static bool AnyMouseButtonUp()              { return std::any_of(m_mouse_button_up.begin(), m_mouse_button_up.end(), [](bool b) { return b; }); }
    static bool AnyMouseButton()                { return std::any_of(m_mouse_button.begin(), m_mouse_button.end(), [](bool b) { return b; }); }

  // member variables
  private:
    static std::array<bool, GLFW_KEY_LAST>          m_key_down;
    static std::array<bool, GLFW_KEY_LAST>          m_key_up;
    static std::array<bool, GLFW_KEY_LAST>          m_key;

    static glm::vec2                                m_cursor_position;
    static glm::vec2                                m_last_cursor_position;
    static glm::vec2                                m_cursor_position_delta;

    static std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_mouse_button_down;
    static std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_mouse_button_up;
    static std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_mouse_button;

    static glm::vec2                                m_scroll_offset;

  // callback functions for glfw
  public:
    static void KeyCallback             (GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CursorPositionCallback  (GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback     (GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback          (GLFWwindow* window, double xoffset, double yoffset);

  // prevent instantiation
  public:
    Input() = delete;
    ~Input() = delete;
  };
}
