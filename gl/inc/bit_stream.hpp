/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/bit_stream.hpp
 * bit_stream.hpp
 * 
 *  Created on: 14 ���. 2021 �.
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
 * $Date: 2017-01-11 16:28:24 +0300 (��, 11 ��� 2017) $
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
 * @brief ��������� ������ ������
 * @details
 * ������������� ����� ����������� ��������� ������ ������ �� ������.<br/>
 * ������ ����� �� ������ ���� ��� �� ��������� ����� ���, �� �� ����� 31 ��� �� ���� ���.
 */
class InputBitStream
{
	typedef uint32_t Data;
	typedef uint32_t Buf;
	typedef uint8_t Bits;
	constexpr static Bits bits = sizeof(Buf) * 8;
	const Buf* ptr;			///< ��������� �� ������� ������
	Buf buf = 0;			///< ���������� �����
	Bits bit = 0;			///< ������� �������� ����

public:
	/**
	 * @brief �����������
	 * @param p ��������� �� ������
	 */
	InputBitStream(const void* p) : ptr((const Buf*)p) {}

	/**
	 * @brief ������ ������ ����
	 * @return �������� ������������ ���� � 0-��� ���� ����������
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
	 * @brief ������ ���������� ���
	 * @param n ���������� ��� ��� ������ (�� 1 �� 31)
	 * @return �������� ����������� ��� ����������� �� 0-�� ����
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
 * @brief ��������� ������ ������ (������ �� ������ ����)
 * @details
 * ������������� ����� ����������� ��������� ������ ������ �� ������.<br/>
 * ������ ����� �� ������ ����.
 */
class InputOneBitStream
{
	typedef uint32_t Data;
	typedef uint32_t Buf;
	const Buf* ptr;			///< ��������� �� ������� ������
	Buf buf = 1;			///< ���������� �����

public:
	/**
	 * @brief �����������
	 * @param p ��������� �� ������
	 */
	InputOneBitStream(const void* p) : ptr((const Buf*)p) {}

	/**
	 * @brief ������ ������ ����
	 * @return �������� ������������ ���� � 0-��� ���� ����������
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
 * @brief ��������� ������ ������
 * @details
 * ������������� ����� ����������� ��������� ������ ������ �� ������.<br/>
 * ������ ����� �� ������ ���� ��� �� ��������� ����� ���, �� �� ����� 31 ��� �� ���� ���.
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
	const Ptr* ptr;			///< ��������� �� ������� ������
	Buf buf = 0;			///< ���������� �����
//	Mask mask = 0;			///< ����� �����

public:
	/**
	 * @brief �����������
	 * @param p ��������� �� ������
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
	 * @brief ������ ���������� ���
	 * @param n ���������� ��� ��� ������ (�� 0 �� 8)
	 * @return �������� ����������� ��� ����������� �� �������� ����
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
	 * @brief �������� � ������ ������� ����
	 * @param n ��-�� ��� ��� ������� (�� 0 �� 7)
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
