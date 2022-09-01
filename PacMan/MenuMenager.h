#pragma once
#include "TextureMenager.h"
#include <fstream>
#include "Font.h"
#include "Flags.h"
#include <vector>
#include <algorithm>

struct PointsAndKills
{
	unsigned int point;
	unsigned int kill;
};

template <>
struct std::greater<const std::pair<std::string, PointsAndKills>&>
{
	constexpr bool operator()(const std::pair<std::string, PointsAndKills>& _Left, const std::pair<std::string, PointsAndKills>& _Right) const
	{
		return (_Left.second.point != _Right.second.point) ? (_Left.second.point > _Right.second.point) : (_Left.second.kill > _Left.second.kill);
	}
};

struct KeyPressed
{
	uint8_t _Shift : 1,
			_Alt : 1;
};

struct FontsSize
{
	SDL_Rect _source;
	SDL_Rect _dest;
};

struct ButtonSize
{
	SDL_Rect _startSource;
	SDL_Rect _startDest;
	SDL_Rect _exitSource;
	SDL_Rect _exitDest;
	SDL_Rect _ponitsSource;
	SDL_Rect _pointsDest;
	SDL_Rect _menuSource;
	SDL_Rect _menuDest;
	SDL_Rect _allSource;
	SDL_Rect _allDest;
};

struct MenuSize
{
	SDL_Rect _source;
	SDL_Rect _dest;
};

class MenuMenager
{
private:
	KeyPressed _isKeyPressed;
	std::string _name,_line;
	PointsAndKills act_points{};
	std::vector<std::pair<std::string, PointsAndKills>> _showPoints;
	FontsSize _fonts;
	ButtonSize _buttons;
	MenuSize _menu;
	SDL_Texture* _buttonT;
	SDL_Texture* _menuT;
	std::unique_ptr<FontTexture> _fontTexture;
	void getState(Flags& _islives) noexcept;
	bool checkPosition(SDL_MouseButtonEvent& mouseButton, SDL_Rect& button) noexcept;
	void drawMenu(SDL_Renderer* render) noexcept;
	void drawMenuOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
public:
	MenuMenager() :_isKeyPressed{ 0 }, _name{ std::string{""} }, _line{ std::string{""} }, act_points{ 0 }, _showPoints{ std::vector<std::pair<std::string,PointsAndKills>>{} }, 
		_fonts{}, _buttonT{ nullptr }, _menuT{ nullptr }, _fontTexture{ std::make_unique<FontTexture>(25) }
	{
		_buttons._startSource = { 0,0,200,100 };
		_buttons._startDest = { 300,250,200,100 };
		_buttons._exitSource = { 0,100,200,100 };
		_buttons._exitDest = { 300,350,200,100 };
		_buttons._ponitsSource = { 0,200,200,100 };
		_buttons._pointsDest = { 300,450,200,100 };
		_buttons._menuSource = { 0,300,200,100 };
		_buttons._menuDest = { 560,500,200,100 };
		_buttons._allSource = { 0,0,200,300 };
		_buttons._allDest = { 300,250,200,300 };
		_menu._source = { 0,0,800,640 };
		_menu._dest = { 0,0,800,640 };
	}
	void loadPointsFromFile() noexcept;
	void savePointsToFile() noexcept;
	void saveActualPoints() noexcept;
	void loadTexture(SDL_Renderer* render) noexcept;
	void drawButton(SDL_Renderer* render) noexcept;
	void drawButtonOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void drawPoints(SDL_Renderer* render) noexcept;
	void drawPointsOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept;
	void drawName(SDL_Renderer* render, Flags& _flags) noexcept;
	void choseMenu(Flags& _flag) noexcept;
	void returnToMenu(Flags& _flag) noexcept;
	void destroyTexture() noexcept;
	void getPointsFromGame(const unsigned int* _pointsGhost, const unsigned int& _showPoints, Flags& flags) noexcept;
	void clearName() noexcept;
	void loadFonts(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept;
};

inline void MenuMenager::saveActualPoints() noexcept
{
	_showPoints.push_back(std::pair(_name, act_points));
	std::sort(_showPoints.begin(), _showPoints.end(), std::greater<const std::pair<std::string, PointsAndKills>&>());
	while (_showPoints.size() > 10)
	{
		_showPoints.pop_back();
	}
}

inline void MenuMenager::loadTexture(SDL_Renderer* render) noexcept
{
	_menuT = TextureMenager::loadTexture(render, "assets/image/menu.png");
	_buttonT = TextureMenager::loadTexture(render, "assets/image/button.png");
}

inline bool MenuMenager::checkPosition(SDL_MouseButtonEvent& mouseButton, SDL_Rect& button) noexcept
{
	return (mouseButton.x > button.x && mouseButton.x<(button.x + button.w) && mouseButton.y>button.y && mouseButton.y < (button.y + button.h));
}

inline void MenuMenager::drawMenu(SDL_Renderer* render) noexcept
{
	TextureMenager::drawTexture(render, _menuT, &_menu._source, &_menu._dest);
}

inline void MenuMenager::drawMenuOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	TextureMenager::drawTextureOnRenderTexture(render, _menuT, destTexture, &_menu._source, &_menu._dest);
}

inline void MenuMenager::drawButton(SDL_Renderer* render) noexcept
{
	drawMenu(render);
	TextureMenager::drawTexture(render, _buttonT, &_buttons._allSource, &_buttons._allDest);
}

inline void MenuMenager::drawButtonOnBackScreen(SDL_Renderer* render, SDL_Texture* destTexture) noexcept
{
	drawMenuOnBackScreen(render, destTexture);
	TextureMenager::drawTextureOnRenderTexture(render, _buttonT, destTexture, &_buttons._allSource, &_buttons._allDest);
}

inline void MenuMenager::destroyTexture() noexcept
{
	SDL_DestroyTexture(_menuT);
	SDL_DestroyTexture(_buttonT);
	_fontTexture->destroy();
}

inline void MenuMenager::clearName() noexcept
{
	_name.clear();
}

inline void MenuMenager::loadFonts(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept
{
	_fontTexture->loadFontsToTexture(std::move(red), std::move(green), std::move(blue), render);
}