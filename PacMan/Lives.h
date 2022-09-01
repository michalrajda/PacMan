#pragma once
#include "TextureMenager.h"
#include "Flags.h"


struct LiveSize
{
	SDL_Rect _source;
	SDL_Rect _dest;
};

class Live
{
private:
	LiveSize _lives;
	SDL_Texture* livesT;
public:
	Live():_lives{ { 0, 0, 128, 32 },{ 0,0,128,32 } }, livesT{ nullptr }{};
	void loadTexture(SDL_Renderer* render) noexcept;
	void resetLives() noexcept;
	void draw(SDL_Renderer* render, Flags& _isLives) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void destroyTexture() noexcept;
	LiveSize& getLives() noexcept;
};

inline void Live::loadTexture(SDL_Renderer* render) noexcept
{
	livesT = TextureMenager::loadTexture(render, "assets/image/lives.png");
}

inline void Live::resetLives() noexcept
{
	_lives = { { 0,0,128,32 },{ 0,0,128,32 } };
}

inline void Live::draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	TextureMenager::drawTextureOnRenderTexture(render, livesT, destTexture, &_lives._source, &_lives._dest);
}

inline void Live::destroyTexture() noexcept
{
	SDL_DestroyTexture(livesT);
}

inline LiveSize& Live::getLives() noexcept
{
	return _lives;
}
