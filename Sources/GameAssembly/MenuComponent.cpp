#include "MenuComponent.h"
#include <ImGui/imgui.h>
#include <Termina/Core/Application.hpp>
#include <Termina/World/World.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp> // <-- AJOUT IMPORTANT

void MenuComponent::OnPlay()
{
    Termina::World* world = m_Owner->GetParentWorld();
    if (world) {
        // Obtenir les acteurs
        Termina::Actor* menuCamActor = world->GetActorByName(m_menuCameraName);
        Termina::Actor* playerActor = world->GetActorByName(m_playerCameraName);

        // Si le joueur existe, on désactive sa caméra (enlève le flag Primary)
        if (playerActor && playerActor->HasComponent<Termina::CameraComponent>()) {
            playerActor->GetComponent<Termina::CameraComponent>().SetPrimary(false);
        }

        // Si la caméra du menu existe, on l'active en Primary et on la fixe dans le World
        if (menuCamActor && menuCamActor->HasComponent<Termina::CameraComponent>()) {
            menuCamActor->GetComponent<Termina::CameraComponent>().SetPrimary(true);
            world->SetMainCamera(menuCamActor);
            TN_DEBUG("Caméra du Menu activée !");
        } else {
            TN_ERROR("Impossible de trouver la caméra Menu : %s", m_menuCameraName);
        }
    }
}

void MenuComponent::Update(float deltaTime)
{
	if (!m_isMenuOpen) return;

	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300, 200)); 
	
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::Begin("Menu Principal", nullptr, windowFlags);
	ImGui::Text("Bienvenue dans le jeu !");
	ImGui::Separator();

	if (ImGui::Button("Lancer", ImVec2(280, 50))) {
		TN_DEBUG("Menu: Bouton Lancer clique !");
		m_isMenuOpen = false; // Ferme le menu

		Termina::World* world = m_Owner->GetParentWorld();
		if (world) {
			Termina::Actor* menuCamActor = world->GetActorByName(m_menuCameraName);
			Termina::Actor* playerActor = world->GetActorByName(m_playerCameraName);

			// On désactive la caméra du menu
			if (menuCamActor && menuCamActor->HasComponent<Termina::CameraComponent>()) {
				menuCamActor->GetComponent<Termina::CameraComponent>().SetPrimary(false);
			}

			// On active la caméra du joueur
			if (playerActor && playerActor->HasComponent<Termina::CameraComponent>()) {
				playerActor->GetComponent<Termina::CameraComponent>().SetPrimary(true);
				world->SetMainCamera(playerActor); 
				TN_DEBUG("Caméra Joueur activée !");
			} else {
				TN_ERROR("Impossible de trouver la camera nommée : %s", m_playerCameraName);
			}
		}
	}

	ImGui::Spacing();

	if (ImGui::Button("Quitter", ImVec2(280, 50))) {
		TN_DEBUG("Menu: Fermeture du jeu demandee.");
		Termina::Application::Get().Close();
	}

	ImGui::End();
}

void MenuComponent::Inspect()
{
    ImGui::InputText("Nom Camera Menu", m_menuCameraName, IM_ARRAYSIZE(m_menuCameraName));
    ImGui::InputText("Nom Camera Joueur", m_playerCameraName, IM_ARRAYSIZE(m_playerCameraName));
}

void MenuComponent::Serialize(nlohmann::json& out) const
{
	out["Nom Cam Menu"] = m_menuCameraName;
	out["Nom Cam Joueur"] = m_playerCameraName;
}

void MenuComponent::Deserialize(const nlohmann::json& in)
{
	if (in.contains("Nom Cam Menu")) {
		std::string temp = in["Nom Cam Menu"];
		strncpy_s(m_menuCameraName, sizeof(m_menuCameraName), temp.c_str(), _TRUNCATE);
	}

	if (in.contains("Nom Cam Joueur")) {
		std::string temp = in["Nom Cam Joueur"];
		strncpy_s(m_playerCameraName, sizeof(m_playerCameraName), temp.c_str(), _TRUNCATE);
	}
}