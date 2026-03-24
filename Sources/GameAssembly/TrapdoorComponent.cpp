#include "TrapdoorComponent.h"

#include <ImGui/imgui.h>



void TrapdoorComponent::OnCollisionEnter(Termina::Actor* other)
{

	if (other->HasComponent<Termina::Transform>()) {
		other->GetComponent <Termina::Transform>().SetPosition(glm::vec3(0, 0, 0));
		TN_DEBUG("Trapdoor Toucher");
	}
}


