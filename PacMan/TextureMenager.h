#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

struct TextureMenager 
{
	static SDL_Texture* loadTexture(SDL_Renderer* render, const char* adress) noexcept
	{
		SDL_Surface* _surface = IMG_Load(adress);
		if (_surface == nullptr)
		{
			std::cout << "Blad inicjalizacji kanwy:" << IMG_GetError() << std::endl;
			return nullptr;
		}
		SDL_Texture* _fontTexture = SDL_CreateTextureFromSurface(render, _surface);
		if (_fontTexture == nullptr)
		{
			std::cout << "blad ladowania pliku" << SDL_GetError() << std::endl;
			SDL_FreeSurface(_surface);
			return _fontTexture;
		}
		std::cout << "Zaladowano plik          " << SDL_GetError() << IMG_GetError() << std::endl;
		SDL_FreeSurface(_surface);
		return _fontTexture;
	}

	static void setAlfaMode(SDL_Texture* texture, unsigned char& alfa) noexcept
	{
		if (SDL_SetTextureAlphaMod(texture, alfa) < 0)
		{
			std::cout << "Could'nt set alfa mode" << std::endl;
		}
	}

	static SDL_Texture* createRenderTexture(SDL_Renderer* render,const int& width,const int& height) noexcept
	{
		SDL_Texture* texture = nullptr;
		if ((texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height)) == nullptr)
		{
			std::cout << "blad ladowania pliku" << SDL_GetError() << std::endl;
			return texture;
		}
		std::cout << "Zaladowano plik          " << SDL_GetError() << std::endl;
		return texture;
	}

	static void drawTextureOnRenderTexture(SDL_Renderer* render, SDL_Texture* sourceTexture, SDL_Texture* destinationTexture, const SDL_Rect* rsource, const SDL_Rect* dest) noexcept
	{
		if (SDL_SetRenderTarget(render, destinationTexture) < 0)
		{
			std::cout << SDL_GetError() << std::endl;
		}
		else
		{
			if (SDL_RenderCopy(render, sourceTexture, rsource, dest) < 0)
			{
				std::cout << SDL_GetError() << std::endl;
			}
			else
			{
				if (SDL_SetRenderTarget(render, nullptr) < 0)
				{
					std::cout << SDL_GetError() << std::endl;
				}
			}
		}
	}

	static void drawTexture(SDL_Renderer* render, SDL_Texture* text, const SDL_Rect* rource, const SDL_Rect* dest) noexcept
	{
		if (SDL_RenderCopy(render, text, rource, dest) < 0)
		{
			std::cout << SDL_GetError() << std::endl;
		}
	}

	static void drawPair(SDL_Renderer* render, std::vector<SDL_Texture*>& tsource, const SDL_Rect* rsource, std::pair<int, SDL_Rect>& pacman) noexcept
	{
		if (SDL_RenderCopy(render, tsource[pacman.first], rsource, &pacman.second) < 0)
		{
			std::cout << SDL_GetError() << std::endl;
		}
	}

	static void drawPair(SDL_Renderer* render, SDL_Texture* tsource[], const SDL_Rect* rsource, std::pair<int, SDL_Rect>& pacman) noexcept
	{
		if (SDL_RenderCopy(render, tsource[pacman.first], rsource, &pacman.second) < 0)
		{
			std::cout << SDL_GetError() << std::endl;
		}
	}


	static void drawVectorPair(SDL_Renderer* render, std::vector<std::vector<SDL_Texture*>>& tsource, std::vector<SDL_Rect>& rsource, std::vector<std::pair<int, SDL_Rect>>& rdest) noexcept
	{
		unsigned int size = rsource.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			if (SDL_RenderCopy(render, tsource[i][rdest[i].first], &rsource[i], &rdest[i].second) < 0)
			{
				std::cout << SDL_GetError() << std::endl;
			}
		}
	}

	static void drawVectorPair(SDL_Renderer* render, SDL_Texture* tsource, SDL_Rect&& rsource, std::vector<std::pair<int, SDL_Rect>>& rdest) noexcept
	{
		unsigned int size = rdest.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			rsource.y = 32 * rdest[i].first;
			if (SDL_RenderCopy(render, tsource, &rsource, &rdest[i].second) < 0)
			{
				std::cout << SDL_GetError() << std::endl;
			}
		}
	}

	static void drawVectorRect(SDL_Renderer* render, SDL_Texture* text, const SDL_Rect* source, const std::vector<SDL_Rect>& dest) noexcept
	{
		unsigned int size = dest.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			if (SDL_RenderCopy(render, text, source, &dest[i]) < 0)
			{
				std::cout << SDL_GetError() << std::endl;
			}
		}
	}

	static void destroyTexture(SDL_Texture* text) noexcept
	{
		SDL_DestroyTexture(text);
	}

	static SDL_Texture* createTextueFromSurface(SDL_Renderer* render, unsigned char* buffer, int& width, int& height) noexcept
	{
		SDL_Surface* surf = nullptr;
		SDL_Texture* tex = nullptr;
		if (surf = SDL_CreateRGBSurfaceWithFormatFrom(reinterpret_cast<void*>(buffer), width, height, 32, 4 * width, SDL_PIXELFORMAT_RGBA32))
		{
			std::cout << "Surface was created\n";
		}
		else
		{
			std::cout << "Error: " << SDL_GetError() << "\n";
		}
		if (tex = SDL_CreateTextureFromSurface(render, surf))
		{
			std::cout << "Texture was created\n";
		}
		else
		{
			std::cout << "Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(surf);
		return tex;
	}
};



