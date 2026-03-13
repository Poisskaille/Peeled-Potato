#pragma once

#include <Termina/Core/System.hpp>
#include "InputCodes.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <GLM/glm.hpp>

struct GLFWwindow;

namespace Termina {

    /// Represents a binding for an action (key, mouse button, or gamepad button).
    struct ActionBinding {
        enum class Type { Key, MouseButton, GamepadButton } BindingType;
        Key          KeyCode    = Key::Space;
        MouseButton  MouseBtn   = MouseButton::Left;
        GamepadButton GamepadBtn = GamepadButton::A;
        int32        GamepadId  = 0;
    };

    /// Represents a binding for an axis (key, mouse delta, scroll, or gamepad axis).
    struct AxisBinding {
        enum class Type { Key, MouseDeltaX, MouseDeltaY, ScrollX, ScrollY, GamepadAxis } BindingType;
        Key          KeyCode         = Key::Space;
        GamepadAxis  GamepadAxisCode = GamepadAxis::LeftX;
        int32        GamepadId       = 0;
        float        Scale           = 1.0f;
    };

    /// Represents the input system, handling keyboard, mouse, and gamepad input.
    class InputSystem : public ISystem
    {
    public:
        InputSystem(GLFWwindow* window);
        ~InputSystem();

        /// Called before the update phase, used for input polling.
        void PreUpdate(float deltaTime) override;
        /// Called after the update phase, used for input processing.
        void PostUpdate(float deltaTime) override;

        static bool     IsKeyPressed(Key key);
        static bool     IsKeyHeld(Key key);
        static bool     IsKeyReleased(Key key);

        static bool     IsMouseButtonPressed(MouseButton button);
        static bool     IsMouseButtonHeld(MouseButton button);
        static bool     IsMouseButtonReleased(MouseButton button);
        static glm::vec2 GetMousePosition();
        static glm::vec2 GetMouseDelta();
        static glm::vec2 GetScrollDelta();

        static void     SetCursorVisible(bool visible);
        static void     SetCursorLocked(bool locked);

        static bool     IsGamepadConnected(int32 gamepadId);
        static bool     IsGamepadButtonPressed(int32 gamepadId, GamepadButton button);
        static bool     IsGamepadButtonHeld(int32 gamepadId, GamepadButton button);
        static bool     IsGamepadButtonReleased(int32 gamepadId, GamepadButton button);
        static float    GetGamepadAxis(int32 gamepadId, GamepadAxis axis, float deadzone = 0.1f);

        /// Maps an action to a set of bindings (key, mouse button, or gamepad button).
        void MapAction(const std::string& name, std::vector<ActionBinding> bindings);
        /// Maps an axis to a set of bindings (key, mouse delta, scroll, or gamepad axis).
        void MapAxis(const std::string& name, std::vector<AxisBinding> bindings);
        /// Unmaps an action by name.
        void UnmapAction(const std::string& name);
        /// Unmaps an axis by name.
        void UnmapAxis(const std::string& name);

        /// Returns true if the action is currently pressed.
        static bool     IsActionPressed(const std::string& name);
        /// Returns true if the action is currently held.
        static bool     IsActionHeld(const std::string& name);
        /// Returns true if the action is currently released.
        static bool     IsActionReleased(const std::string& name);
        /// Returns the value of the axis (0.0 to 1.0).
        static float    GetAxis(const std::string& name);

        using KeyCallback         = std::function<void(Key, bool)>;
        using MouseButtonCallback = std::function<void(MouseButton, bool)>;
        void AddKeyCallback(KeyCallback callback);
        void AddMouseButtonCallback(MouseButtonCallback callback);

        UpdateFlags GetUpdateFlags() const override { return UpdateFlags::UpdateDuringEditor; }
        std::string GetName()        const override { return "Input System"; }
        int         GetPriority()    const override { return 10; }

        /// Shows the debug window for input system state.
        static void         ShowDebugWindow(bool* open = nullptr);

        static InputSystem* Get();

    private:
        static void KeyCallbackGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallbackGLFW(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallbackGLFW(GLFWwindow* window, double xpos, double ypos);
        static void ScrollCallbackGLFW(GLFWwindow* window, double xoffset, double yoffset);

        void PollGamepads();

        static constexpr int32 MaxKeys           = 512;
        static constexpr int32 MaxMouseButtons   = 8;
        static constexpr int32 MaxGamepads       = 4;
        static constexpr int32 MaxGamepadButtons = 15;
        static constexpr int32 MaxGamepadAxes    = 6;

        GLFWwindow* m_Window = nullptr;

        bool m_KeyCurrent[MaxKeys]         = {};
        bool m_KeyPrevious[MaxKeys]        = {};
        bool m_MouseButtonCurrent[MaxMouseButtons]  = {};
        bool m_MouseButtonPrevious[MaxMouseButtons] = {};

        double m_MouseX = 0.0, m_MouseY = 0.0;
        double m_PrevMouseX = 0.0, m_PrevMouseY = 0.0;
        double m_MouseDeltaX = 0.0, m_MouseDeltaY = 0.0;
        double m_ScrollX = 0.0, m_ScrollY = 0.0;
        double m_ScrollAccumX = 0.0, m_ScrollAccumY = 0.0;
        bool   m_FirstMouse = true;

        bool  m_GamepadButtonCurrent[MaxGamepads][MaxGamepadButtons]  = {};
        bool  m_GamepadButtonPrevious[MaxGamepads][MaxGamepadButtons] = {};
        float m_GamepadAxes[MaxGamepads][MaxGamepadAxes]              = {};

        std::unordered_map<std::string, std::vector<ActionBinding>> m_ActionMap;
        std::unordered_map<std::string, std::vector<AxisBinding>>   m_AxisMap;

        std::vector<KeyCallback>         m_KeyCallbacks;
        std::vector<MouseButtonCallback> m_MouseButtonCallbacks;

        static InputSystem* s_Instance;
    };

}
