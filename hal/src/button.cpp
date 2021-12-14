/* ups_build-in
 * /ups_build-in/src/button_src/button.cpp
 * button.cpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 12:17:37
 *      Author: AlexICMT
 */

#include "button.hpp"
#include "MDR32F9Qx_rst_clk.h"
#include "macros.hpp"

namespace kip5 {
namespace button {

unsigned Button::keys_cnt = 0;
unsigned Button::ext_key_cnt = 0;
unsigned Button::keys_cnt_rep = 0;
unsigned Button::no_key_cnt = 0;
unsigned Button::beep_cnt = 0;
unsigned Button::time_cnt = 0;

volatile Button::button_t Button::keys = 0;
volatile Button::button_t Button::keys_flt = 0;
volatile Button::button_t Button::keys_rep = 0;
volatile bool Button::no_key = false;
volatile bool Button::time_flag = false;
bool Button::use_beep = false;

void Button::handler(button_t buf)
{
	// бузер
	if(beep_cnt)
	{
		beep_cnt--;
		if(!beep_cnt)
		{
			BOOZER_OFF();
		}
	}

	// антидребезг кнопок
	if(keys == buf)						// если новое состояние равно старому
	{
		if(++keys_cnt > KEY_FLT)		// если счетчик повторений больше KEY_FLT
		{
			keys_flt = buf;				// сохранить отфильтрованное значение
			keys_cnt = KEY_FLT;			// не допускаем переволнения счетчика
		}
	}
	else
	{
		keys = buf;						// запомнить новое состояние кнопок
		keys_cnt = 0;					// обнулить счетчик кнопок с антидребезгом
	}

	// автоповтор кнопок
	if(keys_flt == 0)					// если ни одна кнопка не нажата
	{
		keys_cnt_rep = 0;				// то обнулить счетчик автоповтора
		keys_rep = 0;					// ни одна кнопка не нажата
	}
	else
	{
		no_key_cnt = 0;					// счетчик бездействия кнопок
		no_key = false;					// флаг бездействия кнопок
		if(++keys_cnt_rep == 1)			// если кнопка нажата первый раз
		{
			keys_rep = keys_flt;		// то кнопка с автоповтором нажата первый раз
#ifdef USE_BEEP
			if(use_beep)
				BEEP(BEEP_MS);			// звуковой сигнал
#endif
		}
		else
		{
			// если кнопка нажата достаточно долго
			if(keys_cnt_rep == ((long)KEY_REP_FIRST * KEY_TIMER_FRQ / 1000))
			{
				keys_rep = keys_flt;	// кнопка нажата (автоповтор)
#ifdef USE_BEEP
				if(use_beep)
					BEEP(BEEP_MS);		// звуковой сигнал
#endif
				// последующие автоповторы не такие длительные как первый
				keys_cnt_rep = (long)(KEY_REP_FIRST - KEY_REP) * KEY_TIMER_FRQ / 1000;
			}
		}
	}

	// время и бездействие кнопок
	if(++time_cnt == ((long)KEY_TIMER_FRQ * INTERVAL_MS / (1000)))
	{
		time_cnt = 0;
		time_flag = true;				// флаг устанавливающийся через заданный промежуток времени

		if(keys_flt == 0)
		{
			if(no_key_cnt++ > (NO_KEY_SEC * 1000L / INTERVAL_MS))
			{
				no_key_cnt = 0;
				no_key = true;			// ни одна кнопка не нажата в течение заданного времени
			}
		}
	}
}

// Чтение кнопок с автоповтором
Button::button_t Button::getKeysRep()
{
	CLI();
	button_t keysRep = keys_rep;
	keys_rep = 0;
	SEI();
	return keysRep;
}

// Проверка на срабатывание кнопок с автоповтором
bool Button::getKeyRep(button_t key)
{
	CLI();
	button_t keysRep = keys_rep;
	if(keysRep & key)
	{
		keysRep &= ~key;
		keys_rep = keysRep;
		SEI();
		return true;
	}
	SEI();
	return false;
}

} // namespace button
} // namespace kip5


