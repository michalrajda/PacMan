#include "Lives.h"

void Live::draw(SDL_Renderer* render, Flags& _islives) noexcept
{
	if (_lives._dest.w <= 0)
	{
		_islives._nextLevel = false;
		_islives._actLevel = false;
		_islives._isLives = false;
		_islives._reset = true;
		return;
	}
	TextureMenager::drawTexture(render, livesT, &_lives._source, &_lives._dest);
}
