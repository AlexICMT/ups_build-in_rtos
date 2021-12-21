/* ups_build_in_rtos
 * /ups_build_in_rtos/hal/inc/framework.hpp
 * framework.hpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 10:56:06
 *      Author: AlexICMT
 */

#ifndef HAL_INC_FRAMEWORK_HPP_
#define HAL_INC_FRAMEWORK_HPP_

#include "view_ctrl_bus.hpp"
#include "button.hpp"
#include "v_mlt_12864_io.hpp"
#include "char_painter.hpp"
#include "initialization.hpp"
#include "frame_buffer.hpp"
#include "v_mlt_12864_base.hpp"
#include "fonts.hpp"
#include "delay.hpp"
#include "adc.hpp"
#include "dma.hpp"
#include "timer.hpp"
//#include "help.hpp"

#include "systick.hpp"

extern kip5::view_ctrl_bus::OutExtPort ep1;
extern kip5::view_ctrl_bus::OutExtPort ep2;
extern kip5::init::Initialization init;
extern kip5::gl::FrameBuffer* frameBuffer;
extern kip5::v_mlt::Mlt_12864_io* melt;
extern kip5::v_mlt::Mlt_12864_base* lcd;
extern kip5::gl::CharPainter* charPainterMin;
extern kip5::adc::Adc* asensors_switch;
extern kip5::dma::Dma* dma_adc_switch;

extern kip5::view_ctrl_bus::View_Ctrl_Bus bus;
extern kip5::button::Button buttons;

extern void init_before_rtos();

extern volatile uint32_t keyboardPort;
extern void readButtonPort();
extern void buttonsInit();

#endif /* HAL_INC_FRAMEWORK_HPP_ */
