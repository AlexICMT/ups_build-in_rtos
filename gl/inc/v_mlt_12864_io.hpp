/* ups_build-in
 * /ups_build-in/test_inc_hpp/v_mlt_12864_io.hpp
 * v_mlt_12864_io.hpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 14:08:37
 *      Author: AlexICMT
 */

#ifndef TEST_INC_HPP_V_MLT_12864_IO_HPP_
#define TEST_INC_HPP_V_MLT_12864_IO_HPP_

#include "v_mlt_base.hpp"
#include "port.hpp"
#include "utils.hpp"
#include "view_ctrl_bus.hpp"

#define NEW_LCD_RST_DEBUG 1

namespace kip5 {
namespace v_mlt {

class Mlt_12864_io : public LcdIo
{
	using Port = volatile MDR_PORT_TypeDef*;
	using Bit = unsigned;

	static constexpr volatile uint32_t* rxtx(volatile MDR_PORT_TypeDef* p)
	{
		return reinterpret_cast<volatile uint32_t*>(p);
	}

public:
	/**
	 * Конструктор принимающий два дополнительных порта и бит шины управления ЖКИ
	 */
	constexpr Mlt_12864_io(view_ctrl_bus::View_Ctrl_Bus& bus, view_ctrl_bus::OutExtPort& ep1, view_ctrl_bus::OutExtPort& ep2, Port portE, Bit bitE)
		: _bus  {bus}
		, _ep1  {ep1}
		, _ep2  {ep2}
		, _e    {utils::bb(rxtx(portE), bitE)}
		, _busy {utils::bb(&MDR_PORTA->RXTX, 7)}//{utils::bb(rxtx(MDR_PORTA),PORT_Pin_7)}
	{
		init();
	}
	~Mlt_12864_io() = default;

	bool writeCommand(Data command) override;
	Data readStatus() override;

	bool writeData(Data data) override;
	Data readData() override;

	void takeBus(int n) override;
	void releaseBus() override;

	void reset(bool on) override;

	void backlight(bool on) override;


protected:
	void init();

private:
	#ifdef NEW_LCD_RST_DEBUG
		inline void setE()						{MDR_PORTB->RXTX = PORT_Pin_7 | (MDR_PORTB->RXTX & (~JTAG_PINS(MDR_PORTB)));}//{PORT_SetBits(MDR_PORTB, 7);}так не работает почему то
		inline void clrE()						{MDR_PORTB->RXTX &= ~(PORT_Pin_7 | JTAG_PINS(MDR_PORTB));}//{PORT_ResetBits(MDR_PORTB, 7);}так не работает почему то
	#else
		inline void setE()						{*_e = 1;}
		inline void clrE()						{*_e = 0;}
	#endif

	inline void enable1(bool f)				{_ep1.setBit(2, f);}
	inline void enable2(bool f)				{_ep1.setBit(3, f);}

	inline void commandMode()				{_ep1.clrBit(1);}
	inline void dataMode()					{_ep1.setBit(1);}

	inline void writeMode()					{_ep1.clrBit(0);}
	inline void readMode()					{_ep1.setBit(0);}

	inline void setReset(bool f = true)		{_ep1.setBit(4, !f);}
	inline void clrReset()					{setReset(false);}

	inline void setBackLight(bool f = true)	{_ep1.setBit(5, f);}
	inline void clrBackLight()				{setBackLight(false);}

//	inline bool readBusy()					{return OExtPort::readDataBus() & 0x80;}
	inline bool readBusy()					{return *_busy;}

	inline void setPortInputMode()			{_bus.inputMode();}
	inline void setPortOutputMode()			{_bus.outputMode();}

	inline uint32_t readPort()				{return _bus.read();}
	inline void writePort(uint32_t data)	{_bus.write(data);}

	inline void lockBus()					{_bus.lock();}
	inline void unlockBus()					{_bus.unlock();}

	void writeSequence(Data data);
	Data readSequence();

	bool waitBusy();

private:
	void Delay20ns();
	void Delay450ns();

private:
	view_ctrl_bus::View_Ctrl_Bus& _bus;
	view_ctrl_bus::OutExtPort& _ep1;
	view_ctrl_bus::OutExtPort& _ep2;

	typedef volatile uint32_t* const O;
	typedef volatile const uint32_t* const I;
	typedef volatile uint32_t* const IO;

	O _e    {nullptr};
	I _busy {nullptr};
};




} // namespace v_mlt
} // namespace kip5

#endif /* TEST_INC_HPP_V_MLT_12864_IO_HPP_ */
