#pragma once
#include "Lives.h"
#include "BackGround.h"
#include "PacMan.h"
#include "MenuMenager.h"
#include "FlashTrack.h"
#include <string>
#define fpsLimiter 1

class TextTexture
{
private:
	const unsigned int _x, _y, _fontSize;
	FontsSize _fonts;
	FontTexture* _fontTexture;
	std::string _line;
public:
	TextTexture(std::string&& line, unsigned int&& fontsize,unsigned int&& x,unsigned int&& y) :_fonts{}, _line{ line }, _x{x}, _y{y}, _fontSize{fontsize}
	{
		_fontTexture = new FontTexture{ std::move(fontsize) };
	}
	void drawLine(SDL_Renderer* render, float& fps) noexcept;
	void drawLine(SDL_Renderer* render, int& level, int& count) noexcept;
	void loadTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept;
	void destroyTexture() noexcept;
	~TextTexture() 
	{
		delete _fontTexture;
	};
};

class MainGame
{
private:
	float _fps;
	Flags flag;
	SDL_Window* _window;
	uint32_t _screenWidth;
	uint32_t _screenHeight;
	std::string _title;
	std::unique_ptr<Live> _lives;
	std::unique_ptr<BoostMenager> _boostMenager;
	std::unique_ptr<PointMenager> _pointMenager;
	std::unique_ptr<GhostMenager> _ghostMenager;
	std::unique_ptr<PacManMenager> _pacManMenager;
	std::unique_ptr<MapMenager> _mapMenager;
	std::unique_ptr<MenuMenager> _menuMenager;
	std::unique_ptr<TextTexture> _fpsTexture;
	std::unique_ptr<TextTexture> _levelTexture;
	std::unique_ptr<FlashMenager> _flashMenager;
	SDL_Texture* _backScreen[3];
	SDL_Renderer* _render;
	void pError(std::string error) noexcept;
	void initSystem() noexcept;
	void exitSystem() noexcept;
	void gameLoop() noexcept;
	void getState(int* direction) noexcept;
	void clear() noexcept;
	void uRender() noexcept;
	void loadBackScreen() noexcept;
	void destryBackSreen() noexcept;
	void drawButtonOnBackSrceen() noexcept;
	void drawButtonBackScreen() noexcept;
	void drawPointOnBackScreen() noexcept;
	void drawPointBackScreen() noexcept;
	void drawLevelOnBackScreen() noexcept;
	void drawLevelBackScreen(int& level,int& count) noexcept;
	void getState() noexcept;
	void loadTexture() noexcept;
	void initResetSource() noexcept;
	void destroyTexture() noexcept;
	void getFPS(float& frametime) noexcept;
	void update(const float& desiredDT) noexcept;
	void draw() noexcept;
	void resetGame() noexcept;
public:
	MainGame(std::string n, uint32_t sW, uint32_t sH) :_menuMenager{std::make_unique<MenuMenager>()}, _mapMenager { std::make_unique<MapMenager>() },
													   _fpsTexture{std::make_unique<TextTexture>(std::string("0123456789."),20,5,615)},
													   _levelTexture{std::make_unique<TextTexture>(std::string("0123456789LEVELSTART "),200,120,100)},
													   _pacManMenager{ std::make_unique<PacManMenager>() }, _render{ nullptr }, _ghostMenager{ std::make_unique<GhostMenager>() }, 
													   _pointMenager{ std::make_unique<PointMenager>() }, _lives{ std::make_unique<Live>() }, _boostMenager{ std::make_unique<BoostMenager>() },
													   _title{ n }, _screenWidth{ sW }, _screenHeight{ sH }, _window{ nullptr }, _fps{ 0 }, flag{ 0 }, 
													   _flashMenager{ std::make_unique<FlashMenager>() }, _backScreen{ nullptr }{}
	~MainGame()
	{
		exitSystem();
	}
	void run() noexcept;
};

inline void TextTexture::drawLine(SDL_Renderer* render, float& fps) noexcept
{
	_line = std::to_string(fps);
	_fonts._dest.x = _x;
	_fonts._dest.y = _y;
	_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
}

inline void TextTexture::loadTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept
{
	_fontTexture->loadFontsToTexture(std::move(red), std::move(green), std::move(blue), render, _line);
}

inline void TextTexture::destroyTexture() noexcept
{
	_fontTexture->destroy();
}

inline void MainGame::pError(std::string error) noexcept
{
	std::cout << error << ":" << SDL_GetError() << std::endl;
	std::cout << "Press Any key to close" << std::endl;
}

inline void MainGame::clear() noexcept
{
	SDL_SetRenderDrawColor(_render, 255, 255, 255, 255);
	SDL_RenderClear(MainGame::_render);
}

inline void MainGame::uRender() noexcept
{
	SDL_RenderPresent(MainGame::_render);
}

inline void MainGame::loadBackScreen() noexcept
{
	for (auto i = 0; i < 3; i++)
	{
		_backScreen[i] = TextureMenager::createRenderTexture(_render, _screenWidth, _screenHeight);
	}
}

inline void MainGame::destryBackSreen() noexcept
{
	for (auto i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(_backScreen[i]);
	}
}

inline void MainGame::drawButtonOnBackSrceen() noexcept
{
	_menuMenager->drawButtonOnBackScreen(_render, _backScreen[1]);
}

inline void MainGame::drawButtonBackScreen() noexcept
{
	clear();
	TextureMenager::drawTexture(_render, _backScreen[1], nullptr, nullptr);
	if (flag._showFPS)
	{
		_fpsTexture->drawLine(_render, _fps);
	}
	uRender();
}

inline void MainGame::drawPointOnBackScreen() noexcept
{
	_menuMenager->drawPointsOnBackScreen(_render, _backScreen[2]);
}

inline void MainGame::drawPointBackScreen() noexcept
{
	clear();
	TextureMenager::drawTexture(_render, _backScreen[2], nullptr, nullptr);
	if (flag._showFPS)
	{
		_fpsTexture->drawLine(_render, _fps);
	}
	uRender();
}

inline void MainGame::drawLevelBackScreen(int& level,int& count) noexcept
{
	clear();
	TextureMenager::drawTexture(_render, _backScreen[0], nullptr, nullptr);
	_levelTexture->drawLine(_render, level, count);
	if (flag._showFPS)
	{
		_fpsTexture->drawLine(_render, _fps);
	}
	uRender();
}

inline void MainGame::update(const float& desiredDT) noexcept
{
	_mapMenager->operator()(desiredDT);
	_pacManMenager->mapCollision(MapLevel::_actualLevel, MapLevel::_tunelSize, desiredDT);
	_pointMenager->pointsCollision(_pacManMenager->getPacMan()[1].second, flag);
	_pointMenager->operator()(desiredDT);
	_boostMenager->operator()(_lives->getLives(), _pacManMenager->getPacManDV(), _pacManMenager->getPacManKill(), _pacManMenager->getPacMan()[1], desiredDT);
	_ghostMenager->MoveGhost(_pacManMenager->getPacManSource(), _lives->getLives(), desiredDT, MapLevel::_tunelSize);
	_pacManMenager->operator()(desiredDT);
	_pacManMenager->setSize();
	//
	_flashMenager->operator()(_pacManMenager->getPacManDV(), _pacManMenager->getPacMan()[1], _pacManMenager->getDirectionTexture(), desiredDT);
	//
}

inline void MainGame::draw() noexcept
{
	clear();
	_mapMenager->draw(_render);
#if drawAktPath
	ghost::drawVisited(_render, _ghostMenager->getPath());
	ghost::drawNode(_render, _ghostMenager->getPath());
#endif //drawAktPath
	_pointMenager->draw(_render);
	_boostMenager->draw(_render);
	//
	_flashMenager->draw(_render);
	//
	_ghostMenager->draw(_render);
	_pacManMenager->draw(_render);
	_lives->draw(_render, flag);
	if (flag._showFPS)
	{
		_fpsTexture->drawLine(_render, _fps);
	}
	uRender();
}

inline void MainGame::run() noexcept
{
	initSystem();
	gameLoop();
	_menuMenager->savePointsToFile();
	destroyTexture();
}