#pragma once
#include <vector>
#include <SDL.h>

struct PacManSource
{
	std::pair<int, SDL_FRect>* _pacMan{nullptr};
	int* _directionD{nullptr};
	int& _directionS;
	float& _pacManKill;
	PacManSource(float& pacManKill, int& directionS) noexcept :_pacManKill{ pacManKill }, _directionS{directionS} {}
};