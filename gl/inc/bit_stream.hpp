/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/bit_stream.hpp
 * bit_stream.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:32:21
 *      Author: AlexICMT
 *
 * History:
 *
 * @file bit_stream.hpp
 * @date 08.09.2015
 * @author vlunyov
 * @brief Bit stream
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#ifndef GL_INC_BIT_STREAM_HPP_
#define GL_INC_BIT_STREAM_HPP_

#include <cstdint>
#include <cstddef>
#include <cassert>

namespace kip5 {
namespace utils {

/**
 * @brief Побитовое чтение данных
 * @details
 * Универсальный класс позволяющий побитовое чтение данных из памяти.<br/>
 * Читать можно по одному биту или по заданному числу бит, но не более 31 бит за один раз.
 */
class InputBitStream
{
	typedef uint32_t Data;
	typedef uint32_t Buf;
	typedef uint8_t Bits;
	constexpr static Bits bits = sizeof(Buf) * 8;
	const Buf* ptr;			///< Указатель на входные данные
	Buf buf = 0;			///< Внутренний буфер
	Bits bit = 0;			///< Позиция текущего бита

public:
	/**
	 * @brief Конструктор
	 * @param p Указатель на данные
	 */
	InputBitStream(const void* p) : ptr((const Buf*)p) {}

	/**
	 * @brief Чтение одного бита
	 * @return Значение прочитанного бита в 0-вом бите результата
	 */
	inline Data readBit()
	{
		if(bit == 0)
		{
			buf = *ptr++;
			bit = bits;
		}
		Data result = buf & 1;
		buf >>= 1;
		bit--;
		return result;
	}

	/**
	 * @brief Чтение нескольких бит
	 * @param n Количество бит для чтения (от 1 до 31)
	 * @return Значение прочитанных бит выровненных по 0-му биту
	 */
	inline Data readBits(Bits n)
	{
		assert(n <= 31);
		Data result = 0;
		if(n > bit)
		{
			n -= bit;
			result = buf << n;
			buf = *ptr++;
			bit = bits;
		}
		result |= buf & (((Data)1 << n) - 1);
		buf >>= n;
		bit -= n;
		return result;
	}
};

/**
 * @brief Побитовое чтение данных (только по одному биту)
 * @details
 * Универсальный класс позволяющий побитовое чтение данных из памяти.<br/>
 * Читать можно по одному биту.
 */
class InputOneBitStream
{
	typedef uint32_t Data;
	typedef uint32_t Buf;
	const Buf* ptr;			///< Указатель на входные данные
	Buf buf = 1;			///< Внутренний буфер

public:
	/**
	 * @brief Конструктор
	 * @param p Указатель на данные
	 */
	InputOneBitStream(const void* p) : ptr((const Buf*)p) {}

	/**
	 * @brief Чтение одного бита
	 * @return Значение прочитанного бита в 0-вом бите результата
	 */
	inline Data readBit()
	{
		if(buf == 1)
		{
			buf = *ptr++;
			Data result = buf & 1;
			buf >>= 1;
			buf |= 1ul << 31;
			return result;
		}
		Data result = buf & 1;
		buf >>= 1;
		return result;
	}
};

/**
 * @brief Побитовое чтение данных
 * @details
 * Универсальный класс позволяющий побитовое чтение данных из памяти.<br/>
 * Читать можно по одному биту или по заданному числу бит, но не более 31 бит за один раз.
 */
class InputBitStream2
{
	using Data = uint32_t;
	using Ptr = uint8_t;
	using Buf = uint32_t;
	using Mask = uint32_t;
	using Bits = uint_fast8_t;
	constexpr static Bits bits = sizeof(Ptr) * 8;
	constexpr static Mask all = (1ul << bits) - 1;
	const Ptr* ptr;			///< Указатель на входные данные
	Buf buf = 0;			///< Внутренний буфер
//	Mask mask = 0;			///< Маска битов

public:
	/**
	 * @brief Конструктор
	 * @param p Указатель на данные
	 */
	InputBitStream2(const void* p) : ptr((const Ptr*)p)
	{
		buf = *ptr++;
		buf |= all << 16;
	}


	inline Data readByte()
	{
		Data result = 0;

		return result;
	}

	/**
	 * @brief Чтение нескольких бит
	 * @param n Количество бит для чтения (от 0 до 8)
	 * @return Значение прочитанных бит выровненных по младшему биту
	 */
	inline Data readBits(int n)
	{
		assert(n <= 8);
		Mask m = (1ul << n) - 1;
//		if(m > mask)
//		{
//			buf = (buf << bits) | *ptr++;
//			mask = (mask << bits) | all;
//		}
//		m &= mask;
		Data result = (buf & m) | (m << 16);
		buf >>= n;
		return result;
	}

	/**
	 * @brief Вставить в начало нулевые биты
	 * @param n Ко-во бит для вставки (от 0 до 7)
	 */
	inline void insertDummyBits(int n)
	{
		assert(n <= 7);
		buf <<= n;
	}
};

// TODO May be use __CLZ() function (returns number of leading zeros)

} // namespace utils
} // namespace kip5




#endif /* GL_INC_BIT_STREAM_HPP_ */
