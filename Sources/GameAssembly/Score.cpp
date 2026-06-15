#include "Score.hpp"

void Score::Update(float dt)
{
	AddScore(dt);
}

void Score::AddScore(float value) { playerScore += value; }
float Score::GetScore() const { return playerScore; }
