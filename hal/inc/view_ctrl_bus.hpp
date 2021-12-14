/* ups_build-in
 * /ups_build-in/inc/view_ctrl_bus_inc/view_ctrl_bus.hpp
 * view_ctrl_bus.hpp
 * 
 *  Created on: 10 нояб. 2021 г.
 *  	  Time: 15:28:42
 *      Author: AlexICMT
 */

#ifndef INC_VIEW_CTRL_BUS_INC_VIEW_CTRL_BUS_HPP_
#define INC_VIEW_CTRL_BUS_INC_VIEW_CTRL_BUS_HPP_

#include "port.hpp"
#include <cstdint>

namespace kip5{
namespace view_ctrl_bus{

using port::OutPortBit;
using utils::bb;

class View_Ctrl_Bus
{
	using Port = MDR_PORT_TypeDef*;			///< Тип порта ШД
	using Data = uint32_t;					///< Тип параметров ШД
	using Bus = uint32_t;					///< Тип ШД
	static constexpr Bus _mask = 0xFF;		///< Маска ШД
public:
	View_Ctrl_Bus(Port port);
	View_Ctrl_Bus(const View_Ctrl_Bus&) = delete;

	inline void write(Data data)
	{
		//PORT_SetBits(_port, ~_mask);
		//PORT_SetBits(_port, data & _mask);
		_port->RXTX = (_port->RXTX & ~_mask) | (data & _mask);
	}

	inline Data read()
	{
		//return PORT_ReadInputData(_port) & _mask;
		return _port->RXTX & _mask;
	}

	inline void setMode(Data out)
	{
		_port->OE = (_port->OE & ~_mask) | (out & _mask);
	}

	inline void outputMode()
	{
		_port->OE |= _mask;
	}

	inline void inputMode()
	{
		_port->OE &= ~_mask;
	}

	void lock() {_lock++;}
	void unlock() {_lock--;}
	bool locked() const {return _lock > 0;}

private:
	volatile Port _port;			///< Порт ШД
	volatile int8_t _lock = 0;		///< Индикатор занятости ШД

}; // class View_Ctrl_Bus

class OutExtPort
{
	using Data = uint32_t;					///< Тип параметров
	using Buf = uint8_t;					///< Тип буфера дополнительного порта
	using Port = MDR_PORT_TypeDef*;			///< Тип порта ШД
	using Bit = uint8_t;					///< Тип номера бита
public:
	OutExtPort(View_Ctrl_Bus& dataBus, Port port, Bit bit, Data data = 0) : _dataBus(dataBus), _latch(port, bit)
	{
		write(data);
	}
	OutExtPort(View_Ctrl_Bus& dataBus, OutPortBit& latch, Data data = 0) : _dataBus(dataBus), _latch(latch)
	{
		write(data);
	}
	OutExtPort(const OutExtPort&) = delete;

	void write(Data data);

	inline Data read() const
	{
		return _buf;
	}

	inline void setBit(int bit)
	{
		write(read() | (1 << bit));
	}

	inline void setBit(int bit, bool f)
	{
		write((read() & ~(1 << bit)) | (f << bit));
	}

	inline void clrBit(int bit)
	{
		write(read() & ~(1 << bit));
	}

	inline void invBit(int bit)
	{
		write(read() ^ (1 << bit));
	}

	inline bool readBit(int bit) const
	{
		return read() >> bit;
	}

	inline OutExtPort& operator=(Data data) {write(data); return *this;}
	inline operator Data() const {return read();}

protected:
	inline static void delay() {__NOP(); __NOP(); __NOP(); __NOP();}

private:
	View_Ctrl_Bus& _dataBus;			///< Порт шины данных
	OutPortBit _latch;			///< Строб регистра дополнительного порта
	Data _buf = 0x00;
};

class OutExtPortBit
{
	using Bit = uint8_t;		///< Тип номера бита
public:
	OutExtPortBit(OutExtPort& oExtPort, Bit bit) : _oExtPort(oExtPort), _bit(bit) {}
	OutExtPortBit(const OutExtPortBit&) = delete;
	inline void set(bool b = true) const {_oExtPort.setBit(_bit, b);}
	inline void clr() const {_oExtPort.clrBit(_bit);}
	inline void inv() const {_oExtPort.invBit(_bit);}
	inline bool get() const {return _oExtPort.readBit(_bit);}
	inline operator bool() const {return get();}
	inline OutExtPortBit& operator=(bool b) {set(b); return *this;}
private:
	OutExtPort& _oExtPort;		///< Дополнительный порт вывода
	Bit _bit;					///< Номер бита
};


} // namespace kip5
} // namespace view_ctrl_bus

#endif /* INC_VIEW_CTRL_BUS_INC_VIEW_CTRL_BUS_HPP_ */
