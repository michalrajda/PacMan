#pragma once
#include "TextureMenager.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <map>

struct Buffer
{
	int Width;
	int Height;
	int sizeT;
	unsigned char* buffer;
	unsigned char err;
	Buffer(int w, int h) noexcept :Width{ w }, Height{ h }, sizeT{ 0 }, buffer{ nullptr }, err{ 0 }{}
	~Buffer()
	{
		delete[] buffer;
	}
	void createNewBuffer(int&& size) noexcept;
	unsigned char& operator[](int&& size) noexcept;
	unsigned char& operator[](int& size) noexcept;
};


class LetterUnicodeLoader
{
private:
	std::map<char, int> _letterInUnicode;
public:
	LetterUnicodeLoader() noexcept :_letterInUnicode{ std::map<char,int>() }{};
	const std::map<char, int>& getLetterUnicode() noexcept;
	void insertAllLetter() noexcept;
	void insertLetter(const char& Letter,const  int& ID_IN_UNICODE) noexcept;
};

using RGB = unsigned char;

class FontLoader
{
private:
	FT_Library _lib;
	FT_Face _face;
	unsigned int fontSize;
	//max size of char abov base line
	int _msA;
public:
	FontLoader(const unsigned int fS) noexcept :_lib{ nullptr }, _face{ nullptr }, fontSize{ fS }, _msA{0} {}
	void loadLibrary();
	void allocateBuffer(Buffer& buffer, const std::map<char, int>& letterUnicode);
	void loadFontsToBuffer(RGB&& red,RGB&& green, RGB&& blue, Buffer& buffer, const std::map<char, int>& letterUnicode, std::map<char, SDL_Rect>& letterDimension);
	void closeLibrary();
};

class FontTexture
{
	SDL_Texture* _fontTexture;
	unsigned int _fontSize;
	std::map<char, SDL_Rect> _LetterDimension;
public:
	FontTexture(unsigned int&& size) noexcept: _LetterDimension{ std::map<char,SDL_Rect>() }, _fontSize{ size }, _fontTexture{ nullptr }{};
	std::map<char, SDL_Rect>& getLetterDimension() noexcept;
	SDL_Texture* getFontTexture() noexcept;
	void loadFontsToTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render,std::string& line) noexcept;
	void loadFontsToTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept;
	void drawLine(SDL_Renderer* render,SDL_Rect& fsource, SDL_Rect& fdest,std::string& line) noexcept;
	void drawLine(SDL_Renderer*,SDL_Texture* destTexture, SDL_Rect& fsource, SDL_Rect& fdest, std::string& line) noexcept;
	void destroy() noexcept;
};

inline void Buffer::createNewBuffer(int&& size) noexcept
{
	sizeT = size;
	buffer = new unsigned char[sizeT] {0};
}

inline unsigned char& Buffer::operator[](int&& size) noexcept
{
	if (size < sizeT)
	{
		return buffer[size];
	}
	return err;
}

inline unsigned char& Buffer::operator[](int& size) noexcept
{
	if (size < sizeT)
	{
		return buffer[size];
	}
	return err;
}

inline const std::map<char, int>& LetterUnicodeLoader::getLetterUnicode() noexcept
{
	return _letterInUnicode;
}

inline void LetterUnicodeLoader::insertLetter(const char& Letter,const int& ID_IN_UNICODE) noexcept
{
	_letterInUnicode.emplace(Letter, ID_IN_UNICODE);
}

inline std::map<char, SDL_Rect>& FontTexture::getLetterDimension() noexcept
{
	return _LetterDimension;
}

inline SDL_Texture* FontTexture::getFontTexture() noexcept
{
	return _fontTexture;
}

inline void FontTexture::drawLine(SDL_Renderer* render, SDL_Rect& fsource, SDL_Rect& fdest, std::string& line) noexcept
{
	for (auto& x : line)
	{
		fsource = _LetterDimension.at(x);
		fdest.w = fsource.w;
		fdest.h = fsource.h;
		TextureMenager::drawTexture(render, _fontTexture, &fsource, &fdest);
		fdest.x += fsource.w;
	}
}

inline void FontTexture::drawLine(SDL_Renderer* render, SDL_Texture* destTexture, SDL_Rect& fsource, SDL_Rect& fdest, std::string& line) noexcept
{
	for (auto& x : line)
	{
		fsource = _LetterDimension.at(x);
		fdest.w = fsource.w;
		fdest.h = fsource.h;
		TextureMenager::drawTextureOnRenderTexture(render, _fontTexture, destTexture, &fsource, &fdest);
		fdest.x += fsource.w;
	}
}

inline void FontTexture::destroy() noexcept
{
	SDL_DestroyTexture(_fontTexture);
}