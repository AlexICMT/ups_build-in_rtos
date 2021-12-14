/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/utf8.hpp
 * utf8.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:41:51
 *      Author: AlexICMT
 *
 * History:
 *
 * @file utf8.hpp
 * @date 06.12.2016
 * @author vlunyov
 * @brief UTF-8
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#ifndef GL_INC_UTF8_HPP_
#define GL_INC_UTF8_HPP_

#include <cstdint>
#include <cstddef>

namespace kip5 {
namespace unicode {

/**
 * Вычисление количества лидирующих нулей (использует ассемблерную команду Cortex M3 CLZ)
 * @param value Входной параметр
 * @return Количество лидирующих нулей (0..32)
 */
__attribute__((always_inline)) inline uint32_t CLZ(uint32_t value)
{
	uint32_t result;
	__asm volatile("clz %0, %1" : "=r" (result) : "r" (value));
	return result;
}

/**
 * @brief Вычисление размера символа UTF-8
 *
 * clz=0: mask=0xxxxxxx size=1 ASCII symbol
 * clz=1: mask=10xxxxxx size=1 Error (Next UTF-8 octet)
 * clz=2: mask=110xxxxx size=2 First UTF-8 octet for 2 octets symbol
 * clz=3: mask=1110xxxx size=3 First UTF-8 octet for 3 octets symbol
 * clz=4: mask=11110xxx size=4 First UTF-8 octet for 4 octets symbol
 * clz=5: mask=111110xx size=1 Error
 * clz=6: mask=1111110x size=1 Error
 * clz=7: mask=11111110 size=1 Error
 * clz=8: mask=11111111 size=1 Error
 *
 * @param first_byte Первый байт символа в кодировке UTF-8
 * @return Размер символа UTF-8 в байтах (1…4)
 */
inline size_t symbol_size_utf8(char first_byte)
{
	uint32_t clz = CLZ(~((uint32_t)first_byte << 24));
	if(clz == 0)
		return 1;			// ASCII symbol
	if(clz == 1 || clz > 4)
		return 1;			// Error
	return clz;				// 2..4 octets UTF-8 symbol
}

size_t strlen_utf8(const char* str);
char32_t extract_utf8(const char** pstr);

} // namespace unicode
} // namespace kip5

#endif /* GL_INC_UTF8_HPP_ */
