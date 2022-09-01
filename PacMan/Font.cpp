#include "Font.h"
#include <string>

void LetterUnicodeLoader::insertAllLetter() noexcept
{
	for (int i = 32; i < 127; i++)
	{
		_letterInUnicode.emplace(static_cast<char>(i), i);
	}
	_letterInUnicode.emplace('•', 260);
	_letterInUnicode.emplace('π', 261);
	_letterInUnicode.emplace('∆', 262);
	_letterInUnicode.emplace('Ê', 263);
	_letterInUnicode.emplace(' ', 280);
	_letterInUnicode.emplace('Í', 281);
	_letterInUnicode.emplace('£', 321);
	_letterInUnicode.emplace('≥', 322);
	_letterInUnicode.emplace('—', 323);
	_letterInUnicode.emplace('Ò', 324);
	_letterInUnicode.emplace('”', 211);
	_letterInUnicode.emplace('Û', 243);
	_letterInUnicode.emplace('å', 346);
	_letterInUnicode.emplace('ú', 347);
	_letterInUnicode.emplace('è', 377);
	_letterInUnicode.emplace('ü', 378);
	_letterInUnicode.emplace('Ø', 379);
	_letterInUnicode.emplace('ø', 380);
}

void FontLoader::loadLibrary()
{
	if (FT_Init_FreeType(&_lib))
	{
		std::cout << "False to init FreeLibrary\n";
		return;
	}
	else if (FT_New_Face(_lib, "assets/fonts/AsapCondensed-SemiBold.ttf", 0, &_face))
	{
		std::cout << "False to load font\n";
		FT_Done_FreeType(_lib);
		return;
	}
	std::cout << "Initialization of FreType lib succesed!\n";
	if (FT_Set_Pixel_Sizes(_face, 0, fontSize))
	{
		std::cout << "ERROR to set font size\n";
	}
}

void FontLoader::allocateBuffer(Buffer& buffer, const std::map<char, int>& letterUnicode)
{
	//adwance to next letter calculated from metrics
	int adwM = 0;
	//max size of surface
	int _sWidth = 0;
	int _sHeight = 0;
	//height of char
	int height = 0;
	//width of char
	int width = 0;
	//offset from left
	int lsb = 0;
	//offset from top
	int tsb = 0;
	//offset to next char
	int rsb = 0;
	//size of char box
	int boxWidth = 0;
	int boxHeight = 0;
	//size of act char above base line
	int sABase = 0;
	//finding max size of char above base line
	for (auto letter : letterUnicode)
	{
		if (FT_Load_Char(_face, letter.second, FT_LOAD_DEFAULT))
		{
			std::cout << "ERROR to load glyph\n";
		}
		//size of act char above base line
		sABase = static_cast<int>(_face->glyph->metrics.horiBearingY / 64);
		//max size of char above base line
		_msA = _msA > sABase ? _msA : sABase;
	}
	//finding max size of bufor(surface) and creating bufor
	for (auto letter : letterUnicode)
	{
		if (FT_Load_Char(_face, letter.second, FT_LOAD_DEFAULT))
		{
			std::cout << "ERROR to load glyph\n";
		}
		//adwance to next letter calculated from metrics
		adwM = static_cast<int>(_face->glyph->metrics.horiAdvance / 64);
		//height of char
		height = static_cast<int>(_face->glyph->metrics.height / 64);
		//width of char
		width = static_cast<int>(_face->glyph->metrics.width / 64);
		//offset from left
		lsb = static_cast<int>(_face->glyph->metrics.horiBearingX / 64);
		lsb = lsb > 0 ? lsb : 0;
		//size of act char above base line
		sABase = static_cast<int>(_face->glyph->metrics.horiBearingY / 64);
		//offset from top
		if (sABase >= _msA)
		{
			tsb = 0;
		}
		else
		{
			tsb = static_cast<int>((_face->glyph->metrics.height - _face->glyph->metrics.horiBearingY) / 64);
			tsb += _msA - height;
		}
		//offset to next char
		rsb = static_cast<int>((_face->glyph->metrics.horiAdvance) / 64) - width - lsb;
		rsb = lsb > 0 ? lsb : 0;
		//size of char box;
		boxWidth = width + lsb + rsb;
		boxWidth = boxWidth > 0 ? boxWidth : (adwM + 2);
		boxHeight = height + tsb;
		//max size of surface
		_sWidth += boxWidth;
		_sHeight = _sHeight >= boxHeight ? _sHeight : boxHeight;
	}
	buffer.Width = _sWidth;
	buffer.Height = _sHeight;
	buffer.createNewBuffer(_sWidth * _sHeight * 4);
}
void FontLoader::loadFontsToBuffer(RGB&& red, RGB&& green, RGB&& blue, Buffer& buffer, const std::map<char, int>& letterUnicode, std::map<char, SDL_Rect>& letterDimension)
{
	//number of symbol in unicode
	int id = 0;
	//act symol;
	char symbol = '0';
	//adwance to next letter calculated from metrics
	int adwM = 0;
	//height of char
	int height = 0;
	//width of char
	int width = 0;
	//offset from left
	int lsb = 0;
	//offset from top
	int tsb = 0;
	//offset to next char
	int rsb = 0;
	//adwance to next letter
	unsigned int adwance = 0;
	//act pix in bitmap;
	unsigned char p = 0;
	//act poz in buffer
	int poz = 0;
	//size of symbol box
	SDL_Rect boxSize = { 0,0,0,buffer.Height };
	//size of act symbol above base line
	int sABase = 0;
	for (auto letter : letterUnicode)
	{
		symbol = letter.first;
		id = letter.second;
		if (FT_Load_Char(_face, id, FT_LOAD_DEFAULT))
		{
			std::cout << "ERROR to load glyph\n";
		}
		if (FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL))
		{
			std::cout << "ERROR to make bitmap\n";
		}
		//adwance to next letter calculated from metrics
		adwM = static_cast<int>(_face->glyph->metrics.horiAdvance / 64);
		//height of char
		height = static_cast<int>(_face->glyph->metrics.height / 64);
		//width of char
		width = static_cast<int>(_face->glyph->metrics.width / 64);
		//offset from left
		lsb = static_cast<int>(_face->glyph->metrics.horiBearingX / 64);
		lsb = lsb > 0 ? lsb : 0;
		//size of act char above base line
		sABase = static_cast<int>(_face->glyph->metrics.horiBearingY / 64);
		//offset from top
		if (sABase >= _msA)
		{
			tsb = 0;
		}
		else
		{
			tsb = height - sABase;
			tsb += _msA - height;
		}
		//offset to next char
		rsb = static_cast<int>((_face->glyph->metrics.horiAdvance) / 64) - width - lsb;
		rsb = lsb > 0 ? lsb : 0;
		//size of char box;
		boxSize.w = width + lsb + rsb;
		boxSize.x = adwance;
		//save pixel to buffer
		if (boxSize.w)
		{
			//adwance to next letter
			adwance += lsb;
			for (int i = 0; i < height; i++)
			{
				for (int y = 0; y < width; y++)
				{
					//act poz in buffer
					poz = ((i + tsb) * buffer.Width + y + adwance) * 4;
					buffer[poz++] = red;
					buffer[poz++] = green;
					buffer[poz++] = blue;
					//act pix in bitmap;
					p = _face->glyph->bitmap.buffer[i * width + y];
					buffer[poz] = p != 0 ? p : 0;
				}
			}
			adwance += width + rsb;
		}
		else
		{
			boxSize.w = adwM + 2;
			adwance += adwM + 2;
		}
		letterDimension.emplace(symbol, boxSize);
	}
}

void FontLoader::closeLibrary()
{
	if (_face)
	{
		if (FT_Done_Face(_face))
		{
			std::cout << "Exxor while closing FT_face\n";
		}
		else
		{
			std::cout << "Closing FT_Face DONE\n";
		}
	}
	if (_lib)
	{
		if (FT_Done_FreeType(_lib))
		{
			std::cout << "Exxor while closing FT_FreeType\n";
		}
		else
		{
			std::cout << "Closing FT_FreeType DONE\n";
		}
	}
}

void FontTexture::loadFontsToTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render, std::string& line) noexcept
{
	Buffer* _buffer = new Buffer{ 0,0 };
	LetterUnicodeLoader* _letterUnicode = new LetterUnicodeLoader{};
	for (auto& x : line)
	{
		_letterUnicode->insertLetter(x, x);
	}
	FontLoader* _fonts = new FontLoader{ _fontSize };
	_fonts->loadLibrary();
	_fonts->allocateBuffer(*_buffer, _letterUnicode->getLetterUnicode());
	_fonts->loadFontsToBuffer(std::move(red), std::move(green), std::move(blue), *_buffer, _letterUnicode->getLetterUnicode(), _LetterDimension);
	_fonts->closeLibrary();
	_fontTexture = TextureMenager::createTextueFromSurface(render, _buffer->buffer, _buffer->Width, _buffer->Height);
	delete _buffer;
	delete _letterUnicode;
	delete _fonts;
}

void FontTexture::loadFontsToTexture(RGB&& red, RGB&& green, RGB&& blue, SDL_Renderer* render) noexcept
{
	Buffer* _buffer = new Buffer{ 0,0 };
	LetterUnicodeLoader* _letterUnicode = new LetterUnicodeLoader{};
	_letterUnicode->insertAllLetter();
	FontLoader* _fonts = new FontLoader{_fontSize};
	_fonts->loadLibrary();
	_fonts->allocateBuffer(*_buffer, _letterUnicode->getLetterUnicode());
	_fonts->loadFontsToBuffer(std::move(red), std::move(green) , std::move(blue) , *_buffer, _letterUnicode->getLetterUnicode(), _LetterDimension);
	_fonts->closeLibrary();
	_fontTexture = TextureMenager::createTextueFromSurface(render, _buffer->buffer,_buffer->Width,_buffer->Height);
	delete _buffer;
	delete _letterUnicode;
	delete _fonts;
}