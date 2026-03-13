#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/World/ComponentRegistry.hpp>

#include "MySimpleComponent.hpp"
#include "FlyCamComponent.hpp"

COMPONENT_MODULE_BEGIN()
    REGISTER_COMPONENT(MySimpleComponent, "MySimpleComponent")
    REGISTER_COMPONENT(FlyCamComponent, "FlyCamComponent")
COMPONENT_MODULE_END()
