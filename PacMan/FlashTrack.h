#pragma once
#include "TextureMenager.h"

struct FlashSize
{
	SDL_Rect _source;
	SDL_Rect _dest;
};

struct FlashPoint
{
	const float _dvTexture;
	float _dv[2];
	float dxy[2];
	float _dxTexture;
	FlashPoint() noexcept :_dvTexture{ 8.0f }, _dv{ 0.0f, 0.0f }, dxy{ 0.0f, 0.0f }, _dxTexture{ 0.0f }{}
};

struct MoveFlashSize
{
	float dv;
	float acumulator;
	MoveFlashSize() noexcept :dv{ 100.0f }, acumulator { 0.0f } {};
	void operator()(std::vector<FlashSize>& size, std::vector<FlashPoint>& flash,unsigned int& front, unsigned int& end, float& dvPacMan, std::pair<int, SDL_FRect>& pacMan,int& kierPacMan, const float& dt) noexcept;
};

class FlashTexture
{
private:
	SDL_Texture* flashT;
public:
	FlashTexture() noexcept :flashT{ nullptr } {}
	void draw(SDL_Renderer* render, FlashSize& size) noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void destroyTexture() noexcept;
};

class FlashMenager
{
private:
	unsigned int _front;
	unsigned int _end;
	std::vector<FlashSize> _flashSize;
	std::vector<FlashPoint> _flashPoint;
	MoveFlashSize _moveFlashSize;
	FlashTexture _flashTexture;
public:
	FlashMenager() noexcept :_front{ 0 }, _end{ 0 }, _flashSize{ std::vector<FlashSize>(110, FlashSize{}) }, _flashPoint{ std::vector<FlashPoint>(110,FlashPoint{}) }, _moveFlashSize{}, _flashTexture{}{}
	void loadTexture(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render) noexcept;
	void operator()(float& dvPacMan, std::pair<int, SDL_FRect>pacMan,int& kierPacMan, const float& dt) noexcept;
	void destroyTexture() noexcept;
};

inline void FlashTexture::draw(SDL_Renderer* render, FlashSize& size) noexcept
{
	TextureMenager::drawTexture(render, flashT, &size._source, &size._dest);
}

inline void FlashTexture::loadTexture(SDL_Renderer* render) noexcept
{
	flashT = TextureMenager::loadTexture(render, "assets/image/velocity.png");
}

inline void FlashTexture::destroyTexture() noexcept
{
	SDL_DestroyTexture(flashT);
}

inline void FlashMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_flashTexture.loadTexture(render);
}

inline void FlashMenager::draw(SDL_Renderer* render) noexcept
{
	for (unsigned int i = _front; i < _end; i++)
	{
		_flashTexture.draw(render, _flashSize[i]);
	}
}

inline void FlashMenager::operator()(float& dvPacMan, std::pair<int, SDL_FRect>pacMan, int& kierPacMan, const float& dt) noexcept
{
	_moveFlashSize(_flashSize, _flashPoint, _front, _end, dvPacMan, pacMan, kierPacMan, dt);
}

inline void FlashMenager::destroyTexture() noexcept
{
	_flashTexture.destroyTexture();
}

