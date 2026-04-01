#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class ObstacleComponent : public TerminaScript::ScriptableComponent
{
public:

	ObstacleComponent() = default;
	ObstacleComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void OnPlay() override; // Appelé au début du jeu pour initialiser la hauteur aléatoire
	void Inspect() override;
	void Serialize(nlohmann::json& out) const;
	void Deserialize(const nlohmann::json& in);
	void OnCollisionEnter(Termina::Actor* other) override;

private:
	// Paramètres éditables depuis l'interface
	float m_gapSize = 3.0f;       // Taille du trou au centre
	float m_minHeight = -2.0f;    // Décalage minimum du trou sur l'axe Y (hauteur)
	float m_maxHeight = 3.0f;     // Décalage maximum du trou
	float m_distanceToNext = 15.0f; // Distance à laquelle faire apparaître le prochain obstacle sur l'axe X (ou Z selon ton jeu)

	bool m_hasPassed = false;     // Sécurité pour ne pas déclencher la génération 100 fois
};