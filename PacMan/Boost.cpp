#include "Boost.h"
#include <ctime>
#include "Collision.h"

void BoostComponent::randBoost() noexcept
{
	std::vector<SDL_Rect> _boostRandom;
	srand(static_cast<unsigned int>(time(0)));
	_size = _boostRSource.size();
	unsigned int size = _size - 1;
	for (unsigned int i = 0; i < _size; ++i)
	{
		unsigned int index = rand() % (size);
		_boostRandom.emplace_back(_boostRSource[index]);
	}
	_boostRSource = _boostRandom;
	_size = _boostRSource.size();
}

void BoostKiler::operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept
{
	int w = source[0]._source.w;
	if (!w)
	{
		if (pacman.first)
		{
			if (_acumulator <= 10.0f)
			{
				_acumulator += dt;
			}
			else
			{
				pacman.first = 0;
				_acumulator = 0.0f;
			}
		}
		else
		{
			if (_acumulator <= 20.0f)
			{
				_acumulator += dt;
			}
			else
			{
				source[0]._source = { 0,0,26,26 };
				int x0 = 0;
				int x1 = source[1]._dest.x;
				int x2 = source[2]._dest.x;
				int x3 = source[3]._dest.x;
				do
				{
					source[0]._dest = boost.getNextBoost();
					x0 = source[0]._dest.x;
				} while (x0 == x1 || x0 == x2 || x0 == x3);
				_acumulator = 0.0f;
			}
		}
	}
	else
	{
		if (kolizja(source[0]._dest,pacman.second) && gKolizjiXY(source[0]._dest, pacman.second) >= (source[0]._dest.w / 2.0f * source[0]._dest.h))
		{
			_acumulator = 0.0f;
			source[0]._source = source[0]._dest = { 0,0,0,0 };
			pacman.first = 1;
			dx = 0.0f;
		}
		else
		{
			dx += dv * dt;
			if (dx < 286.0f)
			{
				source[0]._source.x = static_cast<int>(dx / 26.0f) * 26;
			}
			else
			{
				source[0]._source = source[0]._dest = { 0,0,0,0 };
				dx = 0.0f;
				_acumulator = 0.0f;
			}
		}
	}
}

void BoostHealer::operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept
{
	int w = source[1]._dest.w;
	if (!w)
	{
		if (_acumulator <= 25.0f)
		{
			_acumulator += dt;
		}
		else
		{
			source[1]._source = { 0,26,26,26 };
			int x0 = source[0]._dest.x;
			int x1 = 0;
			int x2 = source[2]._dest.x;
			int x3 = source[3]._dest.x;
			do
			{
				source[1]._dest = boost.getNextBoost();
				x1 = source[1]._dest.x;
			} while (x1 == x0 || x1 == x2 || x1 == x3);
			_acumulator = 0.0f;
		}
	}
	else
	{
		if (kolizja(source[1]._dest, pacman.second) && gKolizjiXY(source[1]._dest, pacman.second) >= (source[1]._dest.w / 2.0f * source[1]._dest.h))
		{
			_acumulator = 0.0f;
			source[1]._source = source[1]._dest = { 0,0,0,0 };
			dx = 0.0f;
			if (lives._dest.w < 224)
			{
				lives._dest.w += 32;
				lives._source.w += 32;
			}
		}
		else
		{
			dx += dv * dt;
			if (dx < 286.0f)
			{
				source[1]._source.x = static_cast<int>(dx / 26.0f) * 26;
			}
			else
			{
				source[1]._source = source[1]._dest = { 0,0,0,0 };
				dx = 0.0f;
				_acumulator = 0.0f;
			}
		}
	}
}

void BoostGodMode::operator()(LiveSize& lives, BoostComponent& boost,float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept
{
	int w = source[2]._dest.w;
	if (!w)
	{
		if (_acumulator <= 25.0f)
		{
			_acumulator += dt;
		}
		else
		{
			source[2]._source = { 0,52,26,26 };
			int x0 = source[0]._dest.x;
			int x1 = source[1]._dest.x;
			int x2 = 0;
			int x3 = source[3]._dest.x;
			do
			{
				source[2]._dest = boost.getNextBoost();
				x2 = source[2]._dest.x;
			} while (x2 == x0 || x2 == x1 || x2 == x3);
			_acumulator = 0.0f;
		}
	}
	else
	{
		if (kolizja(source[2]._dest, pacman.second) && gKolizjiXY(source[2]._dest, pacman.second) >= (source[1]._dest.w / 2.0f * source[1]._dest.h))
		{
			_acumulator = 0.0f;
			source[2]._source = source[2]._dest = { 0,0,0,0 };
			dx = 0.0f;
			if (pacManKill == 0.0f)
			{
				pacManKill = 5.5f;
			}
		}
		else
		{
			dx += dv * dt;
			if (dx < 286.0f)
			{
				source[2]._source.x = static_cast<int>(dx / 26.0f) * 26;
			}
			else
			{
				source[2]._source = source[2]._dest = { 0,0,0,0 };
				dx = 0.0f;
				_acumulator = 0.0f;
			}
		}
	}
}

void BoostFlash::operator()(LiveSize& lives, BoostComponent& boost, float& dvPacMan, float& pacManKill, std::pair<int, SDL_FRect>& pacman, BoostSize* source, const float& dt) noexcept
{
	int w = source[3]._source.w;
	if (!w)
	{
		if (dvPacMan == 200.0f)
		{
			if (_acumulator <= 7.0f)
			{
				_acumulator += dt;
			}
			else
			{
				dvPacMan = 100.f;
				_acumulator = 0.0f;
			}
		}
		else
		{
			if (_acumulator <= 15.0f)
			{
				_acumulator += dt;
			}
			else
			{
				source[3]._source = { 0,78,26,26 };
				int x0 = source[0]._dest.x;
				int x1 = source[1]._dest.x;
				int x2 = source[2]._dest.x;
				int x3 = 0;
				do
				{
					source[3]._dest = boost.getNextBoost();
					x3 = source[3]._dest.x;
				} while (x3 == x0 || x3 == x1 || x3 == x2);
				_acumulator = 0.0f;
			}
		}
	}
	else
	{
		if (kolizja(source[3]._dest, pacman.second) && gKolizjiXY(source[3]._dest, pacman.second) >= (source[3]._dest.w / 2.0f * source[3]._dest.h))
		{
			_acumulator = 0.0f;
			source[3]._source = source[3]._dest = { 0,0,0,0 };
			dvPacMan = 200.0;
			dx = 0.0f;
		}
		else
		{
			dx += dv * dt;
			if (dx < 286.0f)
			{
				source[3]._source.x = static_cast<int>(dx / 26.0f) * 26;
			}
			else
			{
				source[3]._source = source[3]._dest = { 0,0,0,0 };
				dx = 0.0f;
				_acumulator = 0.0f;
			}
		}
	}
}
