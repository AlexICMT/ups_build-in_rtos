/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/utf8.cpp
 * utf8.cpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:42:38
 *      Author: AlexICMT
 *
 * History:
 *
 * @file utf8.cpp
 * @date 06.12.2016
 * @author vlunyov
 * @brief UTF-8
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#include "utf8.hpp"

namespace kip5 {
namespace unicode {

/**
 * Вычисление количества символов в UTF-8 строке
 * @param str Указатель на строку UTF-8
 * @return Количество символов
 */
size_t strlen_utf8(const char* str)
{
	size_t len {0};
	while(*str)
	{
		str += symbol_size_utf8(*str);
		len++;
	}
	return len;
}

/**
 * Извлекает UTF-32 символ из строки UTF-8 символов
 * @param pstr Указатель на указатель на строку UTF-8 (инкрементируется на ко-во октетов в UTF-8 символе)
 * @return UTF-32 символ
 */
char32_t extract_utf8(const char** pstr)
{
	char32_t utf8 = (unsigned char)*(*pstr)++;
	if((utf8 & 0x80) == 0)
		return utf8;
	auto size = symbol_size_utf8(utf8);
	if(size > 1)
	{
		utf8 &= (1ul << (7-size))-1;
		for(decltype(size) i = 0; i < size-1; ++i)
			utf8 = (utf8 << 6) | (*(*pstr)++ & 0x3F);
	}
	return utf8;
}

} // namespace unicode
} // namespace kip5


