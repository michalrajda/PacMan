#include "MenuMenager.h"
#include <string>

void MenuMenager::getState(Flags& _islives) noexcept
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
			case SDLK_LALT:
			{
				_isKeyPressed._Alt = 1;
				break;
			}
			case SDLK_RALT:
			{
				_isKeyPressed._Alt = 1;
				break;
			}
			case SDLK_LSHIFT:
			{
				_isKeyPressed._Shift = 1;
				break;
			}
			case SDLK_RSHIFT:
			{
				_isKeyPressed._Shift = 1;
				break;
			}
			default:
				break;
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (_evnt.key.keysym.sym)
			{
			case SDLK_PAGEUP:
			{
				_islives._showFPS = true;
				break;
			}
			case SDLK_PAGEDOWN:
			{
				_islives._showFPS = false;
				break;
			}
			case SDLK_LALT:
			{
				_isKeyPressed._Alt = 0;
				break;
			}
			case SDLK_RALT:
			{
				_isKeyPressed._Alt = 0;
				break;
			}
			case SDLK_LSHIFT:
			{
				_isKeyPressed._Shift = 0;
				break;
			}
			case SDLK_RSHIFT:
			{
				_isKeyPressed._Shift = 0;
				break;
			}
			case SDLK_q:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'Q';
					break;
				}
				_name += 'q';
				break;
			}
			case SDLK_w:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'W';
					break;
				}
				_name += 'w';
				break;
			}
			case SDLK_e:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += ' ';
						break;
					}
					_name += 'E';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'Í';
					break;
				}
				_name += 'e';
				break;
			}
			case SDLK_r:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'R';
					break;
				}
				_name += 'r';
				break;
			}
			case SDLK_t:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'T';
					break;
				}
				_name += 't';
				break;
			}
			case SDLK_y:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'Y';
					break;
				}
				_name += 'y';
				break;
			}
			case SDLK_u:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'U';
					break;
				}
				_name += 'u';
				break;
			}
			case SDLK_i:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'I';
					break;
				}
				_name += 'i';
				break;
			}
			case SDLK_o:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += '”';
						break;
					}
					_name += 'O';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'Û';
					break;
				}
				_name += 'o';
				break;
			}
			case SDLK_p:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'P';
					break;
				}
				_name += 'p';
				break;
			}
			case SDLK_a:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += '•';
						break;
					}
					_name += 'A';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'π';
					break;
				}
				_name += 'a';
				break;
			}
			case SDLK_s:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += 'å';
						break;
					}
					_name += 'S';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'ú';
					break;
				}
				_name += 's';
				break;
			}
			case SDLK_d:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'D';
					break;
				}
				_name += 'd';
				break;
			}
			case SDLK_f:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'F';
					break;
				}
				_name += 'f';
				break;
			}
			case SDLK_g:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'G';
					break;
				}
				_name += 'g';
				break;
			}
			case SDLK_h:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'H';
					break;
				}
				_name += 'h';
				break;
			}
			case SDLK_j:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'J';
					break;
				}
				_name += 'j';
				break;
			}
			case SDLK_k:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'K';
					break;
				}
				_name += 'k';
				break;
			}
			case SDLK_l:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += '£';
						break;
					}
					_name += 'L';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += '≥';
					break;
				}
				_name += 'l';
				break;
			}
			case SDLK_z:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += 'Ø';
						break;
					}
					_name += 'Z';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'ø';
					break;
				}
				_name += 'z';
				break;
			}
			case SDLK_x:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += 'è';
						break;
					}
					_name += 'X';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'ü';
					break;
				}
				_name += 'x';
				break;
			}
			case SDLK_c:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += '∆';
						break;
					}
					_name += 'C';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'Ê';
					break;
				}
				_name += 'c';
				break;
			}
			case SDLK_v:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'V';
					break;
				}
				_name += 'v';
				break;
			}
			case SDLK_b:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'B';
					break;
				}
				_name += 'b';
				break;
			}
			case SDLK_n:
			{
				if (_isKeyPressed._Shift)
				{
					if (_isKeyPressed._Alt)
					{
						_name += '—';
						break;
					}
					_name += 'N';
					break;
				}
				if (_isKeyPressed._Alt)
				{
					_name += 'Ò';
					break;
				}
				_name += 'n';
				break;
			}
			case SDLK_m:
			{
				if (_isKeyPressed._Shift)
				{
					_name += 'M';
					break;
				}
				_name += 'm';
				break;
			}
			case SDLK_MINUS:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '_';
					break;
				}
				_name += '-';
				break;
			}
			case SDLK_EQUALS:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '+';
					break;
				}
				_name += '=';
				break;
			}
			case SDLK_SPACE:
			{
				_name += ' ';
				break;
			}
			case SDLK_BACKQUOTE:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '~';
					break;
				}
				_name += '`';
				break;
			}
			case SDLK_LEFTBRACKET:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '{';
					break;
				}
				_name += '[';
				break;
			}
			case SDLK_RIGHTBRACKET:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '}';
					break;
				}
				_name += ']';
				break;
			}
			case SDLK_SEMICOLON:
			{
				if (_isKeyPressed._Shift)
				{
					_name += ':';
					break;
				}
				_name += ';';
				break;
			}
			case SDLK_QUOTE:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '"';
					break;
				}
				_name += '\'';
				break;
			}
			case SDLK_BACKSLASH:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '|';
					break;
				}
				_name += '\\';
				break;
			}
			case SDLK_COMMA:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '<';
					break;
				}
				_name += ',';
				break;
			}
			case SDLK_PERIOD:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '>';
					break;
				}
				_name += '.';
				break;
			}
			case SDLK_SLASH:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '?';
					break;
				}
				_name += '/';
				break;
			}
			case SDLK_0:
			{
				if (_isKeyPressed._Shift)
				{
					_name += ')';
					break;
				}
				_name += '0';
				break;
			}
			case SDLK_1:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '!';
					break;
				}
				_name += '1';
				break;
			}
			case SDLK_2:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '@';
					break;
				}
				_name += '2';
				break;
			}
			case SDLK_3:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '#';
					break;
				}
				_name += '3';
				break;
			}
			case SDLK_4:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '$';
					break;
				}
				_name += '4';
				break;
			}
			case SDLK_5:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '%';
					break;
				}
				_name += '5';
				break;
			}
			case SDLK_6:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '^';
					break;
				}
				_name += '6';
				break;
			}
			case SDLK_7:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '&';
					break;
				}
				_name += '7';
				break;
			}
			case SDLK_8:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '*';
					break;
				}
				_name += '8';
				break;
			}
			case SDLK_9:
			{
				if (_isKeyPressed._Shift)
				{
					_name += '(';
					break;
				}
				_name += '9';
				break;
			}
			case SDLK_ESCAPE:
			{
				_islives._reset = false;
				_islives._getname = false;
				_islives._saveName = false;
				break;
			}
			case SDLK_BACKSPACE:
			{
				if (!_name.empty())
				{
					_name.pop_back();
					break;
				}
				break;
			}
			case SDLK_RETURN:
			{
				_islives._getname = false;
				_islives._reset = false;
				_islives._saveName = true;
				break;
			}
			case SDLK_KP_ENTER:
			{
				_islives._getname = false;
				_islives._reset = false;
				_islives._saveName = true;
				break;
			}
			default:
				break;
			}
			break;
		}
		case SDL_QUIT:
		{
			_islives._gameState = false;
			_islives._getname = false;
			_islives._reset = false;
			_islives._saveName = false;
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void MenuMenager::loadPointsFromFile() noexcept
{
	std::fstream plik;
	plik.open("wyniki.tmp", std::ios::binary | std::ios::in);
	if (!plik)
	{
		plik.clear(plik.rdstate() & ~std::ios::failbit);
		plik.open("wyniki.tmp", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
		plik.close();
		return;
	}
	unsigned int length{};
	unsigned int point{};
	unsigned int kill{};
	std::string names;
	unsigned int size{};
	std::pair<std::string, PointsAndKills> wzor;
	plik.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (unsigned int i = 0; i < size; ++i)
	{
		names.clear();
		point = 0;
		kill = 0;
		plik.read(reinterpret_cast<char*>(&length), sizeof(length));
		if (length > 0)
		{
			for (unsigned int i = 0; i < length; ++i)
			{
				names += plik.get();
			}
		}
		plik.read(reinterpret_cast<char*>(&point), sizeof(point));
		plik.read(reinterpret_cast<char*>(&kill), sizeof(kill));
		wzor.first = names;
		wzor.second.point = point;
		wzor.second.kill = kill;
		_showPoints.push_back(wzor);
		if (plik.eof())
		{
			break;
		}
	}
	plik.clear(plik.rdstate() & ~std::ios::failbit);
	plik.close();
}

void MenuMenager::savePointsToFile() noexcept
{
	unsigned int point{};
	unsigned int kills{};
	std::fstream plik;
	unsigned int length{};
	unsigned int size{};
	size = _showPoints.size();
	plik.open("wyniki.tmp", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
	plik.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (unsigned int i = 0; i < size; ++i)
	{
		length = _showPoints[i].first.size();
		point = _showPoints[i].second.point;
		kills = _showPoints[i].second.kill;
		plik.write(reinterpret_cast<char*>(&length), sizeof(length));
		plik.write(reinterpret_cast<char*>(_showPoints[i].first.data()), sizeof(char) * length);
		plik.write(reinterpret_cast<char*>(&point), sizeof(point));
		plik.write(reinterpret_cast<char*>(&kills), sizeof(kills));
	}
	plik.close();
}

void MenuMenager::drawPoints(SDL_Renderer* render) noexcept
{
	drawMenu(render);
	_line.clear();
	auto size = _showPoints.size();
	_fonts._dest.y = 200;
	for (unsigned int i = 0; i < size; ++i)
	{
		_fonts._dest.x = 100;
		_line = std::to_string(i + 1) + ". NAME: " + _showPoints[i].first + ", POINTS: " + std::to_string(_showPoints[i].second.point) + ", KILLS: " + std::to_string(_showPoints[i].second.kill);
		_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
		_fonts._dest.y += _fonts._source.h;
	}
	TextureMenager::drawTexture(render, _buttonT, &_buttons._menuSource, &_buttons._menuDest);
}

void MenuMenager::drawPointsOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	drawMenuOnBackScreen(render, destTexture);
	_line.clear();
	auto size = _showPoints.size();
	_fonts._dest.y = 200;
	for (unsigned int i = 0; i < size; ++i)
	{
		_fonts._dest.x = 100;
		_line = std::to_string(i + 1) + ". NAME: " + _showPoints[i].first + ", POINTS: " + std::to_string(_showPoints[i].second.point) + ", KILLS: " + std::to_string(_showPoints[i].second.kill);
		_fontTexture->drawLine(render, destTexture, _fonts._source, _fonts._dest, _line);
		_fonts._dest.y += _fonts._source.h;
	}
	TextureMenager::drawTextureOnRenderTexture(render, _buttonT, destTexture, &_buttons._menuSource, &_buttons._menuDest);
}

void MenuMenager::drawName(SDL_Renderer* render, Flags& _islives) noexcept
{
	drawMenu(render);
	getState(_islives);
	if (_name.size() > 20)
	{
		_name.pop_back();
	}
	_line.clear();
	_fonts._dest.x = 100;
	_fonts._dest.y = 300;
	_line = "WRITE NAME: " + _name + "_";
	std::map<char, SDL_Rect>& _mapOfFonts = _fontTexture->getLetterDimension();
	SDL_Texture* tex = _fontTexture->getFontTexture();
	_fontTexture->drawLine(render, _fonts._source, _fonts._dest, _line);
	if (_name.size() == 0)
	{
		_islives._saveName = false;
	}
}

void MenuMenager::choseMenu(Flags& flag) noexcept
{
	SDL_Event _evnt;
	while (SDL_PollEvent(&_evnt))
	{
		switch (_evnt.type)
		{
		case SDL_MOUSEBUTTONUP:
		{
			if (_evnt.button.button == SDL_BUTTON_LEFT)
			{
				if (checkPosition(_evnt.button, _buttons._startDest))
				{
					flag._isLives = true;
					flag._actLevel = true;
					flag._nextLevel = true;
				}
				else if (checkPosition(_evnt.button, _buttons._exitDest))
				{
					flag._gameState = false;
				}
				else if (checkPosition(_evnt.button, _buttons._pointsDest))
				{
					flag._showPoints = true;
				}
			}
			break;
		}
		case SDL_QUIT:
		{
			flag._gameState = false;
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
			case SDLK_ESCAPE:
			{
				flag._isLives = false;
				flag._gameState = false;
				flag._showPoints = false;
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}

void MenuMenager::returnToMenu(Flags& flag) noexcept
{
	SDL_Event _evnt;
	while (SDL_PollEvent(&_evnt))
	{
		switch (_evnt.type)
		{
		case SDL_MOUSEBUTTONUP:
		{
			if (_evnt.button.button == SDL_BUTTON_LEFT && checkPosition(_evnt.button, _buttons._menuDest)) flag._showPoints = false;
			break;
		}
		case SDL_QUIT:
		{
			flag._gameState = false;
			flag._showPoints = false;
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
			case SDLK_ESCAPE:
			{
				flag._showPoints = false;
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}

void MenuMenager::getPointsFromGame(const unsigned int* _pointsGhost, const unsigned int& _showPoints,Flags& flags) noexcept
{
	act_points.point = _pointsGhost[0] + _showPoints;
	act_points.kill = _pointsGhost[1];
	if (!act_points.point)
	{
		flags._getname = false;
		flags._reset = false;
		flags._saveName = false;
	}
	else 
	{
		flags._getname = true;
	}
}


