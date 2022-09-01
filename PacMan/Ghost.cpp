#include "Ghost.h"
#include <iostream>

void GhostPath::set_neighbours(int&& height,int&& width) noexcept
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (y > 0)
				nodes[y * width + x].neighbours.push_back(&nodes[(y - 1) * width + (x + 0)]);
			if (y < (height - 1))
				nodes[y * width + x].neighbours.push_back(&nodes[(y + 1) * width + (x + 0)]);
			if (x > 0)
				nodes[y * width + x].neighbours.push_back(&nodes[(y + 0) * width + (x - 1)]);
			if (x < (width - 1))
				nodes[y * width + x].neighbours.push_back(&nodes[(y + 0) * width + (x + 1)]);
		}
	}
}

void GhostPath::find_path(std::vector<SDL_FRect>& _pathsource, SDL_FRect& sghost) noexcept
{
	auto distance = [](sNode* a, sNode* b)
	{
		return sqrt(std::abs(a->sizeNode.x - b->sizeNode.x) * std::abs(a->sizeNode.x - b->sizeNode.x) + std::abs(a->sizeNode.y - b->sizeNode.y) * std::abs(a->sizeNode.y - b->sizeNode.y));
	};
	reset_nodes();
#if drawAktPath
	visited.clear();
	path.clear();
#endif //drawAktPath
	_pathsource.clear();
	sNode* nodeCurrent = start;
	start->LocalGoal = 0.0f;
	start->GlobalGoal = distance(start, end);
	std::priority_queue<sNode*, std::vector<sNode*>, std::greater<sNode*>> notTestedNodes;
	notTestedNodes.push(start);
	while (!notTestedNodes.empty() && nodeCurrent != end)
	{
		nodeCurrent = notTestedNodes.top();
		notTestedNodes.pop();
		nodeCurrent->visited = true;
		for (auto& nodeNeighbour : nodeCurrent->neighbours)
		{
			if (!nodeNeighbour->visited && !nodeNeighbour->wall)
			{
				notTestedNodes.push(nodeNeighbour);
			}
			float fPossiblyLowerGoal = nodeCurrent->LocalGoal + distance(nodeCurrent, nodeNeighbour);
			if (fPossiblyLowerGoal < nodeNeighbour->LocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->LocalGoal = fPossiblyLowerGoal;
				nodeNeighbour->GlobalGoal = nodeNeighbour->LocalGoal + distance(nodeNeighbour, end);
			}
		}
	}
	nodeCurrent = end;
	while (nodeCurrent != start && nodeCurrent != nullptr)
	{
		_pathsource.emplace_back(nodeCurrent->sizeNode);
#if drawAktPath
		path.emplace_back(nodeCurrent->pathSize);
#endif //drawAktPath
		nodeCurrent = nodeCurrent->parent;
	}
	if ((sghost.x != start->sizeNode.x || sghost.y != start->sizeNode.y) && !start->wall)
	{
		_pathsource.emplace_back(start->sizeNode);
#if drawAktPath
		path.emplace_back(start->pathSize);
#endif //drawAktPath
	}
#if drawAktPath
	for (auto& x : nodes)
	{
		if (x.visited)
		{
			visited.emplace_back(x.pathSize);
		}
	}
#endif //drawAktPath
}

void MoveGhost::nextPathAferPacMan() noexcept
{
	unsigned int size = path.size();
	if (size > 1)
	{
		SDL_FRect x = path[size - 1];
		SDL_FRect y = path[size - 2];
		if ((x.y == y.y) && (x.x - y.x) < 0.0f)
		{
			if (pKierunek < 2)
			{
				path.pop_back();
				nextPath = path.back();
				path.pop_back();
			}
		}
		else if ((x.x == y.x) && (x.y - y.y) < 0.0f)
		{
			if (pKierunek == 2 || pKierunek == 3)
			{
				path.pop_back();
				nextPath = path.back();
				path.pop_back();
			}
		}
	}
}

void MoveGhost::reset(GhostSize& ghost, GhostPath& _pathSource) noexcept
{
	acumulator = 0.0f;
	dv = 100.0f;
	dxGhost = 0.0f;
	pKierunek = 4;
	slPacman = false;
	numberEnd = 0;
	ghost._source = { 0, 128, 32, 32 };
	ghost._dest = ends[3];
	nextPath = ends[3];
	//init(_pathSource, ghost._dest);
}

//moveYellow
void MoveYellow::move1(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 10.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 15.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest,nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 10.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveYellow::move2(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 5.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 10.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 6.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveYellow::move3(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 8.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 16.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 4.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveYellow::move4(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 8.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 12.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		SDL_FRect actPacMan = _pacman._pacMan[1].second;
		if (kolizja(actPacMan, tunel._left) || kolizja(actPacMan, tunel._right))
		{
			actPacMan = tunel._right;
			actslPacMan = true;
			if (!(static_cast<int>(ghost._dest.x) % 2) && !(static_cast<int>(ghost._dest.y) % 2))
			{
				dv = 200.0f;
			}
		}
		else
		{
			dv = 100.0f;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 20.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}
//moveYellow
//moveRed
void MoveRed::move1(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	float vTexture = _dvTexture[0];
	int aktKier = { 4 };
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 0;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 10.0f)
		{
			actslPacMan = true;
			acumulator = 0.0f;
		}
		if (slPacman && (acumulator >= 15.0f) || _pacman._pacMan[1].first || (_pacman._pacManKill > 0.0f))
		{
			actslPacMan = false;
			acumulator = 0.0f;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
			//
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 15.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 0;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveRed::move2(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
			acumulator = 0.0f;
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 0;
		}
	}
	else if (pKierunek < 4)
	{
		auto actPacMan = _pacman._pacMan[1].second;
		if (acumulator == 0.0f)
		{
			if ((_pacman._pacManKill == 0.0f) && !_pacman._pacMan[1].first && distance(ghost._dest, actPacMan) <= 288.0f)
			{
				actslPacMan = true;
				acumulator += dt;
			}
		}
		else
		{
			if (acumulator <= 8.0f)
			{
				actslPacMan = true;
				acumulator += dt;
			}
			else if (acumulator <= 16.0f)
			{
				actslPacMan = false;
				acumulator += dt;
			}
			else
			{
				actslPacMan = false;
				acumulator = 0.0f;
			}
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
			//
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 9.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 0;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveRed::move3(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 10.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 15.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 8.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveRed::move4(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 10.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 15.5f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		SDL_FRect actPacMan = _pacman._pacMan[1].second;
		if (kolizja(actPacMan, tunel._left) || kolizja(actPacMan, tunel._right))
		{
			actPacMan = tunel._left;
			actslPacMan = true;
			if (!(static_cast<int>(ghost._dest.x) % 2) && !(static_cast<int>(ghost._dest.y) % 2))
			{
				dv = 200.0f;
			}
		}
		else
		{
			dv = 100.0f;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 15.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}
//moveRed
//movePink
void MovePink::move1(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		SDL_FRect actPacMan = _pacman._pacMan[1].second;
		auto dist = distance(ghost._dest, actPacMan);
		if (dist <= 96.0f)
		{
			actslPacMan = false;
		}
		else if (dist >= 416.0f)
		{
			actslPacMan = true;
		}
		else
		{
			actslPacMan = slPacman;
		}
		if (_pacman._pacMan[1].first || (_pacman._pacManKill > 0.0f))
		{
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
			//
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 20.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MovePink::move2(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		SDL_FRect actPacMan = _pacman._pacMan[1].second;
		auto dist = distance(ghost._dest, actPacMan);
		if (ghost._dest.x <= 192.0f && ghost._dest.y <= 196.0f)
		{
			actslPacMan = false;
		}
		else if (dist >= 416.0f)
		{
			actslPacMan = true;
		}
		else
		{
			actslPacMan = slPacman;
		}
		if (_pacman._pacMan[1].first || (_pacman._pacManKill > 0.0f))
		{
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
			//
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 12.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MovePink::move3(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 7.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 14.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 16.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MovePink::move4(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 0;
		}
	}
	else if (pKierunek < 4)
	{
		auto actPacMan = _pacman._pacMan[1].second;
		if ((_pacman._pacManKill == 0.0f) && !_pacman._pacMan[1].first && distance(ghost._dest, actPacMan) <= 288.0f)
		{
			actslPacMan = true;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
			dv = 50.0f;
		}
		else if (!actslPacMan == slPacman)
		{
			ghost._dest.x = std::trunc(ghost._dest.x);
			ghost._dest.y = std::trunc(ghost._dest.y);
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			dv = 100.0f;
		}
		//
		if (actslPacMan != slPacman)
		{
			nextPathAferPacMan();
		}
		//
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 7.5f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 0;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}
//movePink
//moveGreen
void MoveGreen::move1(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
		}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 0;
		}
	}
	else if (pKierunek < 4)
	{
		auto actPacMan = _pacman._pacMan[1].second;
		if ((_pacman._pacManKill == 0.0f) && !_pacman._pacMan[1].first && distance(ghost._dest, actPacMan) <= 288.0f)
		{
			actslPacMan = true;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
			dv = 50.0f;
		}
		else if (!actslPacMan == slPacman)
		{
			ghost._dest.x = std::trunc(ghost._dest.x);
			ghost._dest.y = std::trunc(ghost._dest.y);
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			dv = 100.0f;
		}
		//
		if (actslPacMan != slPacman) 
		{
			nextPathAferPacMan();
		}
		//
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 5.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 0;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveGreen::move2(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 8.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 16.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 6.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveGreen::move3(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan = slPacman;
	int aktKier{ 4 };
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		if (!slPacman && acumulator >= 8.0f)
		{
			acumulator = 0.0f;
			actslPacMan = true;
		}
		if ((slPacman && acumulator >= 12.0f) || _pacman._pacMan[1].first || _pacman._pacManKill > 0.0f)
		{
			acumulator = 0.0f;
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			SDL_FRect actPacMan = _pacman._pacMan[1].second;
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
		acumulator += dt;
	}
	else
	{
		if (acumulator <= 20.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f) % 18) * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}

void MoveGreen::move4(PacManSource& _pacman, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept
{
	bool actslPacMan{ false };
	int aktKier = 4;
	float vTexture = _dvTexture[0];
	if (kolizja(ghost._dest, _pacman._pacMan[1].second) && gKolizjiXY(ghost._dest, _pacman._pacMan[1].second) >= 96.0f)
	{
		if (_pacman._pacMan[1].first)
		{
			acumulator = 0.0f;
			ghost._dest = nextPath = ends[3];
			path.clear();
			numberEnd = 0;
			_showPoints[0] += 10;
			_showPoints[1] += 1;
			pKierunek = 5;
			dxGhost = 0.0f;
			actslPacMan = false;
}
		else if (_pacman._pacManKill == 0.0f)
		{
			lives._source.w -= 32;
			lives._dest.w -= 32;
			_pacman._directionS = 4;
			_pacman._pacMan[1] = _pacman._pacMan[0];
			_pacman._pacManKill = 5.0f;
			for (int i = 0; i < 2; i++)
			{
				_pacman._directionD[i] = 0;
			}
		}
	}
	if (pKierunek == 5)
	{
		if (dxGhost < 480.0f)
		{
			vTexture = _dvTexture[1];
			aktKier = 5;
		}
		else
		{
			aktKier = 1;
		}
	}
	else if (pKierunek < 4)
	{
		SDL_FRect actPacMan = _pacman._pacMan[1].second;
		auto dist = distance(ghost._dest, actPacMan);
		if (dist <= 96.0f)
		{
			actslPacMan = false;
		}
		else if (dist >= 416.0f)
		{
			actslPacMan = true;
		}
		else
		{
			actslPacMan = slPacman;
		}
		if (_pacman._pacMan[1].first || (_pacman._pacManKill > 0.0f))
		{
			actslPacMan = false;
		}
		if (actslPacMan)
		{
			if (!(static_cast<int>(actPacMan.x) % 32) && !(static_cast<int>(actPacMan.y) % 32))
			{
				actPacMan.x += 1.0f;
				actPacMan.y += 1.0f;
			}
			find_path_to_pacman(_pathSource, ghost._dest, actPacMan);
		}
		else if (actslPacMan != slPacman)
		{
			unsigned int nr = numberEnd % ends.size();
			if (kolizja(ghost._dest, ends[nr])) ++numberEnd;
			init(_pathSource, ghost._dest);
			//
			nextPathAferPacMan();
			//
		}
		if (AreSame(ghost._dest, nextPath))
		{
			get_nextPath(_pathSource, ghost._dest);
		}
		auto xGhost = ghost._dest.x;
		auto yGhost = ghost._dest.y;
		if (xGhost != nextPath.x)
		{
			if (xGhost < nextPath.x)
			{
				ghost._dest.x += dv * dt;
				aktKier = 0;
			}
			else
			{
				ghost._dest.x -= dv * dt;
				aktKier = 1;
			}
		}
		else
		{
			if (yGhost < nextPath.y)
			{
				ghost._dest.y += dv * dt;
				aktKier = 3;
			}
			else
			{
				ghost._dest.y -= dv * dt;
				aktKier = 2;
			}
		}
	}
	else
	{
		if (acumulator <= 8.0f)
		{
			acumulator += dt;
		}
		else
		{
			acumulator = 0.0f;
			aktKier = 1;
			actslPacMan = false;
		}
	}
	if (aktKier != pKierunek)
	{
		dxGhost = 0.0f;
		pKierunek = aktKier;
	}
	else dxGhost += vTexture * dt;
	ghost._source.x = (static_cast<int>(dxGhost / 32.0f)) % 18 * 32;
	ghost._source.y = pKierunek * 32;
	slPacman = actslPacMan;
}
//moveGreen

#if drawAktPath
namespace ghost
{
	SDL_Texture* nodeT0 = nullptr;
	SDL_Texture* visitedT0 = nullptr;
	SDL_Texture* nodeT1 = nullptr;
	SDL_Texture* visitedT1 = nullptr;
	SDL_Texture* nodeT2 = nullptr;
	SDL_Texture* visitedT2 = nullptr;
	SDL_Texture* nodeT3 = nullptr;
	SDL_Texture* visitedT3 = nullptr;
	SDL_Rect _nodeS0{ 0,0,32,32 };
	
}
#endif //drawAktPath