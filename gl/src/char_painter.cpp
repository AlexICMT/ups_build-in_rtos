/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/char_painter.cpp
 * char_painter.cpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:30:21
 *      Author: AlexICMT
 *
 * History:
 *
 * @file char_painter.cpp
 * @date 08.09.2015
 * @author vlunyov
 * @brief Character and string painter
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1556 $
 * $Date: 2017-03-03 11:56:23 +0300 (ѕт, 03 мар 2017) $
 *
 */

#include "char_painter.hpp"
#include "bit_stream.hpp"

namespace kip5 {
namespace gl {

using namespace kip5::utils;

CharPainter::CharPainter(PaintDevice* dev, Font* font) : Painter(dev), _font(font)
{
	setDrawRect(0, 0, _dev->width(), _dev->height());
}

CharPainter::CharPainter(const CharPainter& charPainter) : CharPainter(charPainter._dev, charPainter._font)
{
	setDrawRect(charPainter._minX, charPainter._minY,
		charPainter._maxX - charPainter._minX, charPainter._maxY - charPainter._minY);
}

void CharPainter::setDrawRect(int x, int y, int w, int h)
{
	_minX = x;
	_maxX = x + w - 1;
	_minY = y;
	_maxY = y + h - 1;

	for(int i = 0; i < _dev->lines(); ++i)
	{
		uint_fast8_t b = 0xFF;
		if(i == y / 8)
			b &= ((1 << (y & 7)) - 1) ^ 0xFF;
		if(i == (y + h) / 8)
			b &= (1 << ((y + h) & 7)) - 1;
		if(i < y / 8 || i > (y + h) / 8)
			b = 0x00;
		_maskY[i] = b;
	}
}

void CharPainter::clear()
{
	Painter::clear();
	setPos(0, 0);
}

void CharPainter::putChar(char ch)
{
	if(!preprocessing(ch))
		return;
	if((unsigned char)ch < ' ')
	{
		ch = '?';
	}
	_posX += drawChar(_posX, _posY, ch);
}

void CharPainter::putChar(int x, int y, char ch)
{
	setPos(x, y);
	putChar(ch);
}

void CharPainter::putChar(char16_t ch)
{
	if(!preprocessing(ch))
		return;
	if(ch < ' ')
	{
		ch = u'?';
	}
	_posX += drawChar(_posX, _posY, ch);
}

void CharPainter::putChar(int x, int y, char16_t ch)
{
	setPos(x, y);
	putChar(ch);
}

void CharPainter::print(const char* str)
{
//	while(*str)
//		putChar(*str++);
	while(*str)
		putChar(_font->extractMultibyteChar(&str));
}

void CharPainter::print(int x, int y, const char* str)
{
	setPos(x, y);
	print(str);
}

void CharPainter::print(const char16_t* str)
{
	while(*str)
		putChar(*str++);
}

void CharPainter::print(int x, int y, const char16_t* str)
{
	setPos(x, y);
		print(str);
}

/**
 * It is possible to print UTF-8 characters if the font supported it.
 * @param fmt Format string
 * @param ... Parameters
 */
void CharPainter::printf(const char* fmt, ...)
{
	std::va_list args;
	::va_start(args, fmt);
	constexpr unsigned strMaxLength = 128;
	char str[strMaxLength];
	if(::vsnprintf(str, sizeof(str), fmt, args) > 0)
		print(str);
	::va_end(args);
}

void CharPainter::nextLine()
{
	int h = _font->height();
	_posX = 0;
	_posY += h;

	if(_posY + h > _dev->height())
	{
		if(_autoScroll)
		{
			_posY -= h;
			scrollUp(h);
		}
		else
		{
			_posY = 0;
		}
	}
}

void CharPainter::tab(int t)
{
	_posX += t;
	_posX -= _posX % t;
	if(_autoNextLine)
		if(_posX >= _dev->width())
			nextLine();
}

void CharPainter::tab()
{
	if(!_tabList)
	{
		tab(_tab);
	}
	else
	{
		int x = 0;
		for(int i = 0; x <= _posX; ++i)
		{
			if(i >= _tab - 1)
				i = _tab - 1;
			x += _tabList[i];
		}
		_posX = x;
		if(_autoNextLine)
			if(_posX >= _dev->width())
				nextLine();
	}
}

bool CharPainter::preprocessing(char ch)
{
	bool printable = false;
	// перевод строки: CR, LF, CR+LF ("\r", "\n", "\r\n")
	if(ch == '\n')		//LF 0x0A '\n'
	{
		if(_prevChar != '\r')
			nextLine();
	}
	else if(ch == '\r')	// CR 0x0D '\r'
	{
		nextLine();
	}
	else if(ch == '\t')
	{
		tab();
	}
//	else if((unsigned char)ch >= ' ')
	else
	{
		printable = true;
	}
	_prevChar = ch;

	if(_autoNextLine)
	{
		if(printable)
		{
			int cw = _font->width(ch);
			if(_posX + cw >= _dev->width())
			{
				nextLine();
			}
		}
	}
	return printable;
}

void CharPainter::draw(int x, int y, int w, int h, const void* data)
{
	InputOneBitStream bitStream(data);

	auto modify = [this](int x, int line, uint32_t op) -> void
	{
		if(x >= _minX && x <= _maxX && line >= 0 && line < _dev->lines())
		{
			op &= _maskY[line] | _maskY[line] << 16;
			op |= op << 8;
			op &= _charMode;
			op ^= op >> 16;
			_dev->modify(x, line, op);
		}
	};

	for(int ix = x; ix < x + w; ++ix)
	{
		uint32_t op = 0;
		for(int iy = y; iy < y + h; ++iy)
		{
			op |= (0x00010000 | bitStream.readBit()) << (iy & 7);
			if((iy & 7) == 7 || iy == (y + h - 1))
			{
				modify(ix, iy >> 3, op);
				op = 0;
			}
		}
	}
}

// TODO Experimental CharPainter::draw2 (not yet completed)
void CharPainter::draw2(int x, int y, int w, int h, const void* data)
{
	InputBitStream2 bitStream(data);

	auto modify = [this](int x, int line, uint32_t op) -> void
	{
		if(x >= _minX && x <= _maxX && line >= 0 && line < _dev->lines())
		{
			op &= _maskY[line] | _maskY[line] << 16;
			op |= op << 8;
			op &= _charMode;
			op ^= op >> 16;
			_dev->modify(x, line, op);
		}
	};

	for(int ix = x; ix < x + w; ++ix)
	{
		bitStream.insertDummyBits(y & 7);
		for(int line = y >> 3; line < (y + h + 7) >> 3; ++line)
		{
			int bits = (y + h) - (line << 3);
			if(bits > 8)
				bits = 8;
			uint32_t op = bitStream.readBits(bits);
			modify(ix, line, op);
		}
	}
}

int CharPainter::drawChar(int x, int y, char16_t ch)
{
	int w = _font->width(ch);
	int h = _font->height(ch);
	if(x <= _maxX && x + w >= _minX && y <= _maxY && y + h >= _minY)
	{
		const void* data = _font->getCharDataAddr(ch);
		draw(x, y, w, h, data);
	}
	return w;
}

void CharPainter::scrollUp(int lines)
{
	if(lines == 8)
	{
		for(int y = 0; y < _dev->lines()-1; ++y)
		{
			for(int x = 0; x < _dev->width(); ++x)
			{
				auto d = _dev->read(x, y+1);
				_dev->write(x, y, d);
			}
		}
		for(int x = 0; x < _dev->width(); ++x)
		{
			_dev->write(x, _dev->lines()-1, 0);
		}
	}
	else
	{
		// TODO: —кролл вверх
	}
}

} // namespace gl
} // namespace kip5


