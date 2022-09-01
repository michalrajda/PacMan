#include "MainGame.h"

void TextTexture::drawLine(SDL_Renderer* render, int& level, int& count) noexcept
{
	_line = "LEVEL " + std::to_string(level);
	_fonts._dest.x = _x;
	_fonts._dest.y = _y;
	_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
	if (count)
	{
		_line = std::to_string(count);
		_fonts._dest.x = _x + static_cast<int>(1.3f * _fontSize);
		_fonts._dest.y = _y + _fontSize;
		_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
	}
	else
	{
		_line = "START";
		_fonts._dest.x = _x + static_cast<int>(0.3f * _fontSize);
		_fonts._dest.y = _y + _fontSize;
		_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
	}
}

void MainGame::initSystem() noexcept
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		flag._isInitSDL = true;
		std::cout << "SDL zainicjowany:" << std::boolalpha << flag._isInitSDL << std::endl;
		if (_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_SHOWN))
		{
			flag._isWindow = true;
			std::cout << "Zainicjowano okno:" << flag._isWindow << std::endl;
			if (_render = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED))
			{
				flag._isRender = true;
				std::cout << "Zainicjowano render:" << flag._isRender << std::endl;
				SDL_SetRenderDrawColor(_render, 255, 255, 255, 255);
				if (IMG_Init(IMG_INIT_PNG))
				{
					flag._isImgInit = true;
					std::cout << "Zainicjowano Img_Init:" << flag._isImgInit << std::endl;
				}
				else
				{
					pError("Img_init - false:");
					exitSystem();
				}
			}
			else
			{
				pError("Blad inicjalizacji rendera");
				exitSystem();
			}
		}
		else
		{
			pError("Blad inicjalizacji okna");
			exitSystem();
		}
	}
	else
	{
		pError("Blad inicjalizacji SDL");
		exitSystem();
	}
	if (flag._isInitSDL && flag._isWindow && flag._isRender && flag._isImgInit)
	{
		flag._gameState = true;
		std::cout << "Inicjalizacja wszystkich systemow:" << flag._gameState << std::endl;
		std::cout << std::noboolalpha;
	loadTexture();
	_mapMenager->loadMap(0 ,MapLevel::level1, *_pacManMenager, *_pointMenager, *_boostMenager, *_ghostMenager);
	initResetSource();
	_menuMenager->loadPointsFromFile();
	drawButtonOnBackSrceen();
	drawPointOnBackScreen();
	}
}

void MainGame::exitSystem() noexcept
{
	if (flag._isRender)
	{
		SDL_DestroyRenderer(_render);
		std::cout << "Render zniszczony" << std::endl;
		flag._isRender = false;
	}
	if (flag._isWindow)
	{
		SDL_DestroyWindow(_window);
		std::cout << "Okno zniszczone" << std::endl;
		flag._isWindow = false;
	}
	if (flag._isImgInit)
	{
		IMG_Quit();
		std::cout << "IMG Quit" << std::endl;
		flag._isImgInit = false;
	}
	if (flag._isInitSDL)
	{
		SDL_Quit();
		std::cout << "SDL Quit" << std::endl;
		flag._isInitSDL = false;
	}
	std::cout << "Press ENTER to EXIT";
	std::cin.clear();
	std::cin.get();
}

void MainGame::gameLoop() noexcept
{
	float startTick{ 0.0f };
	float frameTime{ 0.0f };
	float lastTick{ 0.0f };
	float _acumulatorLevel = 0.0f;
	int count{ 5 };
	int _level = 1;
	static const float _maxFps{ 60.0f };
	static const float _ms{ 1000.0f };
	static const float _desiredFrameTime = /*_ms / _maxFps*/ 10.0f;
	static const float _desiredDT{ _desiredFrameTime / _ms };
#if !fpsLimiter
	float acumulator{ 0.0f };
#endif // !fpsLimiter
	float delay{ 0.0f };
	while (flag._gameState)
	{
		startTick = static_cast<float>(SDL_GetTicks());
		frameTime = startTick - lastTick;
		lastTick = startTick;
		getFPS(frameTime);
		_menuMenager->choseMenu(flag);
		drawButtonBackScreen();
		while (flag._showPoints)
		{
			startTick = static_cast<float>(SDL_GetTicks());
			frameTime = startTick - lastTick;
			lastTick = startTick;
			getFPS(frameTime);
			_menuMenager->returnToMenu(flag);
			drawPointBackScreen();
			delay = _desiredFrameTime - startTick + static_cast<float>(SDL_GetTicks());
			if (delay > 0.0f)
			{
				SDL_Delay(static_cast<uint32_t>(delay));
			}
		}
		while (flag._isLives)
		{
			count = 5;
			_pacManMenager->setSize();
			_boostMenager->initBoostComponent();
			_ghostMenager->initAllGhost();
			_ghostMenager->setFunction(_level);
			drawLevelOnBackScreen();
			lastTick = static_cast<float>(SDL_GetTicks());
			while ((count > -1) && flag._nextLevel)
			{
				startTick = static_cast<float>(SDL_GetTicks());
				frameTime = startTick - lastTick;
				_acumulatorLevel += frameTime;
				lastTick = startTick;
				getFPS(frameTime);
				getState();
				drawLevelBackScreen(_level,count);
				if (_acumulatorLevel >= 1000.0f)
				{
					_acumulatorLevel = 0.0f;
					count--;
				}
				delay = _desiredFrameTime - startTick + static_cast<float>(SDL_GetTicks());
				if (delay > 0.0f)
				{
					SDL_Delay(static_cast<uint32_t>(delay));
				}
			}
			while (flag._actLevel)
			{
				startTick = static_cast<float>(SDL_GetTicks());
				frameTime = startTick - lastTick;
				lastTick = startTick;
#if !fpsLimiter
				acumulator += frameTime / _ms;
#endif // !fpsLimiter
				getFPS(frameTime);
				getState(_pacManMenager->getDirection());
#if !fpsLimiter
				while (acumulator > _desiredDT)
				{
#endif //!fpsLimiter
					update(_desiredDT);
#if !fpsLimiter
					acumulator -= _desiredDT;
				}
#endif //!fpsLimiter
				draw();
#if fpsLimiter
				delay = _desiredFrameTime - startTick + static_cast<float>(SDL_GetTicks());
				if (delay > 0.0f)
				{
					SDL_Delay(static_cast<uint32_t>(delay));
				}
#endif //fpsLimiter
			}
			if (flag._nextLevel)
			{
				_mapMenager->loadNextLevel(_level, *_pacManMenager, *_pointMenager, *_boostMenager, *_ghostMenager, flag);
				_level++;
			}
		}
		if (flag._reset)
		{
#if !fpsLimiter
			acumulator = 0.0f;
#endif // !fpsLimiter
			resetGame();
			_level = 1;
			while (flag._getname)
			{
				startTick = static_cast<float>(SDL_GetTicks());
				frameTime = startTick - lastTick;
				lastTick = startTick;
				getFPS(frameTime);
				clear();
				_menuMenager->drawName(_render, flag);
				if (flag._showFPS)
				{
					_fpsTexture->drawLine(_render, _fps);
				}
				uRender();
				delay = _desiredFrameTime - startTick + static_cast<float>(SDL_GetTicks());
				if (delay > 0.0f)
				{
					SDL_Delay(static_cast<uint32_t>(delay));
				}
			}
			if (flag._saveName)
			{
				_menuMenager->saveActualPoints();
				drawPointOnBackScreen();
				flag._saveName = false;
			}
		}
		delay = _desiredFrameTime - startTick + static_cast<float>(SDL_GetTicks());
		if (delay > 0.0f)
		{
			SDL_Delay(static_cast<uint32_t>(delay));
		}
	}
}

void MainGame::getState(int* direction) noexcept
{
	SDL_Event _evnt;
	while (SDL_PollEvent(&_evnt))
	{
		switch (_evnt.type)
		{
		case SDL_KEYDOWN:
		{
			switch (_evnt.key.keysym.sym)
			{
			case SDLK_w:
			{
				direction[0] = 0;
				direction[1] = -1;
				break;
			}
			case SDLK_s:
			{
				direction[0] = 0;
				direction[1] = 1;
				break;
			}
			case SDLK_d:
			{
				direction[0] = 1;
				direction[1] = 0;
				break;
			}
			case SDLK_a:
			{
				direction[0] = -1;
				direction[1] = 0;
				break;
			}
			default:
			{
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (_evnt.key.keysym.sym)
			{
			case SDLK_PAGEUP:
			{
				flag._showFPS = true;
				break;
			}
			case SDLK_PAGEDOWN:
			{
				flag._showFPS = false;
				break;
			}
			case SDLK_w:
			{
				direction[1] = 0;
				break;
			}
			case SDLK_s:
			{
				direction[1] = 0;
				break;
			}
			case SDLK_d:
			{
				direction[0] = 0;
				break;
			}
			case SDLK_a:
			{
				direction[0] = 0;
				break;
			}
			case SDLK_ESCAPE:
			{
				flag._isLives = false;
				flag._nextLevel = false;
				flag._actLevel = false;
				flag._reset = true;
				break;
			}
			default:
			{
				break;
			}
			}
			break;
		}
		case SDL_QUIT:
		{
			flag._isLives = false;
			flag._actLevel = false;
			flag._nextLevel = false;
			flag._gameState = false;
			flag._showPoints = false;
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void MainGame::drawLevelOnBackScreen() noexcept
{
	_mapMenager->draw(_render, _backScreen[0]);
	_lives->draw(_render, _backScreen[0]);
	_pointMenager->draw(_render, _backScreen[0]);
	_pacManMenager->draw(_render, _backScreen[0]);
	_ghostMenager->draw(_render, _backScreen[0]);
}

void MainGame::getState() noexcept
{
	SDL_Event _evnt;
	while (SDL_PollEvent(&_evnt))
	{
		switch (_evnt.type)
		{
		case SDL_KEYUP:
		{
			switch (_evnt.key.keysym.sym)
			{
			case SDLK_PAGEUP:
			{
				flag._showFPS = true;
				break;
			}
			case SDLK_PAGEDOWN:
			{
				flag._showFPS = false;
				break;
			}
			case SDLK_RETURN:
			{
				flag._nextLevel = false;
				break;
			}
			case SDLK_ESCAPE:
			{
				flag._isLives = false;
				flag._nextLevel = false;
				flag._actLevel = false;
				flag._reset = true;
				break;
			}
			default:
			{
				break;
			}
			}
			break;
		}
		case SDL_QUIT:
		{
			flag._isLives = false;
			flag._actLevel = false;
			flag._nextLevel = false;
			flag._gameState = false;
			flag._showPoints = false;
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void MainGame::loadTexture() noexcept
{
	loadBackScreen();
	//
	_flashMenager->loadTexture(_render);
	//
	_mapMenager->loadTexture(_render);
	_pacManMenager->loadTexture(_render);
	_pointMenager->loadTexture(_render);
	_boostMenager->loadTexture(_render);
	_ghostMenager->loadTexture(_render);
	_lives->loadTexture(_render);
	_menuMenager->loadTexture(_render);
	_menuMenager->loadFonts(0, 0, 0, _render);
	_fpsTexture->loadTexture(255, 0, 0, _render);
	_levelTexture->loadTexture(0, 0, 0, _render);
#if drawAktPath
	ghost::loadNode(_render);
#endif //drawAktPath

}

void MainGame::initResetSource() noexcept
{
	_mapMenager->initResetSource();
	_pacManMenager->initResetSource();
	_pointMenager->initResetSource();
	_boostMenager->initResetSource();
	_ghostMenager->initResetSource();
}

void MainGame::destroyTexture() noexcept
{
	_mapMenager->destroyTexture();
	_pacManMenager->destroyTexture();
	_pointMenager->destroyTexture();
	_boostMenager->destroyTexture();
	_ghostMenager->destroyTexture();
	_lives->destroyTexture();
	_menuMenager->destroyTexture();
	_fpsTexture->destroyTexture();
	_levelTexture->destroyTexture();
	destryBackSreen();
	//
	_flashMenager->destroyTexture();
	//
#if drawAktPath
	ghost::destroyTexture();
#endif //drawAktPath
}

void MainGame::getFPS(float& _frameTime) noexcept
{
	static const unsigned int num_samples = 10;
	static unsigned long long currentFrame = 0;
	static float timeAverage = 0.0f;
	static float frameTimes[num_samples]{ 60.0f };
	frameTimes[currentFrame % 10] = _frameTime;
	timeAverage = 0.0f;
	for (int i = 0; i < num_samples; ++i)
	{
		timeAverage += frameTimes[i];
	}
	_fps = 1000.0f / (timeAverage / num_samples);
	++currentFrame;
}

void MainGame::resetGame() noexcept
{
	_menuMenager->getPointsFromGame(_ghostMenager->getPoints(), _pointMenager->getPoints(), flag);
	_menuMenager->clearName();
	_mapMenager->resetBackGround();
	_pacManMenager->resetPacMan();
	_pointMenager->reset();
	_boostMenager->reset();
	_ghostMenager->resetGhost();
	_lives->resetLives();
}