#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>

class MenuComponent : public TerminaScript::ScriptableComponent {
public:
    MenuComponent() = default;
    MenuComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    static MenuComponent* Instance();
    void Start() override;
    void OpenMenu();

    void OnPlay() override; // Ajout pour initialiser la caméra au lancement !
    void Update(float deltaTime) override;
    void Inspect() override;

    void Serialize(nlohmann::json& out) const;

    void Deserialize(const nlohmann::json& in);

private:
    bool m_isMenuOpen = true;
    static MenuComponent* instance;

    // Le nom de l'acteur qui possède la caméra de jeu
    char m_playerCameraName[256] = "PlayerCamera"; 
    
    // Le nom de l'acteur qui possède la caméra du Menu (à créer dans ta scène)
    char m_menuCameraName[256] = "MenuCamera"; 
};