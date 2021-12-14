/* ups_build-in
 * /ups_build-in/src/delay_cpp/delay.cpp
 * delay.cpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 14:03:12
 *      Author: AlexICMT
 */

#include "delay.hpp"

namespace kip5 {
namespace delay {

__attribute__((optimize("O1")))
void delay(unsigned t)
{
	while(t)
	{
		t--;
	//	__NOP();
	}
}

} // namespace delay
} // namespace kip5



