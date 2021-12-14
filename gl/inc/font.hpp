/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/font.hpp
 * font.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:34:23
 *      Author: AlexICMT
 *
 * History:
 *
 * @file font.hpp
 * @date 27.07.2015
 * @author vlunyov
 * @brief Font
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#ifndef GL_INC_FONT_HPP_
#define GL_INC_FONT_HPP_

#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace kip5 {
namespace gl {

/**
 * @brief Шрифт
 * @details
 * Формат шрифта:<br/>
 * f 1 байт - флаги:<br/>
 *            бит 0 - наличие символов с кодами 0x00..0x1F<br/>
 *            бит 1 - наличие символов с кодами 0x80..0xBF<br/>
 * n 1 байт - ко-во символов<br/>
 * h 1 байт - высота символов (если 0, то высота указывается отдельно для каждого символа)<br/>
 * w 1 байт - ширина символов (если 0, то ширина указывается отдельно для каждого символа)<br/>
 * t 2 байта * n - таблица смещений символов (присутствует только при f.7 = 1)<br/>
 * m x байт - изображения символов (если h=0, то начинается с высоты символа)<br/>
*/
class Font
{
	using Char = char16_t;
	using IndexType = std::initializer_list<uint16_t>;	///< Тип данных индекса шрифта
	using DataType = std::initializer_list<uint8_t>;	///< Тип данных изображения шрифта

public:
	static constexpr int VARIABLE_HEIGHT = 0;	///< Значение параметра height для шрифта с индивидуальной высотой символа
	static constexpr int VARIABLE_WIDTH = 0;	///< Значение параметра width для шрифта с индивидуальной шириной символа
	enum Flags : uint8_t
	{
		NORMAL			= 0x00,		///< Нормальный шрифт
		ESCAPE_SYMBOLS	= 0x01,		///< Содержатся символы с кодами 0x00..0x1А
		MIDDLE_SYMBOLS	= 0x02,		///< Содержатся символы с кодами 0x80..0xBF
		NO_INDEX		= 0x04,		///< Шрифт не содержит индекса
	};

public:
	Font(int flags, int width, int height, IndexType index, DataType data);
	Font(int flags, int width, int height, DataType data);
	~Font();

	inline int getCharCount() const;

	bool isFixedHeight() const;
	int height() const;
	int height(Char ch) const;

	bool isMonospace() const;
	int width() const;
	int width(Char ch) const;
	int width(const char* str) const;

protected:
	inline bool isContainEscapeSymbols() const;
	inline bool isContainMiddleSymbols() const;
	inline bool isNoIndex() const;

public:
	int charToIndex(Char ch) const;
	const uint8_t* getCharDataAddr(Char ch) const;
	static Char extractMultibyteChar(const char** ps);

private:
	const uint16_t* _index;		///< Указатель на индекс шрифта
	const uint8_t* _data;		///< Указатель на изображение шрифта
	union
	{
		uint8_t _flags;			///< Флаги шрифта
		struct
		{
			uint8_t _escapeSymbols : 1;	///< Содержит символы с кодами 0x00..0x1А
			uint8_t _middleSymbols : 1;	///< Содержит символы с кодами 0x80..0xBF
			uint8_t _noIndex : 1;		///< Не содержит индекса
		};
	};
	uint16_t _count;			///< Количество символов в шрифте
	uint8_t _width;				///< Ширина шрифта (при моноширном шрифте)
	uint8_t _height;			///< Высота шрифта (при моновысотном шрифте)
};


/// Ко-во символов в шрифте
int Font::getCharCount() const
{
	return _count;
}

/// Содержатся ли символы с кодами 0x00..0x1А
bool Font::isContainEscapeSymbols() const
{
	return _escapeSymbols != 0;
//	return _flags & ESCAPE_SYMBOLS;
}

/// Содержатся ли символы с кодами 0x80..0xBF
bool Font::isContainMiddleSymbols() const
{
	return _middleSymbols != 0;
//	return _flags & MIDDLE_SYMBOLS;
}

/// Содержится ли индексная таблица
bool Font::isNoIndex() const
{
	return _noIndex != 0;
//	return _flags & NO_INDEX;
}

} // namespace gl
} // namespace kip5



#endif /* GL_INC_FONT_HPP_ */
