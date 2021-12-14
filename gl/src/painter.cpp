/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/painter.cpp
 * painter.cpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:44:48
 *      Author: AlexICMT
 *
 * History:
 *
 *  @file painter.cpp
 * @date 03.09.2015
 * @author vlunyov
 * @brief Painter
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#include "painter.hpp"
#include <utility>

namespace kip5 {
namespace gl {

using std::swap;

/// Обновить изображение на устройстве (используется с буферризированными устройствами)
void Painter::update()
{
	_dev->update();
}

/// Очистка экрана
void Painter::clear()
{
	uint8_t data = 0;
	int columns = _dev->width();
	int lines = _dev->height() / 8;
	for(int line = 0; line < lines; ++line)
		for(int column = 0; column < columns; ++column)
			_dev->write(column, line, data);
}

void Painter::fill(const uint8_t* t)
{
	int columns = _dev->width();
	int lines = _dev->height() / 8;
	for(int line = 0; line < lines; ++line)
		for(int column = 0; column < columns; ++column)
			_dev->write(column, line, t[column & 7]);
}

void Painter::pixelNoCheck(int x, int y)
{
	_dev->modify(x, y >> 3, (_pixelMode & 0x0101) << (y & 7));
}

void Painter::pixel(int x, int y)
{
	if(x < 0 || x >= _dev->width() || y < 0 || y >= _dev->height())
		return;
	pixelNoCheck(x, y);
}

bool Painter::getPixel(int x, int y)
{
	if(x < 0 || x >= _dev->width() || y < 0 || y >= _dev->height())
		return false;
	uint_fast8_t data = _dev->read(x, y >> 3);
	return (data & (1 << (y & 7))) != 0;
}

void Painter::verticalLine(int x, int y1, int y2)
{
	if(x < 0 || x >= _dev->width())
		return;
	if(y1 > y2)
		swap(y1, y2);
	if(y2 < 0 || y1 >= _dev->height())
		return;
	if(y1 < 0)
		y1 = 0;
	if(y2 >= _dev->height())
		y2 = _dev->height() - 1;

//	uint16_t modeMask = pixelMode;

	unsigned mask = 0;
	unsigned bit = 1 << (y1 & 7);
	while(y1 <= y2)
	{
		mask |= bit;
		bit <<= 1;
		bit &= 0xFF;
		if(bit == 0 || y1 == y2)
		{
			if(_pixelMode != MODE_TRACERY)
				_dev->modify(x, y1 >> 3, _pixelMode & (mask | (mask << 8)));
			else
				_dev->modify(x, y1 >> 3, (0xFFFF ^ _tracery[x & 7]) & (mask | (mask << 8)));
			mask = 0;
			bit = 1;
		}
		y1++;
	}
}

void Painter::horizontalLine(int x1, int x2, int y)
{
	if(y < 0 || y >= _dev->height())
		return;
	if(x1 > x2)
		swap(x1, x2);
	if(x2 < 0 || x1 >= _dev->width())
		return;
	if(x1 < 0)
		x1 = 0;
	if(x2 >= _dev->width())
		x2 = _dev->width() - 1;
	while(x1 <= x2)
		pixelNoCheck(x1++, y);
}

void Painter::line(int x1, int y1, int x2, int y2)
{
	if(x1 == x2)
	{
		verticalLine(x1, y1, y2);
		return;
	}
	if(y1 == y2)
	{
		horizontalLine(x1, x2, y1);
		return;
	}

	int dy = y2 - y1;
	int dx = x2 - x1;
	if(dx == 0 && dy == 0)
	{
		pixel(x1, y1);
		return;
	}

	int kx = 1;
	if(dx < 0)
	{
		dx = -dx;
		kx = -1;
	}
	else if(dx == 0)
		kx = 0;

	int ky = 1;
	if(dy < 0)
	{
		dy = -dy;
		ky = -1;
	}

	bool flag = true;
	if(dx < dy)
	{
		flag = false;
		int b = dx;
		dx = dy;
		dy = b;
	}

	int i1 = dy * 2;
	int d = i1 - dx;
	int i2 = d - dx;

	int x = x1;
	int y = y1;

	for(int i = 0;; i++)
	{
		pixel(x, y);

		if(i >= dx)
			break;

		if(flag)
			x += kx;
		else
			y += ky;

		if(d < 0)
			d += i1;
		else
		{
			d += i2;
			if(!flag)
				x += kx;
			else
				y += ky;
		}
	}
}

void Painter::clippingLine(int x1, int y1, int x2, int y2)
{
	if(clipping(x1, y1, x2, y2, 0, _dev->width() - 1, 0, _dev->height() - 1))
		line(x1, y1, x2, y2);
}

// Cohen-Sutherland line-clipping algorithm
bool Painter::clipping(int& x1, int& y1, int& x2, int& y2, int xmin, int xmax, int ymin, int ymax)
{
	typedef unsigned Code;

	enum CodeVariants : Code
	{
		CENTER = 0x00,
		TOP = 0x01,
		BOTTOM = 0x02,
		RIGHT = 0x04,
		LEFT = 0x08
	};

	auto computeCode = [&](int x, int y) -> Code
	{
		Code code {CENTER};
		if(y > ymax) code |= TOP;
		else if(y < ymin) code |= BOTTOM;
		if(x > xmax) code |= RIGHT;
		else if(x < xmin) code |= LEFT;
		return code;
	};

	typedef long int I;

	auto muldiv = [](I a, I b, I c) -> I
	{
		constexpr auto p = 6;
		return (((a * b) << p) / c) >> p;
	};

	Code code1 = computeCode(x1, y1);
	Code code2 = computeCode(x2, y2);
	bool accept	{false};
	bool done {false};
	do
	{
		if(code1 == CENTER && code2 == CENTER)
		{
			accept = true;
			done = true;
		}
		else if(code1 & code2)
		{
			done = true;
		}
		else
		{
			int x, y;
			Code code = code1 ? code1 : code2;
			if(code & TOP)
			{
				x = x1 + muldiv(x2 - x1, ymax - y1, y2 - y1);
				y = ymax;
			}
			else if(code & BOTTOM)
			{
				x = x1 + muldiv(x2 - x1, ymin - y1, y2 - y1);
				y = ymin;
			}
			else if(code & RIGHT)
			{
				y = y1 + muldiv(y2 - y1, xmax - x1, x2 - x1);
				x = xmax;
			}
			else
			{
				y = y1 + muldiv(y2 - y1, xmin - x1, x2 - x1);
				x = xmin;
			}
			if(code == code1)
			{
				x1 = x;
				y1 = y;
				code1 = computeCode(x1, y1);
			}
			else
			{
				x2 = x;
				y2 = y;
				code2 = computeCode(x2, y2);
			}
		}
	} while(!done);

	return accept;
}

void Painter::rectangle(int x1, int y1, int x2, int y2)
{
	verticalLine(x1, y1, y2);
	verticalLine(x2, y1, y2);
	if(x1 > x2)
		swap(x1, x2);
	horizontalLine(x1 + 1, x2 - 1, y1);
	horizontalLine(x1 + 1, x2 - 1, y2);
}

void Painter::filledRectangle(int x1, int y1, int x2, int y2)
{
	if(x1 > x2)
		swap(x1, x2);
	if(x1 < 0)
		x1 = 0;
	if(x2 >= _dev->width())
		x2 = _dev->width() - 1;
	for(int x = x1; x <= x2; ++x)
		verticalLine(x, y1, y2);
}

void Painter::circle(int xc, int yc, int r)
{
	if(r == 0)
		return pixel(xc, yc);
	if(r < 0)
		r = -r;
	pixel(xc + r, yc);
	pixel(xc - r, yc);
	pixel(xc, yc - r);
	pixel(xc, yc + r);
	if(r == 1)
		return;
	for(int x = 0, y = r, d = 3 - 2 * r;;)
	{
		if(d <= 0)
			d += 4 * x + 6;
		else
			d += 4 * (x - y) + 10, y--;
		x++;
		if(x > y)
			break;
		pixel(xc + x, yc + y);
		pixel(xc + x, yc - y);
		pixel(xc - x, yc - y);
		pixel(xc - x, yc + y);
		if(x == y)
			break;
		pixel(xc + y, yc + x);
		pixel(xc + y, yc - x);
		pixel(xc - y, yc - x);
		pixel(xc - y, yc + x);
	}
}

void Painter::filledCircle(int xc, int yc, int r)
{
	if(r == 0)
		return pixel(xc, yc);
	if(r < 0)
		r = -r;
	verticalLine(xc, yc - r, yc + r);
	if(r == 1)
	{
		pixel(xc + 1, yc);
		pixel(xc - 1, yc);
		return;
	}
	for(int x = 0, y = r, d = 3 - 2 * r;;)
	{
		if(d <= 0)
		{
			d += 4 * x + 6;
		}
		else
		{
			verticalLine(xc + y, yc - x, yc + x);
			verticalLine(xc - y, yc - x, yc + x);
			d += 4 * (x - y) + 10, y--;
		}
		x++;

		if(x > y)
			break;
		verticalLine(xc + x, yc - y, yc + y);
		verticalLine(xc - x, yc - y, yc + y);
		if(x == y)
			break;
	}
}

void Painter::triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	clippingLine(x1, y1, x2, y2);
	clippingLine(x2, y2, x3, y3);
	clippingLine(x3, y3, x1, y1);
}

void Painter::filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	// Сортировка по координате x: x1 <= x2 <= x3
	if(x1 > x2)
		swap(x1, x2), swap(y1, y2);
	if(x2 > x3)
		swap(x2, x3), swap(y2, y3);
	if(x1 > x2)
		swap(x1, x2), swap(y1, y2);

	int dxa = x2 - x1;
	int dya = y2 - y1;

	int dxb = x3 - x1;
	int dyb = y3 - y1;

	int dxc = x3 - x2;
	int dyc = y3 - y2;

	constexpr unsigned p = 16;

	int da = (dya << p) / dxa;
	int db = (dyb << p) / dxb;
	int dc = (dyc << p) / dxc;

	int ya = (y1 << p) + (da >> 1);
	int yb = (y1 << p) + (db >> 1);
	int yc = (y2 << p) + (dc >> 1);

	// Первый этап
	for(int x = x1; x < x2; ++x)
	{
		verticalLine(x, ya >> p, yb >> p);
		ya += da;
		yb += db;
	}

	// Второй этап
	for(int x = x2; x < x3; ++x)
		{
			verticalLine(x, yc >> p, yb >> p);
			yc += dc;
			yb += db;
		}
	}

	} // namespace gl
	} // namespace kip5


