#pragma once
#include "TextureMenager.h"
#include <limits>
#include <queue>
#include <functional>
#include "Lives.h"
#include <iostream>
#include "Collision.h"
#include "PacManSource.h"
#include "TunelSize.h"
#define drawAktPath 0

struct sNode
{
	SDL_FRect sizeNode;
#if drawAktPath
	SDL_Rect pathSize{ 0,0,32,32 };
#endif //drawAktPath
	bool wall;
	bool visited;
	float GlobalGoal;
	float LocalGoal;
	std::vector<sNode*> neighbours;
	sNode* parent;
	sNode() noexcept : sizeNode{ 0.0f,0.0f,32.0f,32.0f }, wall{ false }, visited{ false }, GlobalGoal{ std::numeric_limits<float>::max() }, LocalGoal{ std::numeric_limits<float>::max() }, 
					   neighbours{ std::vector<sNode*>() }, parent{ nullptr }{}
};

template <>
struct std::greater<sNode*>
{
	constexpr bool operator()(const sNode* _Left, const sNode* _Right) const
	{
		return _Left->GlobalGoal > _Right->GlobalGoal;
	}
};

class GhostPath
{
	std::vector<sNode> nodes;
	int _width;
#if drawAktPath
	std::vector<SDL_Rect> path;
	std::vector<SDL_Rect> visited;
#endif //drawAktPath
	sNode* start;
	sNode* end;
public:
	GhostPath() noexcept : nodes{ std::vector<sNode>() }, _width{}, start{ nullptr }, end{ nullptr }{};
	void clear() noexcept;
	void push_back(int&& x, int&& y, bool&& wall) noexcept;
	template <typename A>
	void set_start(const A& source) noexcept
	{
		auto x = source.x + 32.0f;
		auto y = source.y + 32.0f;
		const int xx = static_cast<int>(std::trunc(x / 32.0f));
		const int yy = static_cast<int>(std::trunc(y / 32.0f));
		start = &nodes[yy * _width + xx];
	}
	template<rect A>
	void set_end(const A& source) noexcept
	{
		auto x = source.x + 32.0f;
		auto y = source.y + 32.0f;
		const int xx = static_cast<int>(std::trunc(x / 32.0f));
		const int yy = static_cast<int>(std::trunc(y / 32.0f));
		end = &nodes[yy * _width + xx];
	}
	void set_neighbours(int&& height, int&& width) noexcept;
	void find_path(std::vector<SDL_FRect>& _pathsource, SDL_FRect& sghost) noexcept;
	void reset_nodes() noexcept;
	void resize_nodes(int&& height,int&& width) noexcept;
#if drawAktPath
	std::vector<SDL_Rect>& get_path() noexcept;
	std::vector<SDL_Rect>& get_visited() noexcept;
#endif //drawAktPath
};

struct GhostSize
{
	SDL_Rect _source;
	SDL_FRect _dest;
	GhostSize() :_source{ 0,128,32,32 }, _dest{ 0.0f,0.0f,32.0f,32.0f }{}
	void clear() noexcept;
};

struct MoveGhost
{
	float acumulator;
	const float _dvTexture[2];
	float dxGhost;
	float dv;
	unsigned int pKierunek;
	bool slPacman;
	std::vector<SDL_FRect> path;
	std::vector<SDL_FRect> ends;
	SDL_FRect nextPath;
	unsigned int numberEnd;
	MoveGhost() noexcept : acumulator{ 0.0f }, _dvTexture{ 32.0f / 0.04f, 64.0f }, dxGhost{ 0.0f }, dv{ 100.0f }, pKierunek{ 4 }, slPacman{ false }, path{ std::vector<SDL_FRect>() },
						   ends{ std::vector<SDL_FRect>() }, nextPath{}, numberEnd{ 0 }{}
	virtual ~MoveGhost() {}
	template <rect A, rect B>
	auto distance(A& _left, B& _right)
	{
		return sqrt(std::abs(_left.x - _right.x) * std::abs(_left.x - _right.x) + std::abs(_left.y - _right.y) * std::abs(_left.y - _right.y));
	}
	void clear() noexcept;
	template <rect A, rect B>
	void find_path_to_pacman(GhostPath& _pathSource, A& gdest, B& pacman) noexcept
	{
		_pathSource.set_start(gdest);
		_pathSource.set_end(pacman);
		_pathSource.find_path(path, gdest);
	}
	void nextPathAferPacMan() noexcept;
	void init(GhostPath& _pathSource, SDL_FRect& ghost) noexcept;
	void init(GhostPath& _pathSource, SDL_FRect& ghost, unsigned int&& nrkier) noexcept;
	void push_backEnds(int&& x, int&& y, int&& ktory) noexcept;
	void get_nextPath(GhostPath& _pathSource, SDL_FRect& ghost) noexcept;
	void resize_ends(int&& ile) noexcept;
	void reset(GhostSize& gdest, GhostPath& _pathSource) noexcept;
	virtual void move1(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives,const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept = 0;
	virtual void move2(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept = 0;
	virtual void move3(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept = 0;
	virtual void move4(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept = 0;
};

//moveYellow
struct MoveYellow :MoveGhost
{
	void move1(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move2(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move3(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move4(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
};
//moveYellow
//moveRed
struct MoveRed :MoveGhost
{
	void move1(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move2(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move3(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move4(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
};
//moveRed
//movePink
struct MovePink :MoveGhost
{
	void move1(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move2(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move3(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move4(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
};
//movePink
//moveGreen
struct MoveGreen :MoveGhost
{
	void move1(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move2(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move3(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
	void move4(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel) noexcept override;
};
//moveGreen

class GhostTexture
{
private:
	SDL_Texture* ghostT;
	SDL_Rect _dest{};
public:
	GhostTexture() :ghostT{ nullptr }, _dest{ 0,0,32,32 }{}
	void destroyTexture() noexcept;
	void loadTexture(SDL_Renderer* render, const char* sourceGhost) noexcept;
	void draw(SDL_Renderer* render, SDL_Rect& _source) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect& _source) noexcept;
	void setSize(const GhostSize& source) noexcept;
};

class GhostMenager
{
private:
	std::vector<GhostTexture> _ghostTexture;
	std::vector<GhostPath> _ghostPaths;
	std::vector<GhostPath> _ghostPathsSource;
	std::vector<std::vector<SDL_FRect>> _ghostEndsSource;
	GhostSize _ghostSize[4];
	unsigned int kills[2];
	MoveGhost* _moveGhost[4];
	void initPathSource() noexcept;
	void initEndSource() noexcept;
	void (MoveGhost::* moveGhost)(PacManSource& _pacMan, GhostPath& _pathSource, GhostSize& ghost, LiveSize& lives, const float& dt, unsigned int* _showPoints, const TunelSize& tunel);

public:
	GhostMenager() noexcept : _ghostTexture{ std::vector<GhostTexture>(4,GhostTexture{}) }, _ghostPaths{ std::vector<GhostPath>(4, GhostPath()) }, 
							  _ghostPathsSource{ std::vector<GhostPath>(4, GhostPath()) }, _ghostEndsSource{ std::vector<std::vector<SDL_FRect>>(4,std::vector<SDL_FRect>(4,{0.0f,0.0f,32.0f,32.0f})) }, 
							  _ghostSize{}, kills{ {0},{0} }
	{
		_moveGhost[0] = new MoveRed{};
		_moveGhost[1] = new MoveGreen{};
		_moveGhost[2] = new MoveYellow{};
		_moveGhost[3] = new MovePink;
	}
	~GhostMenager()
	{
		for (auto& x : _moveGhost)
		{
			delete x;
		}
	}
	void clear() noexcept;
#if drawAktPath
	std::vector<GhostPath>& getPath() noexcept;
#endif //drawAktPath
	void loadTexture(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render) noexcept;
	void draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void destroyTexture() noexcept;
	const unsigned int* getPoints() noexcept;
	void MoveGhost(PacManSource& pacMan, LiveSize& lives, const float& dt, const TunelSize& tunel) noexcept;
	void resetGhost() noexcept;
	void initAllGhost() noexcept;
	void resizeAllEnds(int&& x) noexcept;
	void pushBackPath(int&& x, int&& y, bool&& wall) noexcept;
	void resizeAllNodesPath(int&& height,int&& width) noexcept;
	void pushBackGhostEnd(int&& x, int&& y, int&& ghost, int&& end) noexcept;
	void set_neighboursAllPath(int&& height, int&& width) noexcept;
	void initResetSource() noexcept;
	void setFunction(const int& level) noexcept;
};

inline void GhostPath::clear() noexcept
{
	nodes.clear();
	start = end = nullptr;
#if drawAktPath
	path.clear();
	visited.clear();
#endif //drawAktPath
}

inline void GhostPath::push_back(int&& x, int&& y, bool&& wall) noexcept
{
	nodes[(y + 1) * _width + x + 1].wall = wall;
	nodes[(y + 1) * _width + x + 1].sizeNode.x = static_cast<float>(x) * 32.0f;
	nodes[(y + 1) * _width + x + 1].sizeNode.y = static_cast<float>(y) * 32.0f;
#if drawAktPath
	nodes[(y + 1) * _width + x + 1].pathSize.x = x * 32;
	nodes[(y + 1) * _width + x + 1].pathSize.y = y * 32;
#endif //drawAktPath
}

inline void GhostPath::reset_nodes() noexcept
{
	for (auto& x : nodes)
	{
		x.visited = false;
		x.LocalGoal = std::numeric_limits<float>::max();
		x.GlobalGoal = std::numeric_limits<float>::max();
		x.parent = nullptr;
	}
}

inline void GhostPath::resize_nodes(int&& height, int&& width) noexcept
{
	_width = width;
	nodes.resize(height * width, sNode());
}

#if drawAktPath
inline std::vector<SDL_Rect>& GhostPath::get_path() noexcept
{
	return path;
}

inline std::vector<SDL_Rect>& GhostPath::get_visited() noexcept
{
	return visited;
}
#endif //drawAktPath

inline void GhostSize::clear() noexcept
{
	_source = { 0,128,32,32 };
	_dest = { 0.0f,0.0f,32.0f,32.0f };
}

inline void MoveGhost::clear() noexcept
{
	path.clear();
	ends.clear();
	numberEnd = 0;
	dxGhost = 0.0f;
	dv = 100.0f;
	pKierunek = 4;
	acumulator = 0.0f;
	slPacman = false;
}

inline void MoveGhost::init(GhostPath& _pathSource, SDL_FRect& ghost) noexcept
{
	_pathSource.set_start(ghost);
	unsigned int nr = numberEnd % ends.size();
	_pathSource.set_end(ends[nr]);
	++numberEnd;
	_pathSource.find_path(path, ghost);
}

inline void MoveGhost::init(GhostPath& _pathSource, SDL_FRect& ghost, unsigned int&& nrkier) noexcept
{
	_pathSource.set_start(ghost);
	_pathSource.set_end(ends[nrkier]);
	_pathSource.find_path(path, ghost);
	numberEnd = ++nrkier;
}

inline void MoveGhost::push_backEnds(int&& x, int&& y, int&& ktory)noexcept
{
	ends[ktory].x = static_cast<float>(x) * 32.0f;
	ends[ktory].y = static_cast<float>(y) * 32.0f;
}

inline void MoveGhost::get_nextPath(GhostPath& _pathSource, SDL_FRect& ghost) noexcept
{
	if (!path.empty())
	{
		nextPath = path.back();
		path.pop_back();
	}
	else
	{
		init(_pathSource, nextPath);
		nextPath = path.back();
		path.pop_back();
	}
}

inline void MoveGhost::resize_ends(int&& ile) noexcept
{
	ends.resize(ile, SDL_FRect{ 0.0f,0.0f,32.0f,32.0f });
}

inline void GhostTexture::destroyTexture() noexcept
{
	SDL_DestroyTexture(ghostT);
}

inline void GhostTexture::loadTexture(SDL_Renderer* render, const char* sourceGhost) noexcept
{
	ghostT = TextureMenager::loadTexture(render, sourceGhost);
}

inline void GhostTexture::draw(SDL_Renderer* render, SDL_Rect& _source) noexcept
{
	TextureMenager::drawTexture(render, ghostT, &_source, &_dest);
}

inline void GhostTexture::draw(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect& _source) noexcept
{
	TextureMenager::drawTextureOnRenderTexture(render, ghostT, destTexture, &_source, &_dest);
}

inline void GhostTexture::setSize(const GhostSize& source) noexcept
{
	_dest.x = static_cast<int>(source._dest.x);
	_dest.y = static_cast<int>(source._dest.y);
	_dest.w = static_cast<int>(source._dest.w);
	_dest.h = static_cast<int>(source._dest.h);
}

inline void GhostMenager::initPathSource() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_ghostPathsSource[i] = _ghostPaths[i];
	}
}

inline void GhostMenager::initEndSource() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_ghostEndsSource[i] = _moveGhost[i]->ends;
	}
}

inline void GhostMenager::clear() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_ghostSize[i].clear();
		_moveGhost[i]->clear();
		_ghostPaths[i].clear();
	}
}

#if drawAktPath
inline std::vector<GhostPath>& GhostMenager::getPath() noexcept
{
	return _ghostPaths;
}
#endif //drawAktPath

inline void GhostMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_ghostTexture[0].loadTexture(render, "assets/image/ghostred.png");
	_ghostTexture[1].loadTexture(render, "assets/image/ghostgreen.png");
	_ghostTexture[2].loadTexture(render, "assets/image/ghostyellow.png");
	_ghostTexture[3].loadTexture(render, "assets/image/ghostpink.png");
}

inline void GhostMenager::draw(SDL_Renderer* render) noexcept
{
	for (size_t i = 0; i < 4; i++)
	{
		_ghostTexture[i].draw(render, _ghostSize[i]._source);
	}
}

inline void GhostMenager::draw(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_ghostTexture[i].draw(render, destTexture, _ghostSize[i]._source);
	}
}

inline void GhostMenager::destroyTexture() noexcept
{
	for (auto& x : _ghostTexture)
	{
		x.destroyTexture();
	}
}

inline const unsigned int* GhostMenager::getPoints() noexcept
{
	return kills;
}

inline void GhostMenager::MoveGhost(PacManSource& pacMan, LiveSize& lives, const float& dt, const TunelSize& tunel) noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		(_moveGhost[i]->*moveGhost)(pacMan, _ghostPaths[i], _ghostSize[i], lives, dt, kills, tunel);
		_ghostTexture[i].setSize(_ghostSize[i]);
	}
}

inline void GhostMenager::resetGhost() noexcept
{
	for (auto i = 0; i < 4; i++)
	{
		_ghostPaths[i] = _ghostPathsSource[i];
		_moveGhost[i]->ends = _ghostEndsSource[i];
		_moveGhost[i]->reset(_ghostSize[i], _ghostPaths[i]);
		_ghostTexture[i].setSize(_ghostSize[i]);
	}
	kills[0] = 0;
	kills[1] = 0;
}

inline void GhostMenager::initAllGhost() noexcept
{
	for (unsigned int i = 0; i < 4; i++)
	{
		_moveGhost[i]->init(_ghostPaths[i], _ghostSize[i]._dest);
		_moveGhost[i]->get_nextPath(_ghostPaths[i], _ghostSize[i]._dest);
		_ghostTexture[i].setSize(_ghostSize[i]);
	}
}

inline void GhostMenager::resizeAllEnds(int&& x) noexcept
{
	for (auto i = 0; i < 4; i++)
	{
		_moveGhost[i]->resize_ends(std::move(x));
	}
}

inline void GhostMenager::pushBackPath(int&& x, int&& y, bool&& wall) noexcept
{
	for (auto& i : _ghostPaths)
	{
		i.push_back(std::move(x), std::move(y), std::move(wall));
	}
}

inline void GhostMenager::resizeAllNodesPath(int&& height, int&& width) noexcept
{
	for (auto& i : _ghostPaths)
	{
		i.resize_nodes(std::move(height), std::move(width));
	}
}

inline void GhostMenager::pushBackGhostEnd(int&& x, int&& y, int&& ghost, int&& end) noexcept
{
	_moveGhost[ghost]->push_backEnds(std::move(x), std::move(y), std::move(end));
	if (end == 3)
	{
		_ghostSize[ghost]._dest.x = static_cast<float>(x) * 32.0f;
		_ghostSize[ghost]._dest.y = static_cast<float>(y) * 32.0f;
		_ghostTexture[ghost].setSize(_ghostSize[ghost]);
	}
}

inline void GhostMenager::set_neighboursAllPath(int&& height, int&& width) noexcept
{
	for (auto& x : _ghostPaths)
	{
		x.set_neighbours(std::move(height), std::move(width));
	}
}

inline void GhostMenager::initResetSource() noexcept
{
	initPathSource();
	initEndSource();
}

inline void GhostMenager::setFunction(const int& level) noexcept
{
	int size = (level - 1) % 4;
	switch (size)
	{
		case 0:
		{
			moveGhost = &MoveGhost::move1;
			break;
		}
		case 1:
		{
			moveGhost = &MoveGhost::move2;
			break;
		}
		case 2:
		{
			moveGhost = &MoveGhost::move3;
			break;
		}
		case 3:
		{
			moveGhost = &MoveGhost::move4;
			break;
		}
		default:
		{
			break;
		}
	}
}




#if drawAktPath
namespace ghost
{
	extern SDL_Texture* nodeT0;
	extern SDL_Texture* visitedT0;
	extern SDL_Texture* nodeT1;
	extern SDL_Texture* visitedT1;
	extern SDL_Texture* nodeT2;
	extern SDL_Texture* visitedT2;
	extern SDL_Texture* nodeT3;
	extern SDL_Texture* visitedT3;
	extern SDL_Rect _nodeS0;

	inline void loadNode(SDL_Renderer* render)
	{
		ghost::nodeT0 = TextureMenager::loadTexture(render,"assets/image/node0.png");
		ghost::visitedT0 = TextureMenager::loadTexture(render,"assets/image/visited0.png");
		ghost::nodeT1 = TextureMenager::loadTexture(render,"assets/image/node1.png");
		ghost::visitedT1 = TextureMenager::loadTexture(render,"assets/image/visited1.png");
		ghost::nodeT2 = TextureMenager::loadTexture(render,"assets/image/node2.png");
		ghost::visitedT2 = TextureMenager::loadTexture(render,"assets/image/visited2.png");
		ghost::nodeT3 = TextureMenager::loadTexture(render,"assets/image/node3.png");
		ghost::visitedT3 = TextureMenager::loadTexture(render,"assets/image/visited3.png");
	}

	inline void destroyTexture()
	{
		SDL_DestroyTexture(ghost::nodeT0);
		SDL_DestroyTexture(ghost::visitedT0);
		SDL_DestroyTexture(ghost::nodeT1);
		SDL_DestroyTexture(ghost::visitedT1);
		SDL_DestroyTexture(ghost::nodeT2);
		SDL_DestroyTexture(ghost::visitedT2);
		SDL_DestroyTexture(ghost::nodeT3);
		SDL_DestroyTexture(ghost::visitedT3);
	}

	inline void drawNode(SDL_Renderer* render,std::vector<GhostPath>& _ghostPaths)
	{
		TextureMenager::drawVectorRect(render,ghost::nodeT0, &ghost::_nodeS0, _ghostPaths[0].get_path());
		TextureMenager::drawVectorRect(render,ghost::nodeT1, &ghost::_nodeS0, _ghostPaths[1].get_path());
		TextureMenager::drawVectorRect(render,ghost::nodeT2, &ghost::_nodeS0, _ghostPaths[2].get_path());
		TextureMenager::drawVectorRect(render,ghost::nodeT3, &ghost::_nodeS0, _ghostPaths[3].get_path());
	}

	inline void drawVisited(SDL_Renderer* render, std::vector<GhostPath>& _ghostPaths)
	{
		TextureMenager::drawVectorRect(render,ghost::visitedT0, &ghost::_nodeS0, _ghostPaths[0].get_visited());
		TextureMenager::drawVectorRect(render,ghost::visitedT1, &ghost::_nodeS0, _ghostPaths[1].get_visited());
		TextureMenager::drawVectorRect(render,ghost::visitedT2, &ghost::_nodeS0, _ghostPaths[2].get_visited());
		TextureMenager::drawVectorRect(render,ghost::visitedT3, &ghost::_nodeS0, _ghostPaths[3].get_visited());
	}
}
#endif //drawAktPath

