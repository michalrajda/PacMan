#include "FlashTrack.h"
#include <ctime>

void MoveFlashSize::operator()(std::vector<FlashSize>& size, std::vector<FlashPoint>& flash, unsigned int& front, unsigned int& end, float& dvPacMan, std::pair<int, SDL_FRect>& pacMan,int& kierPacMan, const float& dt) noexcept
{
	if (dvPacMan != dv)
	{
		dv = dvPacMan;
		if (dvPacMan == 200.0f)
		{
			acumulator += dt;
			if (kierPacMan == 0)
			{
				size[end]._source = { 0,0,4,4 };
				srand(static_cast<unsigned int>(time(0)));
				int x = static_cast<int>(pacMan.second.x);
				float index = static_cast<float>(rand() % (18) + 4);
				int y = static_cast<int>(pacMan.second.y + index);
				size[end]._dest = { x,y,4,4 };
				flash[end]._dv[0] = -25.0f;
				flash[end]._dv[1] = 0.0f;
				flash[end].dxy[0] = static_cast<float>(x);
				flash[end].dxy[1] = static_cast<float>(y);
				end++;
			}
			else if (kierPacMan == 1)
			{
				size[end]._source = { 0,0,4,4 };
				srand(static_cast<unsigned int>(time(0)));
				int x = static_cast<int>(pacMan.second.x + pacMan.second.w);
				float index = static_cast<float>(rand() % (18) + 4);
				int y = static_cast<int>(pacMan.second.y + index);
				size[end]._dest = { x,y,4,4 };
				flash[end]._dv[0] = 25.0f;
				flash[end]._dv[1] = 0.0f;
				flash[end].dxy[0] = static_cast<float>(x);
				flash[end].dxy[1] = static_cast<float>(y);
				end++;
			}
			else if (kierPacMan == 2)
			{
				size[end]._source = { 0,0,4,4 };
				srand(static_cast<unsigned int>(time(0)));
				float index = static_cast<float>(rand() % (18) + 4);
				int x = static_cast<int>(pacMan.second.x + index);
				int y = static_cast<int>(pacMan.second.y);
				size[end]._dest = { x,y,4,4 };
				flash[end]._dv[0] = 0.0f;
				flash[end]._dv[1] = -25.0f;
				flash[end].dxy[0] = static_cast<float>(x);
				flash[end].dxy[1] = static_cast<float>(y);
				end++;
			}
			else if (kierPacMan == 3)
			{
				size[end]._source = { 0,0,4,4 };
				srand(static_cast<unsigned int>(time(0)));
				float index = static_cast<float>(rand() % (18) + 4);
				int x = static_cast<int>(pacMan.second.x + index);
				int y = static_cast<int>(pacMan.second.y + pacMan.second.h);
				size[end]._dest = { x,y,4,4 };
				flash[end]._dv[0] = 0.0f;
				flash[end]._dv[1] = 25.0f;
				flash[end].dxy[0] = static_cast<float>(x);
				flash[end].dxy[1] = static_cast<float>(y);
				end++;
			}
		}
		else
		{
			acumulator = 0.0f;
		}
	}
	else if (dvPacMan == 200.0f)
	{
		acumulator += dt;
		if (kierPacMan != 4 && acumulator >= 0.07f)
		{
			acumulator = 0.0f;
			if (dvPacMan == 200.0f)
			{
				acumulator += dt;
				if (kierPacMan == 0)
				{
					size[end]._source = { 0,0,4,4 };
					srand(static_cast<unsigned int>(time(0)));
					int x = static_cast<int>(pacMan.second.x);
					float index = static_cast<float>(rand() % (18) + 4);
					int y = static_cast<int>(pacMan.second.y + index);
					size[end]._dest = { x,y,4,4 };
					flash[end]._dv[0] = -25.0f;
					flash[end]._dv[1] = 0.0f;
					flash[end].dxy[0] = static_cast<float>(x);
					flash[end].dxy[1] = static_cast<float>(y);
					end++;
				}
				else if (kierPacMan == 1)
				{
					size[end]._source = { 0,0,4,4 };
					srand(static_cast<unsigned int>(time(0)));
					int x = static_cast<int>(pacMan.second.x + pacMan.second.w);
					float index = static_cast<float>(rand() % (18) + 4);
					int y = static_cast<int>(pacMan.second.y + index);
					size[end]._dest = { x,y,4,4 };
					flash[end]._dv[0] = 25.0f;
					flash[end]._dv[1] = 0.0f;
					flash[end].dxy[0] = static_cast<float>(x);
					flash[end].dxy[1] = static_cast<float>(y);
					end++;
				}
				else if (kierPacMan == 2)
				{
					size[end]._source = { 0,0,4,4 };
					srand(static_cast<unsigned int>(time(0)));
					float index = static_cast<float>(rand() % (18) + 4);
					int x = static_cast<int>(pacMan.second.x + index);
					int y = static_cast<int>(pacMan.second.y);
					size[end]._dest = { x,y,4,4 };
					flash[end]._dv[0] = 0.0f;
					flash[end]._dv[1] = -25.0f;
					flash[end].dxy[0] = static_cast<float>(x);
					flash[end].dxy[1] = static_cast<float>(y);
					end++;
				}
				else if (kierPacMan == 3)
				{
					size[end]._source = { 0,0,4,4 };
					srand(static_cast<unsigned int>(time(0)));
					float index = static_cast<float>(rand() % (18) + 4);
					int x = static_cast<int>(pacMan.second.x + index);
					int y = static_cast<int>(pacMan.second.y + pacMan.second.h);
					size[end]._dest = { x,y,4,4 };
					flash[end]._dv[0] = 0.0f;
					flash[end]._dv[1] = 25.0f;
					flash[end].dxy[0] = static_cast<float>(x);
					flash[end].dxy[1] = static_cast<float>(y);
					end++;
				}
			}
		}
	}
	unsigned int begin = front;
	for (unsigned int i = begin; i < end; i++)
	{
		flash[i]._dxTexture += flash[i]._dvTexture * dt;
		if (flash[i]._dxTexture <= 52.0f)
		{
			size[i]._source.x = static_cast<int>(flash[i]._dvTexture / 4) * 4;
			flash[i].dxy[0] += flash[i]._dv[0] * dt;
			flash[i].dxy[1] += flash[i]._dv[1] * dt;
			size[i]._dest.x = static_cast<int>(flash[i].dxy[0]);
			size[i]._dest.y = static_cast<int>(flash[i].dxy[1]);
		}
		else
		{
			size[i]._dest = size[i]._source = { 0,0,0,0 };
			flash[i]._dxTexture = 0.0f;
			flash[i].dxy[0] = flash[i].dxy[1] = 0.0f;
			flash[i]._dv[0] = flash[i]._dv[1] = 0.0f;
			front++;
		}
	}
	if (front == end)
	{
		front = end = 0;
	}
}