/**************************************************************************//**
 * @file font8n_min.cpp
 * @date 01.12.2004
 * @author vlunyov
 * @brief Font created by the program CreateFont.exe
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 * @note
 * <h2><center>&copy; Copyright KW-Systems</center></h2>
 * <h2><center><a href="https://kwsystems.ru/">KW-Systems</a></center></h2>
 *
 *****************************************************************************/

#include "fonts.hpp"

namespace kip5 {
namespace resources {

// Изображение: font8n_min.bmp (256*256)
// Ко-во символов: 160
// Ко-во байт: 828
// Символы с 0 в начале 1
// Символы с 0 в конце 158

gl::Font font8n_min
	{gl::Font::NORMAL, gl::Font::VARIABLE_WIDTH, 8,
			{
				2, 0x00, 0x00,										// ' '
				2, 0x5F, 0x00,										// '!'
				4, 0x03, 0x00, 0x03, 0x00,							// '"'
				6, 0x14, 0x3E, 0x14, 0x3E, 0x14, 0x00,				// '#'
				6, 0x04, 0x2A, 0x7F, 0x2A, 0x10, 0x00,				// '$'
				5, 0x63, 0x18, 0x0C, 0x63, 0x00,					// '%'
				6, 0x3A, 0x45, 0x5A, 0x20, 0x50, 0x00,				// '&'
				2, 0x03, 0x00,										// '''
				3, 0x3E, 0x41, 0x00,								// '('
				3, 0x41, 0x3E, 0x00,								// ')'
				4, 0x2A, 0x1C, 0x2A, 0x00,							// '*'
				4, 0x08, 0x1C, 0x08, 0x00,							// '+'
				3, 0x40, 0x20, 0x00,								// ','
				3, 0x08, 0x08, 0x00,								// '-'
				2, 0x40, 0x00,										// '.'
				4, 0x60, 0x18, 0x07, 0x00,							// '/'
				4, 0x3E, 0x41, 0x3E, 0x00,							// '0'
				4, 0x42, 0x7F, 0x40, 0x00,							// '1'
				4, 0x66, 0x51, 0x4E, 0x00,							// '2'
				4, 0x22, 0x49, 0x36, 0x00,							// '3'
				4, 0x1C, 0x12, 0x7F, 0x00,							// '4'
				4, 0x27, 0x45, 0x39, 0x00,							// '5'
				4, 0x3E, 0x49, 0x32, 0x00,							// '6'
				4, 0x03, 0x71, 0x0F, 0x00,							// '7'
				4, 0x36, 0x49, 0x36, 0x00,							// '8'
				4, 0x26, 0x49, 0x3E, 0x00,							// '9'
				2, 0x24, 0x00,										// ':'
				3, 0x40, 0x24, 0x00,								// ';'
				3, 0x08, 0x14, 0x00,								// '<'
				3, 0x14, 0x14, 0x00,								// '='
				3, 0x14, 0x08, 0x00,								// '>'
				4, 0x02, 0x59, 0x06, 0x00,							// '?'
				8, 0x3C, 0x42, 0x99, 0xA5, 0x99, 0x22, 0x1C, 0x00,	// '@'
				4, 0x7E, 0x11, 0x7E, 0x00,							// 'A'
				4, 0x7F, 0x49, 0x36, 0x00,							// 'B'
				4, 0x3E, 0x41, 0x22, 0x00,							// 'C'
				4, 0x7F, 0x41, 0x3E, 0x00,							// 'D'
				4, 0x7F, 0x49, 0x41, 0x00,							// 'E'
				4, 0x7F, 0x09, 0x01, 0x00,							// 'F'
				4, 0x3E, 0x51, 0x72, 0x00,							// 'G'
				4, 0x7F, 0x08, 0x7F, 0x00,							// 'H'
				2, 0x7F, 0x00,										// 'I'
				4, 0x20, 0x41, 0x3F, 0x00,							// 'J'
				4, 0x7F, 0x0C, 0x73, 0x00,							// 'K'
				4, 0x7F, 0x40, 0x40, 0x00,							// 'L'
				6, 0x7F, 0x0C, 0x30, 0x0C, 0x7F, 0x00,				// 'M'
				5, 0x7F, 0x06, 0x18, 0x7F, 0x00,					// 'N'
				4, 0x3E, 0x41, 0x3E, 0x00,							// 'O'
				4, 0x7F, 0x11, 0x0E, 0x00,							// 'P'
				5, 0x3E, 0x61, 0x7E, 0x40, 0x00,					// 'Q'
				4, 0x7F, 0x11, 0x6E, 0x00,							// 'R'
				4, 0x26, 0x49, 0x32, 0x00,							// 'S'
				4, 0x01, 0x7F, 0x01, 0x00,							// 'T'
				4, 0x3F, 0x40, 0x7F, 0x00,							// 'U'
				4, 0x0F, 0x70, 0x0F, 0x00,							// 'V'
				6, 0x0F, 0x70, 0x0F, 0x70, 0x0F, 0x00,				// 'W'
				4, 0x73, 0x0C, 0x73, 0x00,							// 'X'
				4, 0x07, 0x78, 0x07, 0x00,							// 'Y'
				4, 0x61, 0x5D, 0x43, 0x00,							// 'Z'
				3, 0x7F, 0x41, 0x00,								// '['
				4, 0x07, 0x18, 0x60, 0x00,							// '\'
				3, 0x41, 0x7F, 0x00,								// ']'
				4, 0x02, 0x01, 0x02, 0x00,							// '^'
				2, 0x80, 0x80,										// '_'
				3, 0x01, 0x02, 0x00,								// '`'
				4, 0x30, 0x4A, 0x7C, 0x00,							// 'a'
				4, 0x7F, 0x48, 0x30, 0x00,							// 'b'
				4, 0x38, 0x44, 0x28, 0x00,							// 'c'
				4, 0x30, 0x48, 0x7F, 0x00,							// 'd'
				4, 0x38, 0x54, 0x58, 0x00,							// 'e'
				3, 0x7E, 0x05, 0x00,								// 'f'
				4, 0x98, 0xA4, 0x7C, 0x00,							// 'g'
				4, 0x7F, 0x08, 0x70, 0x00,							// 'h'
				2, 0x7A, 0x00,										// 'i'
				3, 0x80, 0x7A, 0x00,								// 'j'
				4, 0x7F, 0x10, 0x68, 0x00,							// 'k'
				2, 0x7F, 0x00,										// 'l'
				6, 0x7C, 0x04, 0x7C, 0x04, 0x78, 0x00,				// 'm'
				4, 0x7C, 0x04, 0x78, 0x00,							// 'n'
				4, 0x38, 0x44, 0x38, 0x00,							// 'o'
				4, 0x7C, 0x14, 0x08, 0x00,							// 'p'
				4, 0x08, 0x14, 0x7C, 0x00,							// 'q'
				3, 0x7C, 0x04, 0x00,								// 'r'
				4, 0x48, 0x54, 0x24, 0x00,							// 's'
				3, 0x3F, 0x42, 0x00,								// 't'
				4, 0x3C, 0x40, 0x7C, 0x00,							// 'u'
				4, 0x1C, 0x60, 0x1C, 0x00,							// 'v'
				6, 0x1C, 0x60, 0x1C, 0x60, 0x1C, 0x00,				// 'w'
				4, 0x6C, 0x10, 0x6C, 0x00,							// 'x'
				4, 0x4C, 0x50, 0x3C, 0x00,							// 'y'
				4, 0x64, 0x54, 0x4C, 0x00,							// 'z'
				4, 0x10, 0x6C, 0x82, 0x00,							// '{'
				2, 0xFF, 0x00,										// '|'
				4, 0x82, 0x6C, 0x10, 0x00,							// '}'
				5, 0x04, 0x02, 0x04, 0x02, 0x00,					// '~'
				5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,					// ''
				4, 0x7E, 0x11, 0x7E, 0x00,							// ''
				4, 0x7F, 0x49, 0x31, 0x00,							// ''
				4, 0x7F, 0x49, 0x36, 0x00,							// ''
				4, 0x7F, 0x01, 0x01, 0x00,							// ''
				6, 0xC0, 0x7E, 0x41, 0x7F, 0xC0, 0x00,				// ''
				4, 0x7F, 0x49, 0x41, 0x00,							// ''
				6, 0x73, 0x0C, 0x7F, 0x0C, 0x73, 0x00,				// ''
				4, 0x22, 0x49, 0x36, 0x00,							// ''
				5, 0x7F, 0x30, 0x0C, 0x7F, 0x00,					// ''
				5, 0x7E, 0x31, 0x0D, 0x7E, 0x00,					// ''
				4, 0x7F, 0x0C, 0x73, 0x00,							// ''
				5, 0x40, 0x3E, 0x01, 0x7F, 0x00,					// ''
				6, 0x7F, 0x06, 0x38, 0x06, 0x7F, 0x00,				// ''
				4, 0x7F, 0x08, 0x7F, 0x00,							// ''
				4, 0x3E, 0x41, 0x3E, 0x00,							// ''
				4, 0x7F, 0x01, 0x7F, 0x00,							// ''
				4, 0x7F, 0x09, 0x06, 0x00,							// ''
				4, 0x3E, 0x41, 0x22, 0x00,							// ''
				4, 0x01, 0x7F, 0x01, 0x00,							// ''
				4, 0x27, 0x48, 0x3F, 0x00,							// ''
				6, 0x0C, 0x12, 0x7F, 0x12, 0x0C, 0x00,				// ''
				4, 0x63, 0x1C, 0x63, 0x00,							// ''
				4, 0x7F, 0x40, 0xFF, 0x00,							// ''
				4, 0x07, 0x08, 0x7F, 0x00,							// ''
				6, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0x00,				// ''
				6, 0x7F, 0x40, 0x7F, 0x40, 0xFF, 0x00,				// ''
				5, 0x01, 0x7F, 0x48, 0x30, 0x00,					// ''
				6, 0x7F, 0x48, 0x30, 0x00, 0x7F, 0x00,				// ''
				4, 0x7F, 0x48, 0x30, 0x00,							// ''
				4, 0x22, 0x49, 0x3E, 0x00,							// ''
				6, 0x7F, 0x08, 0x3E, 0x41, 0x3E, 0x00,				// ''
				4, 0x76, 0x09, 0x7F, 0x00,							// ''
				4, 0x20, 0x54, 0x78, 0x00,							// ''
				4, 0x38, 0x54, 0x22, 0x00,							// ''
				4, 0x7C, 0x54, 0x28, 0x00,							// ''
				4, 0x7C, 0x04, 0x04, 0x00,							// ''
				6, 0xC0, 0x78, 0x44, 0x7C, 0xC0, 0x00,				// ''
				4, 0x38, 0x54, 0x58, 0x00,							// ''
				6, 0x6C, 0x10, 0x7C, 0x10, 0x6C, 0x00,				// ''
				4, 0x44, 0x54, 0x28, 0x00,							// ''
				5, 0x7C, 0x20, 0x10, 0x7C, 0x00,					// ''
				5, 0x7C, 0x21, 0x11, 0x7C, 0x00,					// ''
				4, 0x7C, 0x10, 0x6C, 0x00,							// ''
				5, 0x40, 0x38, 0x04, 0x7C, 0x00,					// ''
				6, 0x7C, 0x08, 0x30, 0x08, 0x7C, 0x00,				// ''
				4, 0x7C, 0x10, 0x7C, 0x00,							// ''
				4, 0x38, 0x44, 0x38, 0x00,							// ''
				4, 0x7C, 0x04, 0x7C, 0x00,							// ''
				4, 0x7C, 0x14, 0x08, 0x00,							// ''
				4, 0x38, 0x44, 0x28, 0x00,							// ''
				4, 0x04, 0x7C, 0x04, 0x00,							// ''
				4, 0x4C, 0x50, 0x3C, 0x00,							// ''
				6, 0x08, 0x14, 0x7C, 0x14, 0x08, 0x00,				// ''
				4, 0x6C, 0x10, 0x6C, 0x00,							// ''
				4, 0x7C, 0x40, 0xFC, 0x00,							// ''
				4, 0x0C, 0x10, 0x7C, 0x00,							// ''
				6, 0x7C, 0x40, 0x7C, 0x40, 0x7C, 0x00,				// ''
				6, 0x7C, 0x40, 0x7C, 0x40, 0xFC, 0x00,				// ''
				5, 0x04, 0x7C, 0x50, 0x20, 0x00,					// ''
				6, 0x7C, 0x50, 0x20, 0x00, 0x7C, 0x00,				// ''
				4, 0x7C, 0x50, 0x20, 0x00,							// ''
				4, 0x44, 0x54, 0x38, 0x00,							// ''
				6, 0x7C, 0x10, 0x38, 0x44, 0x38, 0x00,				// ''
				4, 0x68, 0x14, 0x7C, 0x00,							// ''
				0
			}
	};

} // namespace resources
} // namespace kws
