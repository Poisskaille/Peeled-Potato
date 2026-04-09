#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/World/ComponentRegistry.hpp>
#include <ImGui/imgui.h>

#include "FlyCamComponent.hpp"
#include "ParticleSystem.hpp"
#include "PhysicsTestComponent.hpp"
#include "RotateComponent.hpp"
#include "CharacterMovement.hpp"
#include "PlayerCamera.hpp"
#include "GameManager.hpp"
#include "ObstacleComponent.hpp"
#include "ProceduralGenerator.hpp"
#include "RandomComponent.hpp"
#include "MenuComponent.h"

COMPONENT_MODULE_BEGIN()
    REGISTER_COMPONENT(FlyCamComponent, "Fly Cam Component")
    REGISTER_COMPONENT(ParticleSystemComponent, "Particle System")
    REGISTER_COMPONENT(PhysicsTestComponent, "Physics Test")
    REGISTER_COMPONENT(RotateComponent,"Rotate Component")
    REGISTER_COMPONENT(CharacterMovement, "Character Movement")
    REGISTER_COMPONENT(PlayerCamera,"Player Camera")
    REGISTER_COMPONENT(GameManager, "GameManager")
    REGISTER_COMPONENT(ObstacleComponent, "Obstacle Component")
	REGISTER_COMPONENT(ProceduralGenerator, "Procedural Generator")
	REGISTER_COMPONENT(MenuComponent, "Menu Component")
    REGISTER_COMPONENT(RandomComponent, "Random Component")
COMPONENT_MODULE_END()

TERMINA_DLL_EXPORT void SetImGuiContext(void* ctx, void* allocFunc, void* freeFunc, void* userData)
{
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
    ImGui::SetAllocatorFunctions(
        reinterpret_cast<ImGuiMemAllocFunc>(allocFunc),
        reinterpret_cast<ImGuiMemFreeFunc>(freeFunc),
        userData);
}
