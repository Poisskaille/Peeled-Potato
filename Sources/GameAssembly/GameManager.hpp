#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "Score.hpp"

using namespace TerminaScript;

class GameManager : public TerminaScript::ScriptableComponent
{
public:

	GameManager() = default;
	GameManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Start()override;
	void Update(float dt)override;

	void NewGame();

	GameManager* Instance();

private:
	Score* _score;
	GameManager* instance;
};