#include "LethalComponent.h"
#include <Termina/World/Components/Transform.hpp>

void LethalComponent::OnCollisionEnter(Termina::Actor* other)
{
	// On vérifie que c'est bien une entité qui peut se déplacer (le joueur)
	if (other->HasComponent<Termina::Transform>()) {
		
		// Ici, tu peux mettre la logique de mort de ton jeu.
		// Pour l'instant, on va juste réinitialiser sa position au début (0, 0, 0)
		other->GetComponent<Termina::Transform>().SetPosition(glm::vec3(0, 0, 0));
		
		TN_DEBUG("Le joueur a touché un obstacle mortel ! Retour a la case depart.");
	}
}