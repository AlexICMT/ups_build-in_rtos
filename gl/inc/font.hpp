/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/font.hpp
 * font.hpp
 * 
 *  Created on: 14 ���. 2021 �.
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
 * $Date: 2017-01-11 16:28:24 +0300 (��, 11 ��� 2017) $
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
 * @brief �����
 * @details
 * ������ ������:<br/>
 * f 1 ���� - �����:<br/>
 *            ��� 0 - ������� �������� � ������ 0x00..0x1F<br/>
 *            ��� 1 - ������� �������� � ������ 0x80..0xBF<br/>
 * n 1 ���� - ��-�� ��������<br/>
 * h 1 ���� - ������ �������� (���� 0, �� ������ ����������� �������� ��� ������� �������)<br/>
 * w 1 ���� - ������ �������� (���� 0, �� ������ ����������� �������� ��� ������� �������)<br/>
 * t 2 ����� * n - ������� �������� �������� (������������ ������ ��� f.7 = 1)<br/>
 * m x ���� - ����������� �������� (���� h=0, �� ���������� � ������ �������)<br/>
*/
class Font
{
	using Char = char16_t;
	using IndexType = std::initializer_list<uint16_t>;	///< ��� ������ ������� ������
	using DataType = std::initializer_list<uint8_t>;	///< ��� ������ ����������� ������

public:
	static constexpr int VARIABLE_HEIGHT = 0;	///< �������� ��������� height ��� ������ � �������������� ������� �������
	static constexpr int VARIABLE_WIDTH = 0;	///< �������� ��������� width ��� ������ � �������������� ������� �������
	enum Flags : uint8_t
	{
		NORMAL			= 0x00,		///< ���������� �����
		ESCAPE_SYMBOLS	= 0x01,		///< ���������� ������� � ������ 0x00..0x1�
		MIDDLE_SYMBOLS	= 0x02,		///< ���������� ������� � ������ 0x80..0xBF
		NO_INDEX		= 0x04,		///< ����� �� �������� �������
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
	const uint16_t* _index;		///< ��������� �� ������ ������
	const uint8_t* _data;		///< ��������� �� ����������� ������
	union
	{
		uint8_t _flags;			///< ����� ������
		struct
		{
			uint8_t _escapeSymbols : 1;	///< �������� ������� � ������ 0x00..0x1�
			uint8_t _middleSymbols : 1;	///< �������� ������� � ������ 0x80..0xBF
			uint8_t _noIndex : 1;		///< �� �������� �������
		};
	};
	uint16_t _count;			///< ���������� �������� � ������
	uint8_t _width;				///< ������ ������ (��� ���������� ������)
	uint8_t _height;			///< ������ ������ (��� ������������ ������)
};


/// ��-�� �������� � ������
int Font::getCharCount() const
{
	return _count;
}

/// ���������� �� ������� � ������ 0x00..0x1�
bool Font::isContainEscapeSymbols() const
{
	return _escapeSymbols != 0;
//	return _flags & ESCAPE_SYMBOLS;
}

/// ���������� �� ������� � ������ 0x80..0xBF
bool Font::isContainMiddleSymbols() const
{
	return _middleSymbols != 0;
//	return _flags & MIDDLE_SYMBOLS;
}

/// ���������� �� ��������� �������
bool Font::isNoIndex() const
{
	return _noIndex != 0;
//	return _flags & NO_INDEX;
}

} // namespace gl
} // namespace kip5



#endif /* GL_INC_FONT_HPP_ */
