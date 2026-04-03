#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/World/ComponentRegistry.hpp>
#include <ImGui/imgui.h>

#include "FlyCamComponent.hpp"
#include "ParticleSystem.hpp"
#include "PhysicsTestComponent.hpp"
#include "ForwardComponent.hpp"
#include "CharacterMovement.h"
#include "PlayerCamera.h"
#include "ObstacleComponent.h"
#include "TrapdoorComponent.h"
#include "ElectricalPanelBinaryComponent.h"
#include "ElectricalPanelStaticComponent.h"
#include "LethalComponent.h"


COMPONENT_MODULE_BEGIN()
    REGISTER_COMPONENT(FlyCamComponent, "Fly Cam Component")
    REGISTER_COMPONENT(ParticleSystemComponent, "Particle System")
    REGISTER_COMPONENT(PhysicsTestComponent, "Physics Test")
    REGISTER_COMPONENT(ForwardComponent,"Forward Component")
    REGISTER_COMPONENT(CharacterMovement, "Character Movement")
    REGISTER_COMPONENT(PlayerCamera,"Player Camera")
    REGISTER_COMPONENT(ObstacleComponent,"Obstacle Component")
    REGISTER_COMPONENT(TrapdoorComponent,"Trapdoor Component")
    REGISTER_COMPONENT(ElectricalPanelBinaryComponent,"Electrical Panel Binary Component")
    REGISTER_COMPONENT(ElectricalPanelStaticComponent,"Electrical Panel Static Component")
    REGISTER_COMPONENT(LethalComponent,"Lethal Component")
COMPONENT_MODULE_END()

TERMINA_DLL_EXPORT void SetImGuiContext(void* ctx, void* allocFunc, void* freeFunc, void* userData)
{
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
    ImGui::SetAllocatorFunctions(
        reinterpret_cast<ImGuiMemAllocFunc>(allocFunc),
        reinterpret_cast<ImGuiMemFreeFunc>(freeFunc),
        userData);
}
