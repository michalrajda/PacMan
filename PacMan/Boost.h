#pragma once
#include "TextureMenager.h"
#include "Lives.h"

struct BoostSize
{
	SDL_Rect _source;
	SDL_Rect _dest;
	BoostSize() noexcept :_source{}, _dest{}{}
};

class BoostComponent
{
private:
	unsigned int _index;
	unsigned int _size;
	std::vector<SDL_Rect> _boostRSource;
	friend class BoostMenager;
public:
	BoostComponent() noexcept : _index{ 0 }, _size{ 0 }, _boostRSource{ std::vector<SDL_Rect>() }{}
	void emplace_back(int&& x, int&& y) noexcept;
	void randBoost() noexcept;
	SDL_Rect& getNextBoost() noexcept;
	void reset() noexcept;
};

class BoostTexture
{
private:
	SDL_Texture* boostT;
public:
	BoostTexture() : boostT{ nullptr }{};
	void draw(SDL_Renderer* render, BoostSize& _boostSize) noexcept;
	void destroyTexture() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
};

struct MoveBoost
{
	float _acumulator;
	float dx;
	const float dv;
	MoveBoost() noexcept :_acumulator{ 0.0f }, dx{ 0.0f }, dv{ 26.0f }{}
	virtual ~MoveBoost() {};
	void reset() noexcept;
	virtual void operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept = 0;
};

struct BoostKiler :MoveBoost
{
	void operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept override;
};

struct BoostHealer :MoveBoost
{
	void operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept override;
};

struct BoostGodMode :MoveBoost
{
	void operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept override;
};

struct BoostFlash :MoveBoost
{
	void operator()(LiveSize& lives, BoostComponent& boost, float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept override;
};

class BoostMenager
{
private:
	BoostSize _boostSize[4];
	BoostComponent _boostComponent[4];
	BoostTexture _boostTexture;
	MoveBoost* _moveBoost[4];
	std::vector<SDL_Rect> _boostRSource;
public:
	BoostMenager() noexcept :_boostComponent{}, _boostTexture{}, _moveBoost{ nullptr }, _boostSize{}, _boostRSource{ std::vector<SDL_Rect>() }
	{
		_moveBoost[0] = new BoostKiler{};
		_moveBoost[1] = new BoostHealer{};
		_moveBoost[2] = new BoostGodMode{};
		_moveBoost[3] = new BoostFlash{};
	}
	~BoostMenager()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			delete _moveBoost[i];
		}
	}
	void emplace_back(int&& x, int&& y,int& level) noexcept;
	void clear() noexcept;
	void initBoostComponent() noexcept;
	void initResetSource() noexcept;
	void reset() noexcept;
	void draw(SDL_Renderer* render) noexcept;
	void destroyTexture() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void operator()(LiveSize& live,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, const float& dt) noexcept;
};

inline void BoostComponent::emplace_back(int&& x, int&& y) noexcept
{
	SDL_Rect boost{ x * 32 + 3, y * 32 + 3,26,26 };
	_boostRSource.emplace_back(boost);
}

inline SDL_Rect& BoostComponent::getNextBoost() noexcept
{
	unsigned int i = _index % _size;
	++_index;
	return _boostRSource[i];
}

inline void BoostComponent::reset() noexcept
{
	_index = 0;
}

inline void BoostTexture::destroyTexture() noexcept
{
	SDL_DestroyTexture(boostT);
}

inline void BoostTexture::draw(SDL_Renderer* render, BoostSize& _boostSize) noexcept
{
	TextureMenager::drawTexture(render, boostT, &_boostSize._source, &_boostSize._dest);
}

inline void BoostTexture::loadTexture(SDL_Renderer* render) noexcept
{
	boostT = TextureMenager::loadTexture(render, "assets/image/boost.png");
}

inline void MoveBoost::reset() noexcept
{
	_acumulator = 0;
	dx = 0.0f;
}

inline void BoostMenager::emplace_back(int&& x, int&& y,int& level) noexcept
{
	_boostComponent[0].emplace_back(std::move(x), std::move(y));
	if (level)
	{
		if (level % 2)
		{
			_boostComponent[1].emplace_back(std::move(x), std::move(y));
		}
		if (!(level % 2))
		{
			_boostComponent[2].emplace_back(std::move(x), std::move(y));
		}
		if (!(level % 2) || !(level % 3))
		{
			_boostComponent[3].emplace_back(std::move(x), std::move(y));
		}
	}
}

inline void BoostMenager::clear() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_boostSize[i]._source = _boostSize[i]._dest = { 0,0,0,0 };
		_boostComponent[i]._boostRSource.clear();
		_boostComponent[i]._index = 0;
		_moveBoost[i]->reset();
	}
}

inline void BoostMenager::initBoostComponent() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (_boostComponent[i]._size = _boostComponent[i]._boostRSource.size())
		{
			_boostComponent[i].randBoost();
		}
	}
}

inline void BoostMenager::initResetSource() noexcept
{
	_boostRSource = _boostComponent[0]._boostRSource;
}

inline void BoostMenager::reset() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_boostSize[i]._source = _boostSize[i]._dest = { 0,0,0,0 };
		_moveBoost[i]->reset();
	}
	_boostComponent[0]._boostRSource = _boostRSource;
	_boostComponent[0].reset();
	_boostComponent[0].randBoost();
	_boostComponent[1]._boostRSource.clear();
	_boostComponent[1]._index = 0;
	_boostComponent[2]._boostRSource.clear();
	_boostComponent[2]._index = 0;
	_boostComponent[3]._boostRSource.clear();
	_boostComponent[3]._index = 0;
}

inline void BoostMenager::draw(SDL_Renderer* render) noexcept
{
	for (auto& x : _boostSize)
	{
		_boostTexture.draw(render, x);
	}
}

inline void BoostMenager::destroyTexture() noexcept
{
	_boostTexture.destroyTexture();
}

inline void BoostMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_boostTexture.loadTexture(render);
}

inline void BoostMenager::operator()(LiveSize& live,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, const float& dt) noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		unsigned int size = _boostComponent[i]._boostRSource.size();
		if (size)
		{
			_moveBoost[i]->operator()(live, _boostComponent[i], dvPacMan, pacManKill, pacman, _boostSize, dt);
		}
	}
}