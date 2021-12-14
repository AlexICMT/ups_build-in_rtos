/* ups_build-in
 * /ups_build-in/src/view_ctrl_bus_src/view_ctrl_bus.cpp
 * view_ctrl_bus.cpp
 * 
 *  Created on: 10 нояб. 2021 г.
 *  	  Time: 15:29:34
 *      Author: AlexICMT
 */

#include "view_ctrl_bus.hpp"

namespace kip5{
namespace view_ctrl_bus{

View_Ctrl_Bus::View_Ctrl_Bus(Port port) : _port(port)
{
	port::enableClkToPort(_port);
	_port->OE &= ~_mask;							// Режим ввода
//	_port->FUNC &= ~(0xFFFFFFFF & unpack(_mask));	// Функция порта
//	_port->FUNC &= ~(repeat2(PORT_FUNC_PORT) & unpack(_mask));						// Функция порта
	_port->FUNC = utils::insert(_port->FUNC, utils::unpack(_mask), utils::repeat2(PORT_FUNC_PORT));		// Функция порта
	_port->ANALOG |= _mask;							// Цифровой
//	_port->PWR |= 0xAAAAAAAA & unpack(_mask);		// Быстрый фронт (порядка 20нс)
//	_port->PWR |= repeat2(PORT_SPEED_FAST) & unpack(_mask);							// Быстрый фронт (порядка 20нс)
	_port->PWR = utils::insert(_port->PWR, utils::unpack(_mask), utils::repeat2(PORT_SPEED_MAXFAST));	// Максимально быстрый фронт (порядка 10нс)
	_port->PULL |= _mask << 16;						// Подтяжка к VCC
}

void OutExtPort::write(Data data)
{
	_buf = data;
	_dataBus.lock();
	_dataBus.write(data);
	_dataBus.outputMode();
	delay();
	_latch.set();
	delay();
	_latch.clr();
	delay();
	_dataBus.inputMode();
	_dataBus.unlock();
}

} //namespace kip5
} //namespace view_ctrl_bus


