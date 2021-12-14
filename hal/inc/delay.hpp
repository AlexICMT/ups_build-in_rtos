/* ups_build-in
 * /ups_build-in/inc/delay_inc/delay.hpp
 * delay.hpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 13:58:21
 *      Author: AlexICMT
 */

#ifndef INC_DELAY_INC_DELAY_HPP_
#define INC_DELAY_INC_DELAY_HPP_

#include "MDR32Fx.h"

namespace kip5 {
namespace delay {

void delay(unsigned t);

inline void delay_us(unsigned us) {delay((unsigned)((us * 80) / 3));}
inline void delay_ms(unsigned ms) {delay((unsigned)((ms * 80000) / 3));}
inline void delay_s(unsigned s)   {delay((unsigned)((s * 80000000) / 3));}

} // namespace delay
} // namespace kip5

#endif /* INC_DELAY_INC_DELAY_HPP_ */
