/*
 * utils.hpp
 *
 *  Created on: 29/10/2021.
 *      Author: AlexICMT
 */

#ifndef INC_UTILS_INC_UTILS_HPP_
#define INC_UTILS_INC_UTILS_HPP_

#include <stdint.h>
#include <stddef.h>
//#include "macros.hpp"
/*
 * RAM 0x20000000..0x200FFFFF (1MByte)	SRAM bit-band				the access of words
 * RAM 0x22000000..0x23FFFFFF (32MByte)	SRAM bit-band alias			the access of bits in SRAM bit-band from the access of words in SRAM bit-band alias
 * SFR 0x40000000..0x400FFFFF (1MByte)	Peripheral bit-band			the access of words
 * SFR 0x42000000..0x43FFFFFF (32MByte)	Peripheral bit-band alias	the access of bits in Peripheral bit-band from the access of words in Peripheral bit-band alias
 */

namespace kip5{
namespace utils{

	inline constexpr uintptr_t bit_banding(uintptr_t p, unsigned b)
	{
	#if __cplusplus >= 201402L
	#endif
	return 0x02000000u + (p & 0x60000000u) + (p & 0x000FFFFFu) * 32 + (b & 0x1Fu) * 4;
	}
	template<typename T>
	inline constexpr uint32_t* bit_banding(T* const p, unsigned b)
	{
		return reinterpret_cast<uint32_t*>(bit_banding(reinterpret_cast<uintptr_t>(p), b));
	}
	static inline constexpr uint32_t* bb(volatile uint32_t* p, unsigned b)
	{
		return reinterpret_cast<uint32_t*>(0x02000000u +
			(reinterpret_cast<uintptr_t>(p) & 0x60000000u) +
			(reinterpret_cast<uintptr_t>(p) & 0x000FFFFFu) * 32 +
			(b & 0x1Fu) * 4);
	}

/*
p is a number of byte in a register, e.g. &port->RXTX
b is a number of bit in a p byte.
bit_word_offset = (byte_offset * 32) + (bit_number * 4)
bit_word_addr = bit_band_base + bit_word_offset
bit_word_offset 	is a position of needed bit in the bit-band region;
bit_word_addr 		is address of word in bit-band alias region, reflecting a needed bit in the bit-band region;
bit_band_base 		is the started address of bit-band region;
byte_offset 		is a number of byte with needed bit in the bit-band region;
bit_number 		is a number of needed bit in a byte.
 */

	inline constexpr uint32_t insert(uint32_t x, uint32_t m, uint32_t v)
	{
	//	x ^= v; x &= ~m; x ^= v;
	//	return x;
		return (x & ~m) | (v & m);
	}
	static_assert(insert(0b00001111, 0b00111100, 0b10101010) == 0b00101011, "insert");

	// repeat2 algorithm:
	// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxAB
	// ABAB ABAB ABAB ABAB ABAB ABAB ABAB ABAB
	inline constexpr uint32_t repeat2(uint32_t x)
	{
		x &= 3;
		x |= x << 2;
		x |= x << 4;
		x |= x << 8;
		x |= x << 16;
		return x;
	}
	static_assert(repeat2(0) == 0x00000000, "repeat2");
	static_assert(repeat2(1) == 0x55555555, "repeat2");
	static_assert(repeat2(2) == 0xAAAAAAAA, "repeat2");
	static_assert(repeat2(3) == 0xFFFFFFFF, "repeat2");

	// Unpack algorithm:
	// xxxx xxxx xxxx xxxx ABCD EFGH IJKL MNOP
	// 0000 0000 ABCD EFGH 0000 0000 IJKL MNOP
	// 0000 ABCD 0000 EFGH 0000 IJKL 0000 MNOP
	// 00AB 00CD 00EF 00GH 00IJ 00KL 00MN 00OP
	// 0A0B 0C0D 0E0F 0G0H 0I0J 0K0L 0M0N 0O0P
	// AABB CCDD EEFF GGHH IIJJ KKLL MMNN OOPP

#if __cplusplus >= 201402L
inline constexpr uint32_t unpack(uint32_t x)
{
    x = ((x & 0xFF00) << 8) | (x & 0x00FF);
    x = ((x << 4) | x) & 0x0F0F0F0F;
    x = ((x << 2) | x) & 0x33333333;
    x = ((x << 1) | x) & 0x55555555;
    return (x << 1) | x;
}
static_assert(unpack(0x0000) == 0x00000000, "unpack");
static_assert(unpack(0x8001) == 0xC0000003, "unpack");
static_assert(unpack(0xFFFF) == 0xFFFFFFFF, "unpack");

#else
inline constexpr uint32_t unpack4(uint32_t x)
{
	return ((x & 0xFF00) << 8) | (x & 0x00FF);
}

inline constexpr uint32_t unpack3(uint32_t x)
{
	return ((unpack4(x) << 4) | unpack4(x)) & 0x0F0F0F0F;
}

inline constexpr uint32_t unpack2(uint32_t x)
{
	return ((unpack3(x) << 2) | unpack3(x)) & 0x33333333;
}

inline constexpr uint32_t unpack1(uint32_t x)
{
	return ((unpack2(x) << 1) | unpack2(x)) & 0x55555555;
}

inline constexpr uint32_t unpack(uint32_t x)
{
	return ((unpack1(x) << 1) | unpack1(x));
}
#endif // __cplusplus >= 201402L

} //namespace kip5
} //namespace utils



#endif /* INC_UTILS_INC_UTILS_HPP_ */
