/* ups_build-in
 * /ups_build-in/inc/dma_inc/dma.hpp
 * dma.hpp
 * 
 *  Created on: 19 нояб. 2021 г.
 *  	  Time: 15:29:29
 *      Author: AlexICMT
 */

#ifndef INC_DMA_INC_DMA_HPP_
#define INC_DMA_INC_DMA_HPP_

//#include "MDR32F9Qx_config.h"
#include <cstdint>

namespace kip5 {

namespace handlers {
extern "C" void DMA_IRQHandler();
} // namespace handlers

namespace dma {
class Dma
{
public:
	Dma();
	Dma(const Dma&) = delete;

	void Dma_config();
	void Dma_launch_adc();
	uint16_t get_adc();
	//uint32_t Dma_read_adc();
private:
	friend void handlers::DMA_IRQHandler();
};

} // namespace dma
} // namespace kip5



#endif /* INC_DMA_INC_DMA_HPP_ */
