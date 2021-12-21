/* ups_build_in_rtos
 * /ups_build_in_rtos/hal/src/framework.cpp
 * framework.cpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 10:57:08
 *      Author: AlexICMT
 */

#include "framework.hpp"
//#include "MDR32F9Qx_i2c.h"

kip5::view_ctrl_bus::OutExtPort ep1{bus, MDR_PORTE, 2};
kip5::view_ctrl_bus::OutExtPort ep2{bus, MDR_PORTE, 3};
kip5::init::Initialization init;
kip5::gl::FrameBuffer* frameBuffer = nullptr;
kip5::v_mlt::Mlt_12864_io* melt = nullptr;
kip5::v_mlt::Mlt_12864_base* lcd = nullptr;
kip5::gl::CharPainter* charPainterMin = nullptr;
kip5::adc::Adc* asensors_switch = nullptr;
kip5::dma::Dma* dma_adc_switch = nullptr;


	kip5::view_ctrl_bus::View_Ctrl_Bus bus{MDR_PORTA};
	kip5::button::Button buttons;

	volatile uint32_t keyboardPort = 0;


void readButtonPort()
{
	bus.write(0x7F);
	bus.setMode(0x80);
	for(int i = 0; i < 100; ++i) __NOP();
	keyboardPort = bus.read();
	bus.inputMode();
	bus.write(0xFF);
}

void buttonsInit()
{
	if(!bus.locked())
	{
		readButtonPort();
	}
	uint32_t buf = keyboardPort;
	buf >>= 2;
	buf = ~buf;
	buf &= kip5::KEY::ALL;
	buttons.handler(buf);
}

inline void ButInit()
{
	kip5::systick::SysTickTimer::handler = buttonsInit;
}

void init_before_rtos()
{
	init.init();
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);

	for(int i = 0; i < 1000000; i++) {__NOP();}

	melt 			= 	new kip5::v_mlt::Mlt_12864_io(bus, ep1, ep2, MDR_PORTB, 7);
	lcd 			= 	new kip5::v_mlt::Mlt_12864_base(melt);
	frameBuffer 	= 	new kip5::gl::FrameBuffer(lcd->width(), lcd->height(), lcd);
	charPainterMin 	=	new kip5::gl::CharPainter{frameBuffer, &kip5::resources::font8n_min};
	dma_adc_switch 	= 	new kip5::dma::Dma();
	asensors_switch = 	new kip5::adc::Adc();
	kip5::adc::Adc::initInternalADCPorts();

	melt->backlight(true);
	ButInit();
}

