#pragma once
#include <SDL.h>
struct TunelSize
{
	SDL_FRect _left;
	SDL_FRect _right;
	TunelSize() :_left{ 0.0f,0.0f,32.0f,32.0f }, _right{ 0.0f,0.0f,32.0f,32.0f }{}
};