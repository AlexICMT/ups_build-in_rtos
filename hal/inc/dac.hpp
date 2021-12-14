/* ups_build-in
 * /ups_build-in/inc/dac_inc/dac.hpp
 * dac.hpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 12:00:02
 *      Author: AlexICMT
 */

#ifndef INC_DAC_INC_DAC_HPP_
#define INC_DAC_INC_DAC_HPP_

#include "MDR32F9Qx_config.h"

namespace kip5 {
namespace dac {

class Dac
{
public:
	Dac();
	Dac(const Dac&) = delete;

	void dac_init();
	void port_dac_init();
	void dac_launch();

	static void setDac(int data);
	static int getDac();

};

} // namespace dac
} // namespace kip5



#endif /* INC_DAC_INC_DAC_HPP_ */
