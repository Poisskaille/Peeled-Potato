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

	// --- 1. TAILLE DE LA FENETRE ---
	// Modifie ces valeurs pour changer la taille globale de ton menu
	float menuWidth = 600.0f;
	float menuHeight = 400.0f;

	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	// On dit à ImGui : Positionne le centre de la fenêtre exactement au centre de l'écran (0.5, 0.5)
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	// On applique nos nouvelles dimensions
	ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight)); 
	
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::Begin("Menu Principal", nullptr, windowFlags);
	
	// --- 2. ESPACEMENT & CENTRAGE ---
	// On descend un peu le texte pour ne pas le coller au bord
	ImGui::SetCursorPosY(50.0f);
	
	// Titre
	const char* titleText = "Bienvenue dans le jeu !";
	// (Optionnel) Calcul pour centrer le texte : 
	float textWidth = ImGui::CalcTextSize(titleText).x;
	ImGui::SetCursorPosX((menuWidth - textWidth) * 0.5f);
	ImGui::Text(titleText);
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Variables pour la taille de nos boutons (plus grands aussi !)
	float btnWidth = 400.0f;
	float btnHeight = 60.0f;
	// Calcul pour centrer les boutons horizontalement
	float btnPosX = (menuWidth - btnWidth) * 0.5f;

	// On descend la position Y pour espacer le premier bouton
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.0f);

	// Bouton "Lancer"
	ImGui::SetCursorPosX(btnPosX); // Centre le bouton
	if (ImGui::Button("Lancer", ImVec2(btnWidth, btnHeight))) {
		TN_DEBUG("Menu: Bouton Lancer clique !");
		m_isMenuOpen = false; // Ferme le menu

		// --- CHANGEMENT DE CAMERA VERS LE JOUEUR ---
		Termina::World* world = m_Owner->GetParentWorld();
		if (world) {
			Termina::Actor* menuCamActor = world->GetActorByName(m_menuCameraName);
			Termina::Actor* playerActor = world->GetActorByName(m_playerCameraName);

			if (menuCamActor && menuCamActor->HasComponent<Termina::CameraComponent>()) {
				menuCamActor->GetComponent<Termina::CameraComponent>().SetPrimary(false);
			}

			if (playerActor && playerActor->HasComponent<Termina::CameraComponent>()) {
				playerActor->GetComponent<Termina::CameraComponent>().SetPrimary(true);
				world->SetMainCamera(playerActor); 
				TN_DEBUG("Caméra Joueur activée !");
			} else {
				TN_ERROR("Impossible de trouver la camera nommée : %s", m_playerCameraName);
			}
		}
	}

	// On redescend la position Y pour espacer le deuxième bouton
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

	// Bouton "Quitter"
	ImGui::SetCursorPosX(btnPosX); // Centre le bouton
	if (ImGui::Button("Quitter", ImVec2(btnWidth, btnHeight))) {
		TN_DEBUG("Menu: Fermeture du jeu demandee.");
		Termina::Application::Get().Close(); // Quitte l'application
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