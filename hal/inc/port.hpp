/*
 * port.hpp
 *
 *  Created on: 28/10/2021.
 *      Author: AlexICMT
 */

#ifndef INC_PORT_INC_PORT_HPP_
#define INC_PORT_INC_PORT_HPP_

//#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "utils.hpp"

namespace kip5{
namespace port{

//void enableClkToAllPorts(bool enable = true);
void enableClkToPort(MDR_PORT_TypeDef* port, bool enable = true);


class Port
{
public:
	Port(MDR_PORT_TypeDef* port,unsigned bit)
	: _port(port), _mask(bit) {enableClkToPort(port);}; //_mask(1u << bit)
	Port(const Port&) = delete;
protected:
	MDR_PORT_TypeDef* volatile _port; //MDR_PORT_TypeDef* volatile _port
	uint32_t _mask;
}; //class Port

class OutPort : public Port
{
public:
	OutPort(MDR_PORT_TypeDef* port, unsigned bit);
	OutPort(const OutPort&) = delete;
	inline void set() const {PORT_SetBits(_port,_mask);} //_port->RXTX |= _mask;
	inline void clr() const {PORT_ResetBits(_port,_mask);} //_port->RXTX |= _mask;
	inline const OutPort& operator=(bool on) const //overload =
	{
		if(on)
			set();
		else
			clr();
		return *this;
	}
};

class InPort : public Port
{
public:
	InPort(MDR_PORT_TypeDef* port, unsigned bit);
	InPort(const InPort&) = delete;
	inline bool get() const {return (_port->RXTX & _mask) != 0;}
	inline operator bool() const {return get();} //overload bool
};

class PortBitBase
{
public:
	PortBitBase(MDR_PORT_TypeDef* port, unsigned bit);
	PortBitBase(const PortBitBase&) = default;
	inline void set(bool b = true) const {*_portBit = static_cast<uint32_t>(b);}
	inline void clr() const {set(false);}
	inline bool get() const {return static_cast<bool>(*_portBit);}
	inline void inv() const {set(!get());}
	inline uint32_t* getAddress() const {return _portBit;}
protected:
	uint32_t* volatile const _portBit;
};

class OutPortBit : public PortBitBase
{
public:
	OutPortBit(MDR_PORT_TypeDef* port, unsigned bit);
	OutPortBit(const OutPortBit&) = default;
	inline const PortBitBase& operator=(bool b) const {set(b); return *this;}
	inline operator bool() const {return get();}
};

class InPortBit : public PortBitBase
{
public:
	InPortBit(MDR_PORT_TypeDef* port, unsigned bit);
	InPortBit(const InPortBit&) = default;
	inline operator bool() const {return get();} //overload?
};

class PortBitNullBase
{
public:
	PortBitNullBase(MDR_PORT_TypeDef* port, unsigned bit) {(void)port; (void)bit;}
	PortBitNullBase(const PortBitNullBase&) = default;
	inline void set(bool b = true) const {(void)b;}
	inline void clr() const {}
	inline bool get() const {return false;}
	inline void inv() const {}
	inline uint32_t* getAddress() const {return nullptr;}
};

class OutPortBitNull : public PortBitNullBase
{
public:
	OutPortBitNull(MDR_PORT_TypeDef* port, unsigned bit) : PortBitNullBase(port, bit) {}
	OutPortBitNull(const OutPortBitNull&) = default;
	inline const OutPortBitNull& operator=(bool b) const {set(b); return *this;}
	inline operator bool() const {return get();}
};

class InPortBitNull : public PortBitNullBase
{
public:
	InPortBitNull(MDR_PORT_TypeDef* port, unsigned bit) : PortBitNullBase(port, bit) {}
	InPortBitNull(const InPortBitNull&) = default;
	inline operator bool() const {return get();}
};
} //namespace kip5
} //namespace port



#endif /* INC_PORT_INC_PORT_HPP_ */
