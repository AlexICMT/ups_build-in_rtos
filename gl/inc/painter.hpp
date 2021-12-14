/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/painter.hpp
 * painter.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:41:00
 *      Author: AlexICMT
 *
 * History:
 *
 * @file painter.hpp
 * @date 03.09.2015
 * @author vlunyov
 * @brief Painter
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#ifndef GL_INC_PAINTER_HPP_
#define GL_INC_PAINTER_HPP_

#include "v_paint_device_base.hpp"

namespace kip5 {
namespace gl {

/**
 * @brief Отрисовка векторной графики
 *
 * Использует для отрисовки PaintDevice предоставляющий метод
 * void modify(int column, int line, FastOp op) и другие.
 */
class Painter
{
protected:
	PaintDevice* _dev;		///< Устройство на котором происходит отрисовка

public:
	/// Метод вывода точек изображения
	enum PixelMode : uint16_t
	{
		MODE_SET		= 0xFF00,	///< Установить
		MODE_CLEAR		= 0xFFFF,	///< Сбросить
		MODE_INVERSE	= 0x00FF,	///< Инвертировать
		MODE_TRACERY	= 0x0000	///< Узор
	};
private:
	static constexpr int UD = (1ULL<<32)-1;

public:
	explicit Painter(PaintDevice* dev) : _dev(dev) {}
	explicit Painter(const Painter&) = default;

	void update();

	inline int width() const {return _dev->width();}
	inline int height() const {return _dev->height();}

	inline void setPixelMode(PixelMode mode) {_pixelMode = mode;}
	inline PixelMode pixelMode() const {return _pixelMode;}

	inline void setTracery(const uint8_t* t) {_tracery = t;}
	inline const uint8_t* tracery() const {return _tracery;}

	void clear();
	void fill(const uint8_t* t);
	void pixel(int x, int y);
	bool getPixel(int x, int y);
	void verticalLine(int x, int y1, int y2);
	void horizontalLine(int x1, int x2, int y);
	void line(int x1, int y1, int x2, int y2);
	void clippingLine(int x1, int y1, int x2, int y2);
	inline void move(int x = UD, int y = UD) {_x = x; _y = y;}
	inline void line(int x, int y);
	inline void clippingLine(int x, int y);
	void rectangle(int x1, int y1, int x2, int y2);
	void filledRectangle(int x1, int y1, int x2, int y2);
	void circle(int xc, int yc, int r);
	void filledCircle(int xc, int yc, int r);
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

protected:
	void pixelNoCheck(int x, int y);
	bool clipping(int& x1, int& y1, int& x2, int& y2, int xmin, int xmax, int ymin, int ymax);

private:
	const uint8_t* _tracery {nullptr};
	int _x {UD};
	int _y {UD};
	PixelMode _pixelMode {MODE_SET};
};

void Painter::line(int x, int y)
{
	if(_x != UD && _y != UD)
		line(_x, _y, x, y);
	_x = x;
	_y = y;
}

void Painter::clippingLine(int x, int y)
{
	if(_x != UD && _y != UD)
		clippingLine(_x, _y, x, y);
	_x = x;
	_y = y;
}

} // namespace gl
} // namespace kip5



#endif /* GL_INC_PAINTER_HPP_ */
