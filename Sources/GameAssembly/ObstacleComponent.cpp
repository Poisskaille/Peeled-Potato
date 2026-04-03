#include "ObstacleComponent.h"

#include <ImGui/imgui.h>
#include <cstdlib> // Pour rand()


void ObstacleComponent::OnPlay()
{
	// Générer une hauteur de trou aléatoire entre m_minHeight et m_maxHeight
	float randomHeightOffset = m_minHeight + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (m_maxHeight - m_minHeight)));

	// On récupère les enfants de l'acteur (Le pilier Haut et Bas)
	auto& children = m_Owner->GetChildren();
	if (children.size() >= 2) {
		Termina::Actor* bottomPillar = children[0];
		Termina::Actor* topPillar = children[1];

		// On décale la position locale des piliers pour créer le trou
		if (bottomPillar->HasComponent<Termina::Transform>() && topPillar->HasComponent<Termina::Transform>()) {
			auto& bottomTransform = bottomPillar->GetComponent<Termina::Transform>();
			auto& topTransform = topPillar->GetComponent<Termina::Transform>();

			// On écarte les piliers à partir du centre de l'obstacle, et on applique la hauteur aléatoire
			// (Ici on suppose que les piliers font 10 de hauteur par exemple, ajuste avec ton propre scale)
			glm::vec3 bottomPos = bottomTransform.GetLocalPosition();
			bottomPos.y = randomHeightOffset - (m_gapSize / 2.0f) - 5.0f; // Ajuste le -5.0f (la moitié de la hauteur de ton bloc)
			bottomTransform.SetLocalPosition(bottomPos);

			glm::vec3 topPos = topTransform.GetLocalPosition();
			topPos.y = randomHeightOffset + (m_gapSize / 2.0f) + 5.0f; // Ajuste le +5.0f
			topTransform.SetLocalPosition(topPos);
		}
	}
	else {
		TN_ERROR("Le Flappy Obstacle a besoin d'au moins 2 enfants (Pilier bas et Pilier haut) !");
	}
}

void ObstacleComponent::Inspect()
{
	ImGui::Text("Flappy Obstacle Settings");
	ImGui::DragFloat("Gap Size", &m_gapSize, 0.1f, 1.0f, 10.0f);
	ImGui::DragFloat("Min Height", &m_minHeight, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("Max Height", &m_maxHeight, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("Distance Next", &m_distanceToNext, 0.5f, 5.0f, 50.0f);
}

void ObstacleComponent::Serialize(nlohmann::json& out) const
{
	out["Gap Size"] = m_gapSize;
	out["Min Height"] = m_minHeight;
	out["Max Height"] = m_maxHeight;
	out["Distance Next"] = m_distanceToNext;
}

void ObstacleComponent::Deserialize(const nlohmann::json& in)
{
	if (in.contains("Gap Size")) m_gapSize = in["Gap Size"];
	if (in.contains("Min Height")) m_minHeight = in["Min Height"];
	if (in.contains("Max Height")) m_maxHeight = in["Max Height"];
	if (in.contains("Distance Next")) m_distanceToNext = in["Distance Next"];
}

void ObstacleComponent::OnCollisionEnter(Termina::Actor* other)
{
	// Si le collider (qui représente faire le point en passant dans le trou) touche un joueur...
	if (!m_hasPassed && other->HasComponent<Termina::Transform>()) { // Remplace ce IF par une vérifiaction de Tag "Player" si possible
		m_hasPassed = true;
		TN_DEBUG("Point gagné ! Joueur a passé l'obstacle.");
		TN_INFO("Point gagné ! Joueur a passé l'obstacle.");
		TN_ERROR("Point gagné ! Joueur a passé l'obstacle.");
	
		// --- FAIRE APPARAITRE LE PROCHAIN OBSTACLE ---
		
		Termina::World* world = m_Owner->GetParentWorld();
		
		// Clone de l'obstacle actuel
		Termina::Actor* nextObstacle = world->SpawnActorFrom(m_Owner);
		
		// On lui donne une nouvelle position devant celui-ci
		auto& currentTransform = m_Owner->GetComponent<Termina::Transform>();
		auto& nextTransform = nextObstacle->GetComponent<Termina::Transform>();

		glm::vec3 nextPos = currentTransform.GetPosition();
		
		// On ajoute de la distance. Si ton perso avance sur X, on ajoute en X. 
		// Modifie l'axe en fonction de la direction de jeu.
		nextPos.x += m_distanceToNext; 
		
		nextTransform.SetPosition(nextPos);

		// Le nouveau obstacle recalculera sa propre hauteur aléatoire dans son OnPlay() !
	}
}


