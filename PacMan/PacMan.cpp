#include "PacMan.h"
#include <cmath>
void PacManMenager::resetPacMan() noexcept
{
	_changePacMan.reset();
	_pacManS = { 0,128,32,32 };
	_dv = 100.0f;
	_directionS = 4;
	for (auto& x : _directionD)
	{
		x = 0;
	}
	_pacMan[1] = _pacMan[0] = _pacManReset;
	_pacManKill = 0.0f;
	_acumulator = 0;
	_pacManTexture.setAlfaMode(255);
}

void PacManMenager::movePacMan(const float& dt) noexcept
{
	_pacMan[1].second.x += _dv * static_cast<float>(_directionD[0]) * dt;
	_pacMan[1].second.y += _dv * static_cast<float>(_directionD[1]) * dt;
	if (_pacManKill > 0.0f)
	{
		_pacManKill -= dt;
	}
	else _pacManKill = 0.0f;
}

void PacManMenager::mapCollision(const int map[][29], TunelSize& tunelSize, const float& dt) noexcept
{
	if (_pacManKill > 0.0f)
	{
		if (_pacMan[1].first)
		{
			_pacManKill = 0.0f;
			_acumulator = 0;
		}
		else
		{
			_pacManKill -= dt;
			if (_pacManKill <= 0.0f)
			{
				_pacManKill = 0.0f;
				_acumulator = 0;
			}
			else
			{
				_acumulator += 5;
			}
		}
		unsigned char alfaMode = static_cast<unsigned char>(std::abs(_acumulator % 511 - 255));
		_pacManTexture.setAlfaMode(alfaMode);
	}
	if (_directionD[0] == _directionD[1])
	{
		_directionS = 4;
	}
	else
	{
		auto dirX = _directionD[0];
		auto dirY = _directionD[1];
		SDL_FRect actPacMan = _pacMan[1].second;
		actPacMan.x += 2.0f * 32.0f;
		actPacMan.y += 2.0f * 32.0f;
		if (dirX)
		{
			actPacMan.x += _dv * dt * static_cast<float>(dirX);
			auto y = static_cast<int>(std::trunc(actPacMan.y / 32.0f));
			auto ydy = static_cast<int>(std::trunc((actPacMan.y + actPacMan.h) / 32.0f));
			if (dirX < 0)
			{
				_directionS = 1; 
				auto x = static_cast<int>(std::trunc(actPacMan.x / 32.0f));
				SDL_FRect rect1{ x * 32.0f, y * 32.0f, 32.0f, 32.0f };
				SDL_FRect rect2{ x * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
				auto r1 = static_cast<float>(map[y][x] * (kolizja(rect1, actPacMan)));
				auto r2 = static_cast<float>(map[ydy][x] * (kolizja(rect2, actPacMan)));
				auto sumCollison = r1 + r2;
				if (sumCollison)
				{
					if (sumCollison == 1.0f)
					{
						if (r1)
						{
							if (gKolizjiY(rect1,actPacMan) < 15.0f)
							{
								_pacMan[1].second.y += _dv * dt;
								_directionS = 2;
							}
						}
						else
						{
							if (gKolizjiY(rect2, actPacMan) < 15.0f)
							{
								_pacMan[1].second.y -= _dv * dt;
								_directionS = 3;
							}
						}
					}
					else if (sumCollison == 3.0f)
					{
						_pacMan[1].second = tunelSize._right;
					}
					else if (sumCollison == 4.0f)
					{
						_pacMan[1].second = tunelSize._left;
					}
				}
				else
				{
					_pacMan[1].second.x += static_cast<float>(dirX) * _dv * dt;
				}
			}
			else
			{
				_directionS = 0;
				auto xdx = static_cast<int>(std::trunc((actPacMan.x + actPacMan.w) / 32.0f));
				SDL_FRect rect1{ xdx * 32.0f, y * 32.0f, 32.0f, 32.0f };
				SDL_FRect rect2{ xdx * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
				auto r1 = static_cast<float>(map[y][xdx] * (kolizja(rect1, actPacMan)));
				auto r2 = static_cast<float>(map[ydy][xdx] * (kolizja(rect2, actPacMan)));
				auto sumCollison = r1 + r2;
				if (sumCollison)
				{
					if (sumCollison == 1.0f)
					{
						if (r1)
						{
							if (gKolizjiY(rect1, actPacMan) < 15.0f)
							{
								_pacMan[1].second.y += _dv * dt;
								_directionS = 2;
							}
						}
						else
						{
							if (gKolizjiY(rect2, actPacMan) < 15.0f)
							{
								_pacMan[1].second.y -= _dv * dt;
								_directionS = 3;
							}
						}
					}
					else if (sumCollison == 4.0f)
					{
						_pacMan[1].second = tunelSize._left;
					}
					else if (sumCollison == 3.0f)
					{
						_pacMan[1].second = tunelSize._right;
					}
				}
				else
				{
					_pacMan[1].second.x += static_cast<float>(dirX) * _dv * dt;
				}
			}
		}
		else
		{
			actPacMan.y += _dv * dt * static_cast<float>(dirY);
			auto x = static_cast<int>(std::trunc(actPacMan.x / 32.0f));
			auto xdx = static_cast<int>(std::trunc((actPacMan.x + actPacMan.w) / 32.0f));
			if (dirY < 0)
			{
				_directionS = 3;
				auto y = static_cast<int>(std::trunc(actPacMan.y / 32.0f));
				SDL_FRect rect1{ x * 32.0f, y * 32.0f, 32.0f, 32.0f };
				SDL_FRect rect2{ xdx * 32.0f, y * 32.0f, 32.0f, 32.0f };
				auto r1 = static_cast<float>(map[y][x] * (kolizja(rect1, actPacMan)));
				auto r2 = static_cast<float>(map[y][xdx] * (kolizja(rect2, actPacMan)));
				auto sumCollison = r1 + r2;
				if (sumCollison)
				{
					if (sumCollison == 1.0f)
					{
						if (r1)
						{
							if (gKolizjiX(rect1, actPacMan) < 15.0f)
							{
								_pacMan[1].second.x += _dv * dt;
								_directionS = 0;
							}
						}
						else
						{
							if (gKolizjiX(rect2, actPacMan) < 15.0f)
							{
								_pacMan[1].second.x -= _dv * dt;
								_directionS = 1;
							}
						}
					}
					else if (sumCollison == 3.0f)
					{
						_pacMan[1].second = tunelSize._right;
					}
					else if (sumCollison == 4.0f)
					{
						_pacMan[1].second = tunelSize._left;
					}
				}
				else
				{
					_pacMan[1].second.y += static_cast<float>(dirY) * _dv * dt;
				}

			}
			else
			{
				_directionS = 2;
				auto ydy = static_cast<int>(std::trunc((actPacMan.y + actPacMan.h) / 32.0f));
				SDL_FRect rect1{ x * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
				SDL_FRect rect2{ xdx * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
				auto r1 = static_cast<float>(map[ydy][x] * (kolizja(rect1, actPacMan)));
				auto r2 = static_cast<float>(map[ydy][xdx] * (kolizja(rect2, actPacMan)));
				auto sumCollison = r1 + r2;
				if (sumCollison)
				{
					if (sumCollison == 1.0f)
					{
						if (r1)
						{
							if (gKolizjiX(rect1, actPacMan) < 15.0f)
							{
								_pacMan[1].second.x += _dv * dt;
								_directionS = 0;
							}
						}
						else
						{
							if (gKolizjiX(rect2, actPacMan) < 15.0f)
							{
								_pacMan[1].second.x -= _dv * dt;
								_directionS = 1;
							}
						}
					}
					else if (sumCollison == 4.0f)
					{
						_pacMan[1].second = tunelSize._left;
					}
					else if (sumCollison == 3.0f)
					{
						_pacMan[1].second = tunelSize._right;
					}
				}
				else
				{
					_pacMan[1].second.y += static_cast<float>(dirY) * _dv * dt;
				}
			}
		}
	}
}


void PacManMenager::mapCollision(const int map[][29],TunelSize& tunelSize) noexcept
{
	unsigned int suma_dir{};
	_directionS = 0;
	if (_directionD[0] == _directionD[1])
	{
		_directionS = 4;
		return;
	}
	if (_directionD[0])
	{
		if (_directionD[0] < 0)
		{
			_directionS = 1;
		}
		else
		{
			_directionS = 0;
		}
	}
	else
	{
		if (_directionD[1] < 0)
		{
			_directionS = 3;
		}
		else
		{
			_directionS = 2;
		}
	}
	SDL_FRect pacMan = _pacMan[1].second;
	pacMan.x += 2.0f * 32.0f;
	pacMan.y += 2.0f * 32.0f;
	const int x = static_cast<int>(std::trunc(pacMan.x / 32.0f));
	const int y = static_cast<int>(std::trunc(pacMan.y / 32.0f));
	const int xdx = static_cast<int>(std::trunc((pacMan.x + pacMan.w) / 32.0f));
	const int ydy = static_cast<int>(std::trunc((pacMan.y + pacMan.h) / 32.0f));
	SDL_FRect r_xy{ x * 32.0f, y * 32.0f, 32.0f, 32.0f };
	SDL_FRect r_dx_y{ xdx * 32.0f, y * 32.0f, 32, 32.0f };
	SDL_FRect r_x_dy{ x * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
	SDL_FRect r_dx_dy{ xdx * 32.0f, ydy * 32.0f, 32.0f, 32.0f };
	float yx = static_cast<float>(map[y][x] * (kolizja(r_xy, pacMan)));
	float y_dx = static_cast<float>(map[y][xdx] * (kolizja(r_dx_y, pacMan)));
	float dy_x = static_cast<float>(map[ydy][x] * (kolizja(r_x_dy, pacMan)));
	float dy_dx = static_cast<float>(map[ydy][xdx] * (kolizja(r_dx_dy, pacMan)));
	auto sum_kolizji = yx + y_dx + dy_x + dy_dx;
	if (sum_kolizji == 3.0f)
	{
		_pacMan[1].second = tunelSize._right;
		_directionS = 1;
		return;
	}
	else if (sum_kolizji == 4.0f)
	{
		_pacMan[1].second = tunelSize._left;
		_directionS = 0;
		return;
	}
	sum_kolizji = std::ceil(sum_kolizji / 4.0f);
	r_xy = r_xy * yx + r_dx_y * y_dx + r_x_dy * dy_x + r_dx_dy * dy_dx;
	auto xDeph = sum_kolizji * gKolizjiX(pacMan, r_xy);
	auto yDeph = sum_kolizji * gKolizjiY(pacMan, r_xy);
	_pacMan[1].second.x -= _directionD[0] * xDeph;
	_pacMan[1].second.y -= _directionD[1] * yDeph;
	if (xDeph * yDeph)
	{
		if (!_directionD[0] && xDeph < 15.0f)
		{
			float x_x = pacMan.x - r_xy.x;
			if (x_x < 0.0f)
			{
				_directionS = 1;
				_pacMan[1].second.x -= yDeph;
			}
			else
			{
				_directionS = 0;
				_pacMan[1].second.x += yDeph;
			}
		}
		else if (!_directionD[1] && yDeph < 15.0f)
		{
			float y_y = pacMan.y - r_xy.y;
			if (y_y < 0.0f)
			{
				_directionS = 3;
				_pacMan[1].second.y -= xDeph;
			}
			else
			{
				_directionS = 2;
				_pacMan[1].second.y += xDeph;
			}
		}
	}
}
