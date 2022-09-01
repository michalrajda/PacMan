#pragma once
#include <algorithm>
#include "TextureMenager.h"
#include "Collision.h"
#include "TunelSize.h"
#include "PacManSource.h"

class TexturePacMan
{
private:
	SDL_Texture* _pacManT[2];
	SDL_Rect _pacManD;
	int _index;
public:
	TexturePacMan() noexcept :_pacManT{ {nullptr},{nullptr} }, _pacManD{}, _index{0} {}
	void setAlfaMode(unsigned char& alfa) noexcept;
	void setAlfaMode(unsigned char&& alfa) noexcept;
	void destroyTexture() noexcept;
	void draw(SDL_Renderer* render, SDL_Rect* pacManS) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect* pacManS) noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void setSize(std::pair<int, SDL_FRect>& pacManD) noexcept;
};

struct ChangePacMan
{
	float _dx;
	const float _dv;
	int _pKierunek;
	ChangePacMan() noexcept :_dx{ 0.0f }, _dv{ 32.0f / 0.04f }, _pKierunek{ 4 }{}
	void reset() noexcept;
	void operator()(SDL_Rect& source, const int& dirdest, const float& dt) noexcept;
};

class PacManMenager
{
	float _pacManKill;
	PacManSource _pacManSource;
	std::pair<int, SDL_FRect> _pacMan[2];
	std::pair<int, SDL_FRect> _pacManReset;
	SDL_Rect _pacManS;
	ChangePacMan _changePacMan;
	TexturePacMan _pacManTexture;
	int _directionD[2];
	int _directionS;
	float _dv;
	int _acumulator;
public:
	PacManMenager() noexcept : _pacManSource{ _pacManKill,_directionS }, _pacMan{ std::pair<int, SDL_FRect>(0, { 0.0f,0.0f,0.0f,0.0f }) }, _pacManReset{ std::pair<int,SDL_FRect>(0,{}) },
					           _pacManS{ 0,128,32,32 }, _changePacMan{}, _pacManTexture{}, _directionD{ {0},{0} }, _directionS{ 4 }, _pacManKill{0.0}, _dv{ 100.0f }, _acumulator {0}
	{
		_pacManSource._pacMan = _pacMan;
		_pacManSource._directionD = _directionD;
	}
	void clear() noexcept;
	void draw(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	PacManSource& getPacManSource() noexcept;
	int* getDirection() noexcept;
	void setSize() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void destroyTexture() noexcept;
	std::pair<int, SDL_FRect>* getPacMan() noexcept;
	void initResetSource() noexcept;
	void operator()(const float& dt) noexcept;
	void resetPacMan() noexcept;
	void movePacMan(const float& dt) noexcept;
	void mapCollision(const int map[][29], TunelSize& tunelSize,const float& dt) noexcept;
	void mapCollision(const int map[][29],TunelSize& tunelSize) noexcept;
	float& getPacManKill() noexcept;
	float& getPacManDV() noexcept;
	int& getDirectionTexture() noexcept;
};

inline void TexturePacMan::setAlfaMode(unsigned char& alfa) noexcept
{
	TextureMenager::setAlfaMode(_pacManT[0], alfa);
}

inline void TexturePacMan::setAlfaMode(unsigned char&& alfa) noexcept
{
	TextureMenager::setAlfaMode(_pacManT[0], alfa);
}

inline void TexturePacMan::destroyTexture() noexcept
{
	SDL_DestroyTexture(_pacManT[0]);
	SDL_DestroyTexture(_pacManT[1]);
}

inline void TexturePacMan::draw(SDL_Renderer* render, SDL_Rect* pacManS) noexcept
{
	TextureMenager::drawTexture(render, _pacManT[_index], pacManS, &_pacManD);
}

inline void TexturePacMan::draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect* pacManS) noexcept
{
	TextureMenager::drawTextureOnRenderTexture(render, _pacManT[_index], destTexture, pacManS, &_pacManD);
}

inline void TexturePacMan::loadTexture(SDL_Renderer* render) noexcept
{
	_pacManT[0] = TextureMenager::loadTexture(render, "assets/image/pacman.png");
	_pacManT[1] = TextureMenager::loadTexture(render, "assets/image/pacmanboost.png");
}

inline void TexturePacMan::setSize(std::pair<int, SDL_FRect>& pacManD) noexcept
{
	_pacManD.x = static_cast<int>(pacManD.second.x);
	_pacManD.y = static_cast<int>(pacManD.second.y);
	_pacManD.w = static_cast<int>(pacManD.second.w);
	_pacManD.h = static_cast<int>(pacManD.second.h);
	_index = pacManD.first;
}

inline void ChangePacMan::reset() noexcept
{
	_dx = 0.0f;
	_pKierunek = 4;
}

inline void ChangePacMan::operator()(SDL_Rect& source, const int& dirdest, const float& dt) noexcept
{
	if (_pKierunek != dirdest)
	{
		_dx = 0.0f;
		_pKierunek = dirdest;
	}
	else _dx += _dv * dt;
	source.x = (static_cast<int>(_dx / 32.0f) % 18) * 32;
	source.y = _pKierunek * 32;
}

inline void PacManMenager::clear() noexcept
{
	_dv = 100.0f;
	_directionS = 4;
	_pacManKill = 0.0f;
	_pacManS = { 0,128,32,32 };
	_directionD[1] = _directionD[0] = 0;
	_changePacMan.reset();
	_acumulator = 0;
	_pacManTexture.setAlfaMode(255);
}

inline void PacManMenager::draw(SDL_Renderer* render) noexcept
{
	_pacManTexture.draw(render, &_pacManS);
}

inline void PacManMenager::draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	_pacManTexture.draw(render, destTexture, &_pacManS);
}

inline PacManSource& PacManMenager::getPacManSource() noexcept
{
	return _pacManSource;
}

inline int* PacManMenager::getDirection() noexcept
{
	return _directionD;
}

inline void PacManMenager::setSize() noexcept
{
	_pacManTexture.setSize(_pacManSource._pacMan[1]);
}

inline void PacManMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_pacManTexture.loadTexture(render);
}

inline void PacManMenager::destroyTexture() noexcept
{
	_pacManTexture.destroyTexture();
}

inline std::pair<int, SDL_FRect>* PacManMenager::getPacMan() noexcept
{
	return _pacMan;
}

inline void PacManMenager::initResetSource() noexcept
{
	_pacManReset = _pacMan[0];
}

inline void PacManMenager::operator()(const float& dt) noexcept
{
	_changePacMan(_pacManS, _directionS, dt);
}

inline float& PacManMenager::getPacManKill() noexcept
{
	return _pacManKill;
}

inline float& PacManMenager::getPacManDV() noexcept
{
	return _dv;
}

inline int& PacManMenager::getDirectionTexture() noexcept
{
	return _directionS;
}