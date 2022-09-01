#include "Points.h"

void PointComponent::push_backPoints(int&& x, int&& y, int& width) noexcept
{
	unsigned int dsized = dense.size();
	int pointsOnMap = y * (width - 4) + x;
	dense.emplace_back(pointsOnMap);
	sparse[pointsOnMap] = dsized;
	SDL_Rect point{ x * 32 + 8, y * 32 + 8, 16, 16 };
	_vpoints.emplace_back(point);
}


void PointComponent::pop_backCompID(int&& componentID, Flags& _flags) noexcept
{
	if (componentID < static_cast<int>(_vpoints.size()))
	{
		sparse[dense.back()] = componentID;
		sparse[dense[componentID]] = -1;
		std::swap(dense[componentID], dense.back());
		std::swap(_vpoints[componentID], _vpoints.back());
		_vpoints.pop_back();
		dense.pop_back();
		if (_vpoints.empty())
		{
			_flags._actLevel = false;
			_flags._nextLevel = true;
		}
	}
}

void PointMenager::pointsCollision(SDL_FRect& pacman, Flags& _flags)
{
	const int x = static_cast<int>(std::trunc((pacman.x + pacman.w / 2.0f) / 32.0f));
	const int y = static_cast<int>(std::trunc((pacman.y + pacman.h / 2.0f) / 32.0f));
	int pozXY = y * _width + x;
	int nrPoints = _dest.ged_componentID(std::move(pozXY));
	if (nrPoints >= 0)
	{
		SDL_Rect punkt = _dest.get_ComponentRect(nrPoints);
		if (gKolizjiXY(pacman, punkt) >= (punkt.w * punkt.h))
		{
			_dest.pop_backCompID(std::move(nrPoints), _flags);
			++_points;
		}
	}
}