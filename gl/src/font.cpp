/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/font.cpp
 * font.cpp
 * 
 *  Created on: 14 ���. 2021 �.
 *  	  Time: 14:35:56
 *      Author: AlexICMT
 *
 * History:
 *
 * @file font.cpp
 * @date 27.07.2015
 * @author vlunyov
 * @brief Font
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (��, 11 ��� 2017) $
 *
 */

#include "font.hpp"
#include "utf8.hpp"

namespace kip5 {
namespace gl {

/**
 * @brief ����������� ������ � ��������� ��������
 * @param flags �����
 * @param width ������ ��������
 * @param height ������ ��������
 * @param index ��������� �������
 * @param data ����������� ������
 */
Font::Font(int flags, int width, int height, IndexType index, DataType data)
	: _index(index.begin()), _data(data.begin())
	, _flags(flags), _count(index.size() - 1), _width(width), _height(height)
{
}

/**
 * @brief ����������� ������ ��� ��������� �������
 * @param flags �����
 * @param width ������ ��������
 * @param height ������ ��������
 * @param data ����������� ������
 */
Font::Font(int flags, int width, int height, DataType data)
	: _index(nullptr), _data(data.begin())
	, _flags(flags), _count(0), _width(width), _height(height)
{
	const uint8_t* pData = _data;
	while(*pData)
	{
		pData += *pData + 1;
		_count++;
	}
//	uint16_t* pIndex = new uint16_t[_count + 1];
//	_index = pIndex;
//	pData = _data;
//	*pIndex++ = 0;
//	while(*pData)
//	{
//		pData += *pData;
//		*pIndex++ = pData - _data;
//	}
	_flags |= NO_INDEX;
}

Font::~Font()
{
//	if(reinterpret_cast<intptr_t>(_index) >= 0x20000000)
//		delete[] _index;
}

/**
 * @brief ����� ���������� ����� �������
 * @param ch ������
 * @return ���������� ����� �������
 */
int Font::charToIndex(Char ch) const
{
	// UTF-8 to CP1251
	if(ch == 0x0401)
		ch = 0xA8;		// TODO Fix � in fonts
	if(ch == 0x0451)
		ch = 0xB8;		// TODO Fix � in fonts
	if(ch >= 0x0410)
		ch = ch - 0x0410 + 0xC0;	// �..�,�..�

	int charIndex = (unsigned)ch;
	if(!isContainMiddleSymbols())
		if(charIndex >= 0x80)
			charIndex ^= 0x40;
	if(!isContainEscapeSymbols())
		charIndex -= ' ';
	if(charIndex >= _count)
		charIndex = 0;
	return charIndex;
}

/**
 * @brief ����� ����� ����������� �������
 * @param ch ������
 * @return ��������� �� ����������� �������
 */
const uint8_t* Font::getCharDataAddr(Char ch) const
{
	int charIndex = charToIndex(ch);
	const uint8_t* pData = _data;
	if(!isNoIndex())
	{
		pData += _index[charIndex];
		if(!isFixedHeight())
			pData += 2;
	}
	else
	{
		while(charIndex--)
			pData += *pData + 1;
		pData++;
	}
	return pData;
}

Font::Char Font::extractMultibyteChar(const char** ps)
{
//	return ch = *(*ps)++;						// Octet string
	return (Char)unicode::extract_utf8(ps);		// Multibyte string
}

/// ������������� ������ ��������?
bool Font::isFixedHeight() const
{
	return _height != VARIABLE_HEIGHT;
}

/// ������ �������� � �������� (���� �������������, ����� 0)
int Font::height() const
{
	return _height;
}

/**
 * @brief ������ ������� � ��������
 * @param ch ������
 * @return ������ ������� � ��������
 */
int Font::height(Char ch) const
{
	if(isFixedHeight())
		return height();
	int charIndex = charToIndex(ch);
	int charHeight;
	const uint8_t* pData = &_data[_index[charIndex]];
	charHeight = pData[1];
	return charHeight;
}

/**
 * @brief ������������� ������ ��������?
 * @return true - ������������� ������ ��������
 */
bool Font::isMonospace() const
{
	return _width != VARIABLE_WIDTH;
}

/**
 * @brief ������ �������� � �������� (���� �������������, ����� 0)
 * @return ������
 */
int Font::width() const
{
	return _width;
}

/**
 * @brief ������ ������� � ��������
 * @param ch ������
 * @return ������
 */
int Font::width(Char ch) const
{
	if(isMonospace())
		return width();
	int charIndex = charToIndex(ch);
	if(charIndex < 0)
		return 0;
	int charWidth;
	if(!isNoIndex())
	{
		if(isFixedHeight())
		{
			int bytes = _index[charIndex + 1] - _index[charIndex];
			charWidth = (bytes * 8) / height(ch);
		}
		else
		{
			const uint8_t* pData = &_data[_index[charIndex]];
			charWidth = pData[0];
		}
	}
	else
	{
		const uint8_t* pData = _data;
		while(charIndex--)
			pData += *pData + 1;
		charWidth = *pData;
	}
	return charWidth;
}

/**
 * @brief ������ ������ � ��������
 * @param str ������
 * @return ������
 */
int Font::width(const char* str) const
{
	int w {0};
	while(Char ch = extractMultibyteChar(&str))
		w += width(ch);
	return w;
}

} // namespace gl
} // namespace kip5




