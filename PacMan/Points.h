#pragma once
#include "TextureMenager.h"
#include "Collision.h"
#include <cmath>
#include "Flags.h"

class PointComponent
{
private:
	std::vector<int> sparse;
	std::vector<int> dense;
public:
	PointComponent() noexcept :sparse{ std::vector<int>() }, dense{ std::vector<int>() }{}
	std::vector<SDL_Rect> _vpoints;
	void clear() noexcept;
	void pop_backCompID(int&& componentID, Flags& _flag) noexcept;
	int ged_componentID(int&& pktnaMapie) noexcept;
	void resizeSparse(int&& height, int&& width, int&& value) noexcept;
	void push_backPoints(int&& x, int&& y, int& width) noexcept;
	SDL_Rect& get_ComponentRect(int& componentID) noexcept;
};

struct MovePoints
{
	const float _dv;
	float _dx;
	MovePoints() noexcept :_dv{ 16.0f / 0.05f }, _dx{ 0.0f }{}
	void reset() noexcept;
	void operator()(SDL_Rect& source, const float& dt) noexcept;
};

class PointTexture
{
private:
	SDL_Texture* _pointsT;
	SDL_Rect _pointsSource;
public:
	PointTexture() noexcept :_pointsT{nullptr},_pointsSource { 0, 0, 16, 16 } {}
	~PointTexture() noexcept
	{
		if (_pointsT != nullptr) destroyTexture();
	}
	SDL_Rect& getPoints() noexcept;
	void destroyTexture() noexcept;
	void loadPoint(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render, const std::vector<SDL_Rect>& dest) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture, const std::vector<SDL_Rect>& dest) noexcept;
	void reset() noexcept;
};

class PointMenager
{
private:
	PointComponent _source;
	PointComponent _dest;
	PointTexture _pointsTexture{};
	MovePoints _movePoints{};
	int _width;
	int _points;
public:
	PointMenager() noexcept :_source{}, _dest{}, _pointsTexture{}, _movePoints{}, _width{ 0 }, _points{ 0 }{}
	void operator()(const float& dt) noexcept;
	void clear() noexcept;
	void destroyTexture() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render);
	void push_backPoints(int&& x, int&& y, int& width) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void resizeSparse(int&& height, int&& width, int&& value) noexcept;
	void initResetSource() noexcept;
	void reset() noexcept;
	void pointsCollision(SDL_FRect& pacman, Flags& _flags);
	const int& getPoints() noexcept;
};

inline void PointComponent::clear() noexcept
{
	sparse.clear();
	dense.clear();
	_vpoints.clear();
}

inline SDL_Rect& PointComponent::get_ComponentRect(int& componentID) noexcept
{
	return _vpoints[componentID];
}

inline void PointComponent::resizeSparse(int&& height, int&& width, int&& value) noexcept
{
	dense.clear();
	unsigned int size = (height - 2) * (width - 2);
	sparse.resize(size, value);
}

inline int PointComponent::ged_componentID(int&& pktNaMapie) noexcept
{
	if (pktNaMapie > 0 && static_cast<unsigned int>(pktNaMapie) < sparse.size())
	{
		return sparse[pktNaMapie];
	}
	return -1;
}

inline void MovePoints::reset() noexcept
{
	_dx = 0.0f;
}

inline void MovePoints::operator()(SDL_Rect& source, const float& dt) noexcept
{
	_dx += _dv * dt;
	source.x = (static_cast<int>(_dx / 16.0f) % 38) * 16;
}

inline SDL_Rect& PointTexture::getPoints() noexcept
{
	return _pointsSource;
}

inline void PointTexture::destroyTexture() noexcept
{
	SDL_DestroyTexture(_pointsT);
}

inline void PointTexture::loadPoint(SDL_Renderer* render) noexcept
{
	_pointsT = TextureMenager::loadTexture(render, "assets/image/punkty.png");
}

inline void PointTexture::draw(SDL_Renderer* render, const std::vector<SDL_Rect>& dest) noexcept
{
	TextureMenager::drawVectorRect(render, _pointsT, &_pointsSource, dest);
}

inline void PointTexture::draw(SDL_Renderer* render, SDL_Texture* destTexture, const std::vector<SDL_Rect>& dest) noexcept
{
	unsigned int size = dest.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		TextureMenager::drawTextureOnRenderTexture(render, _pointsT, destTexture, &_pointsSource, &dest[i]);
	}
}

inline void PointTexture::reset() noexcept
{
	_pointsSource = { 0,0,16,16 };
}

inline void PointMenager::operator()(const float& dt) noexcept
{
	_movePoints(_pointsTexture.getPoints(), dt);
}

inline void PointMenager::clear() noexcept
{
	_dest.clear();
	_pointsTexture.reset();
	_movePoints.reset();
}

inline void PointMenager::destroyTexture() noexcept
{
	_pointsTexture.destroyTexture();
}

inline void PointMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_pointsTexture.loadPoint(render);
}

inline void PointMenager::draw(SDL_Renderer* render)
{
	_pointsTexture.draw(render, _dest._vpoints);
}

inline void PointMenager::push_backPoints(int&& x, int&& y, int& width) noexcept
{
	_dest.push_backPoints(std::move(x), std::move(y), width);
}

inline void PointMenager::draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	_pointsTexture.draw(render, destTexture, _dest._vpoints);
}

inline void PointMenager::resizeSparse(int&& height, int&& width, int&& value) noexcept
{
	_width = width - 2;
	_dest.resizeSparse(std::move(height), std::move(width), std::move(value));
}

inline void PointMenager::initResetSource() noexcept
{
	_source = _dest;
}

inline void PointMenager::reset() noexcept
{
	_dest = _source;
	_points = 0;
	_pointsTexture.reset();
	_movePoints.reset();
}

inline const int& PointMenager::getPoints() noexcept
{
	return _points;
}
