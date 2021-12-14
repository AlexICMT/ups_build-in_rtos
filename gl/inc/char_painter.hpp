/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/char_painter.hpp
 * char_painter.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:28:18
 *      Author: AlexICMT
 *
 * History:
 *
 *  @file char_painter.hpp
 * @date 08.09.2015
 * @author vlunyov
 * @brief Character and string painter
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1556 $
 * $Date: 2017-03-03 11:56:23 +0300 (Пт, 03 мар 2017) $
 *
 */

#ifndef GL_INC_CHAR_PAINTER_HPP_
#define GL_INC_CHAR_PAINTER_HPP_

#include "painter.hpp"
#include "font.hpp"
#include <cstdio>
#include <cstdarg>
#include <initializer_list>

namespace kip5 {
namespace gl {

/**
 * @brief Отрисовка символов
 *
 * Использует для отрисовки PaintDevice предоставляющий метод
 * void modify(int column, int line, FastOp op) и другие.
 */
class CharPainter : public Painter
{
	/// Базовые операции
	enum CharModeOperation : uint32_t
	{
		NOP		= 0x0000,	///< Не менять
		CLR		= 0xFFFF,	///< Очистить
		SET		= 0xFF00,	///< Установить
		XOR		= 0x00FF	///< Инвертировать
	};

	template<CharModeOperation back, CharModeOperation pen>
	struct CharModeMaker
	{
		static constexpr uint32_t val = (back << 16) | (pen ^ back);
	};

	// BUG Why it can not be placed into CharPainter class? But outside of the class works good!
//	static constexpr uint32_t MakeCharMode(CharModeOperation back, CharModeOperation pen)
//	{
//		return (back << 16) | (pen ^ back);
//	}

	Font* _font;

public:

	/// Метод вывода символов
	enum CharMode : uint32_t
	{
		CHAR_MODE_NORMAL				= 0xFFFF00FF,	///< Нормальное (позитивное)
		CHAR_MODE_NEGATIVE				= 0xFF0000FF,	///< Негативное
		CHAR_MODE_OVER					= 0x0000FF00,	///< Наложение
		CHAR_MODE_OVER_NEGATIVE			= 0x0000FFFF,	///< Негативное наложение
		CHAR_MODE_INVERSE				= 0x000000FF,	///< Инвертирование символа
		CHAR_MODE_BACKGROUND_INVERSE	= 0x00FF00FF,	///< Инвертирование фона
		CHAR_MODE_BACKGROUND_CLEAR		= 0xFFFFFFFF,	///< Очистка фона
		CHAR_MODE_BACKGROUND_SET		= 0xFF00FF00,	///< Заливка фона

//		CHAR_MODE__TEST__	= MakeCharMode(NOP, NOP),		// BUG Do not work!
		CHAR_MODE_NOP_NOP	= CharModeMaker<NOP, NOP>::val,	///< Не менять фон, не менять изображение
		CHAR_MODE_NOP_CLR	= CharModeMaker<NOP, CLR>::val,	///< Не менять фон, очистить изображение
		CHAR_MODE_NOP_SET	= CharModeMaker<NOP, SET>::val,	///< Не менять фон, установить изображение
		CHAR_MODE_NOP_XOR	= CharModeMaker<NOP, XOR>::val,	///< Не менять фон, инвертировать изображение
		CHAR_MODE_CLR_NOP	= CharModeMaker<CLR, NOP>::val,	///< Очистить фон, не менять изображение
		CHAR_MODE_CLR_CLR	= CharModeMaker<CLR, CLR>::val,	///< Очистить фон, очистить изображение
		CHAR_MODE_CLR_SET	= CharModeMaker<CLR, SET>::val,	///< Очистить фон, установить изображение
		CHAR_MODE_CLR_XOR	= CharModeMaker<CLR, XOR>::val,	///< Очистить фон, инвертировать изображение
		CHAR_MODE_SET_NOP	= CharModeMaker<SET, NOP>::val,	///< Установить фон, не менять изображение
		CHAR_MODE_SET_CLR	= CharModeMaker<SET, CLR>::val,	///< Установить фон, очистить изображение
		CHAR_MODE_SET_SET	= CharModeMaker<SET, SET>::val,	///< Установить фон, установить изображение
		CHAR_MODE_SET_XOR	= CharModeMaker<SET, XOR>::val,	///< Установить фон, инвертировать изображение
		CHAR_MODE_XOR_NOP	= CharModeMaker<XOR, NOP>::val,	///< Инвертировать фон, не менять изображение
		CHAR_MODE_XOR_CLR	= CharModeMaker<XOR, CLR>::val,	///< Инвертировать фон, очистить изображение
		CHAR_MODE_XOR_SET	= CharModeMaker<XOR, SET>::val,	///< Инвертировать фон, установить изображение
		CHAR_MODE_XOR_XOR	= CharModeMaker<XOR, XOR>::val,	///< Инвертировать фон, инвертировать изображение
	};

public:
	explicit CharPainter(PaintDevice* dev, Font* font);
	explicit CharPainter(const CharPainter& charPainter);
	~CharPainter() {delete[] _maskY;}

	inline void setFont(Font* f) {_font = f;}
	inline Font* font() {return _font;}

	inline void setCharMode(CharMode mode) {_charMode = mode;}
	inline CharMode charMode() const {return _charMode;}

	inline void setAutoNextLine(bool f) {_autoNextLine = f;}
	inline bool autoNextLine() const {return _autoNextLine;}

	inline void setAutoScroll(bool f) {_autoScroll = f;}
	inline bool autoScroll() const {return _autoScroll;}

	inline void setPosX(int x) {_posX = x;}
	inline void setPosY(int y) {_posY = y;}
	inline void setPos(int x, int y) {_posX = x, _posY = y;}
	inline int posX() const {return _posX;}
	inline int posY() const {return _posY;}

	void setDrawRect(int x, int y, int w, int h);

	void clear();

	void putChar(char ch);
	void putChar(int x, int y, char ch);
	void putChar(char16_t ch);
	void putChar(int x, int y, char16_t ch);

	void print(const char* str);
	void print(int x, int y, const char* str);
	void print(const char16_t* str);
	void print(int x, int y, const char16_t* str);

	void printf(const char* fmt, ...);

	void nextLine();
	void tab(int t);
	void tab();

	inline void setTab(int t = 32) {_tab = t; _tabList = nullptr;}
	inline void setTabTable(std::initializer_list<int16_t> l) {_tab = l.size(); _tabList = l.begin();}

	int drawChar(int x, int y, char16_t ch);

	void scrollUp(int lines);

protected:
	bool preprocessing(char ch);
	void draw(int x, int y, int w, int h, const void* data);
	void draw2(int x, int y, int w, int h, const void* data);

private:
	int16_t _posX = 0;
	int16_t _posY = 0;
	int16_t _minX;
	int16_t _maxX;
	int16_t _minY;
	int16_t _maxY;
	uint8_t* _maskY = new uint8_t[_dev->lines()];
	CharMode _charMode = CHAR_MODE_NORMAL;
	const int16_t* _tabList = nullptr;
	uint8_t _tab = 32;
	bool _autoNextLine = false;
	bool _autoScroll = false;
	char _prevChar = '\0';
};

} // namespace gl
} // namespace kip5



#endif /* GL_INC_CHAR_PAINTER_HPP_ */
