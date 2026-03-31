#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class Score
{
public:
	void Update(float dt);

	void AddScore(float value);
	float GetScore() const;

private:

	float playerScore = 0.f;
};