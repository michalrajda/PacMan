#pragma once
#include "TextureMenager.h"
#include "Boost.h"
#include "TunelSize.h"
#include "Points.h"
#include "Ghost.h"
#include "PacMan.h"

class MapLevel
{
public:
	static constexpr const unsigned int _width = 29;
	static constexpr const unsigned int _height = 24;
	static const char level1[_height][_width];
	static const char level2[_height][_width];
	static const char level3[_height][_width];
	static const char level4[_height][_width];
	static const char level5[_height][_width];
	static const char level6[_height][_width];
	static const char level7[_height][_width];
	static const char level8[_height][_width];
	static const char level9[_height][_width];
	static const char level10[_height][_width];
	static const char level11[_height][_width];
	static const char level12[_height][_width];
	static const char level13[_height][_width];
	static const char level14[_height][_width];
	static const char level15[_height][_width];
	static const char level16[_height][_width];
	static const char level17[_height][_width];
	static const char level18[_height][_width];
	static const char level19[_height][_width];
	static const char level20[_height][_width];
	static int _actualLevel[_height][_width];
	static TunelSize _tunelSize;
	static void initResetSource() noexcept
	{
		_tunelSource = _tunelSize;
		for (unsigned int y = 0; y < _height; y++)
		{
			for (unsigned int x = 0; x < _width; x++)
			{
				_mapLevelSource[y][x] = _actualLevel[y][x];
			}
		}
	}
	static void resetMapLevel() noexcept
	{
		_tunelSize = _tunelSource;
		for (unsigned int y = 0; y < _height; y++)
		{
			for (unsigned int x = 0; x < _width; x++)
			{
				_actualLevel[y][x] = _mapLevelSource[y][x];
			}
		}
	}
private:
	static TunelSize _tunelSource;
	static int _mapLevelSource[_height][_width];
};

struct MoveBackGround
{
	float dx;
	const float _dv;
	MoveBackGround() noexcept :dx{ 0.0f }, _dv{ 32.0f / 0.75f }{}
	void operator()(SDL_Rect& source, const float& dt) noexcept;
	void reset() noexcept;
};

class MapTexture
{
private:
	SDL_Texture* _texture;
public:
	MapTexture() noexcept :_texture{nullptr}  {}
	void destroyMap() noexcept;
	void loadBGround(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render, SDL_Rect& source, std::vector<std::pair<int, SDL_Rect>>& dest) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect& source, std::vector<std::pair<int, SDL_Rect>>& dest) noexcept;
};

class MapMenager
{
private:
	SDL_Rect _source;
	std::vector<std::pair<int, SDL_Rect>> _mapSize;
	std::vector<std::pair<int, SDL_Rect>> _mapSource;
	MapTexture _mapTexture;
	MoveBackGround _moveBackGrouns;
	std::vector<const char(*)[MapLevel::_width]> _levels;
public:
	MapMenager() noexcept :_source{ 0,0,32,32 }, _mapSize{ std::vector<std::pair<int,SDL_Rect>>() }, _mapSource{ std::vector<std::pair<int,SDL_Rect>>() }, _mapTexture{}, _moveBackGrouns{}
	{
		_levels.push_back(MapLevel::level1);
		_levels.push_back(MapLevel::level2);
		_levels.push_back(MapLevel::level3);
		_levels.push_back(MapLevel::level4);
		_levels.push_back(MapLevel::level5);
		_levels.push_back(MapLevel::level6);
		_levels.push_back(MapLevel::level7);
		_levels.push_back(MapLevel::level8);
		_levels.push_back(MapLevel::level9);
		_levels.push_back(MapLevel::level10);
		_levels.push_back(MapLevel::level11);
		_levels.push_back(MapLevel::level12);
		_levels.push_back(MapLevel::level13);
		_levels.push_back(MapLevel::level14);
		_levels.push_back(MapLevel::level15);
		_levels.push_back(MapLevel::level16);
		_levels.push_back(MapLevel::level17);
		_levels.push_back(MapLevel::level18);
		_levels.push_back(MapLevel::level19);
		_levels.push_back(MapLevel::level20);
	}
	void loadMap(int&& level, const char map[MapLevel::_height][MapLevel::_width], PacManMenager& pacman, PointMenager& pointComp, BoostMenager& boost, GhostMenager& _ghostMenager) noexcept;
	void draw(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void operator()(const float& dt) noexcept;
	void destroyTexture() noexcept;
	void clear() noexcept;
	void resetBackGround() noexcept;
	void initResetSource() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void loadNextLevel(int& level, PacManMenager& pacman, PointMenager& pointComp, BoostMenager& boost, GhostMenager& _ghostMenager, Flags& flag) noexcept;
};

inline void MoveBackGround::operator()(SDL_Rect& source, const float& dt) noexcept
{
	dx += _dv * dt;
	source.x = (static_cast<int>(dx / 32.0f) % 6) * 32;
}

inline void MoveBackGround::reset() noexcept
{
	dx = 0.0f;
}

inline void MapTexture::destroyMap() noexcept
{
	SDL_DestroyTexture(_texture);
}

inline void MapTexture::loadBGround(SDL_Renderer* render) noexcept
{
	_texture = TextureMenager::loadTexture(render, "assets/image/map.png");
}

inline void MapTexture::draw(SDL_Renderer* render, SDL_Rect& source, std::vector<std::pair<int, SDL_Rect>>& dest) noexcept
{
	SDL_Rect _source = source;
	unsigned int size = dest.size();
	for (unsigned int i = 0; i < size; i++)
	{
		_source.y = dest[i].first * 32;
		TextureMenager::drawTexture(render, _texture, &_source, &dest[i].second);
	}
}

inline void MapTexture::draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect& source, std::vector<std::pair<int, SDL_Rect>>& dest) noexcept
{
	SDL_Rect _source = source;
	unsigned int size = dest.size();
	for (unsigned int i = 0; i < size; i++)
	{
		_source.y = dest[i].first * 32;
		TextureMenager::drawTextureOnRenderTexture(render, _texture, destTexture, &_source, &dest[i].second);
	}
}

inline void MapMenager::draw(SDL_Renderer* render) noexcept
{
	_mapTexture.draw(render, _source, _mapSize);
}

inline void MapMenager::draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	_mapTexture.draw(render, destTexture, _source, _mapSize);
}

inline void MapMenager::operator()(const float& dt) noexcept
{
	_moveBackGrouns(_source, dt);
}

inline void MapMenager::destroyTexture() noexcept
{
	_mapTexture.destroyMap();
}

inline void MapMenager::clear() noexcept
{
	_source = { 0,0,32,32 };
	_mapSize.clear();
}

inline void MapMenager::resetBackGround() noexcept
{
	MapLevel::resetMapLevel();
	_source = { 0,0,32,32 };
	_moveBackGrouns.reset();
	_mapSize = _mapSource;
}

inline void MapMenager::initResetSource() noexcept
{
	_mapSource = _mapSize;
	MapLevel::initResetSource();
}

inline void MapMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_mapTexture.loadBGround(render);
}

inline void MapMenager::loadNextLevel(int& level, PacManMenager& pacman, PointMenager& pointComp, BoostMenager& boost, GhostMenager& _ghostMenager, Flags& flag) noexcept
{
	int size = static_cast<int>(_levels.size());
	int _level = level;
	if (level < size)
	{
		loadMap(std::move(_level), _levels[level], pacman, pointComp, boost, _ghostMenager);
		flag._actLevel = true;
	}
	else
	{
		flag._actLevel = false;
		flag._nextLevel = false;
		flag._isLives = false;
		flag._reset = true;
	}
}