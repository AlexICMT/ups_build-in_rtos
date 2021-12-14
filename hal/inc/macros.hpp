/*
 * macros.hpp
 *
 *  Created on: 29/10/2021.
 *      Author: AlexICMT
 */

#ifndef INC_MACROS_INC_MACROS_HPP_
#define INC_MACROS_INC_MACROS_HPP_

#include "MDR32Fx.h"
#include "core_cm3.h"

namespace kip5 {
namespace macros {

#define BIT(x)		(1<<(x))
#define SET(x,y)	(x|=(y))
#define CLR(x,y)	(x&=(~(y)))
#define CHK(x,y)	(x&(y))
#define SETBIT(x,y)	(x|=(1<<(y)))
#define CLRBIT(x,y)	(x&=(~(1<<(y))))
#define XORBIT(x,y)	(x^=(1<<(y)))
#define CHKBIT(x,y)	(x&(1<<(y)))
#define BITIS1(x,y)	((x&(1<<(y)))!=0)
#define BITIS0(x,y)	((x&(1<<(y)))==0)
#define COPYBIT(x,xb,y,yb) if(x&(1<<(xb))) y|=(1<<(yb)); else y&=(~(1<<(yb)))

#define B(v,b7,b6,b5,b4,b3,b2,b1,b0)	((v>>7&1)<<b7|(v>>6&1)<<b6|(v>>5&1)<<b5|(v>>4&1)<<b4|(v>>3&1)<<b3|(v>>2&1)<<b2|(v>>1&1)<<b1|(v&1)<<b0)

#define SEI()		__enable_irq()
#define CLI()		__disable_irq()
#define NOP()		__NOP()

class Macros
{
public:
	Macros() = default;
	Macros(const Macros&) = delete;

	inline constexpr char Bits(void)
	{return 0;};
	inline constexpr char Bits(char x1)
	{return (1<<x1);}
	inline constexpr char Bits(char x1, char x2)
	{return (1<<x1)|(1<<x2);}
	inline constexpr char Bits(char x1, char x2, char x3)
	{return (1<<x1)|(1<<x2)|(1<<x3);}
	inline constexpr char Bits(char x1, char x2, char x3, char x4)
	{return (1<<x1)|(1<<x2)|(1<<x3)|(1<<x4);}
	inline constexpr char Bits(char x1, char x2, char x3, char x4, char x5)
	{return (1<<x1)|(1<<x2)|(1<<x3)|(1<<x4)|(1<<x5);}
	inline constexpr char Bits(char x1, char x2, char x3, char x4, char x5, char x6)
	{return (1<<x1)|(1<<x2)|(1<<x3)|(1<<x4)|(1<<x5)|(1<<x6);}
	inline constexpr char Bits(char x1, char x2, char x3, char x4, char x5, char x6, char x7)
	{return (1<<x1)|(1<<x2)|(1<<x3)|(1<<x4)|(1<<x5)|(1<<x6)|(1<<x7);}
	inline constexpr char Bits(char x1, char x2, char x3, char x4, char x5, char x6, char x7, char x8)
	{return (1<<x1)|(1<<x2)|(1<<x3)|(1<<x4)|(1<<x5)|(1<<x6)|(1<<x7)|(1<<x8);}

	inline constexpr unsigned char Bit(unsigned char b)
	{
		return 1 << b;
	}

	// �������������: OutPort(PORTB, Bit(3));
	inline void OutPort(unsigned char volatile *p, unsigned char v)
	{
		*p = v;
	}
};


} // namespace macros
} // namespace kip5





#endif /* INC_MACROS_INC_MACROS_HPP_ */
