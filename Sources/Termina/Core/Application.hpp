#pragma once

#include "SystemManager.hpp"
#include "Window.hpp"

namespace Termina {
    /// Represents the main application instance.
    class Application
    {
    public:
        Application(const std::string& gameName);
        virtual ~Application();

        /// Runs the application loop.
        void Run();
        void Close() { m_Running = false; }

        static Application& Get();

        template <typename T>
        static T* GetSystem()
        {
            return Get().m_SystemManager.GetSystem<T>();
        }

        Window* GetWindow() { return m_Window; }
    private:
        void PreUpdate(float dt);
        void Update(float dt);
        void PostUpdate(float dt);
        void PrePhysics(float dt);
        void Physics(float dt);
        void PostPhysics(float dt);
        void PreRender(float dt);
        void Render(float dt);
        void PostRender(float dt);

        static Application* s_Instance;

    protected:
        virtual void OnPreUpdate(float dt) {}
        virtual void OnUpdate(float dt) {}
        virtual void OnPostUpdate(float dt) {}
        virtual void OnPrePhysics(float dt) {}
        virtual void OnPhysics(float dt) {}
        virtual void OnPostPhysics(float dt) {}
        virtual void OnPreRender(float dt) {}
        virtual void OnRender(float dt) {}
        virtual void OnPostRender(float dt) {}

        SystemManager m_SystemManager;
        Window* m_Window;

        float m_LastFrameTime = 0.0f;
        bool m_Running = true;
    };
}
