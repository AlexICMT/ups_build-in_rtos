/* ups_build-in
 * /ups_build-in/inc/button_inc/button.hpp
 * button.hpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 12:17:26
 *      Author: AlexICMT
 */

#ifndef INC_BUTTON_INC_BUTTON_HPP_
#define INC_BUTTON_INC_BUTTON_HPP_

#include "MDR32F9Qx_config.h"

namespace kip5 {

namespace KEY {
enum KEYS : uint32_t
{
	RESET	= 1 << 0,
	DOWN	= 1 << 1,
	SELECT	= 1 << 2,
	UP		= 1 << 3,
	ENTER	= 1 << 4,
	ALL		= RESET | DOWN | SELECT | UP | ENTER
};
} // namespace KEY

namespace buttonsIO {

// Частота срабатывания таймера [Гц]
#define KEY_TIMER_FRQ	1000

// Антидребезг
#define KEY_FLT			10

// Интервал синхронизации [мс]
#define INTERVAL_MS		1000

// Интервал выхода из меню при бездействии кнопок [с]
#define NO_KEY_SEC		15

// Период первого автоповтора кнопок [мс]
#define KEY_REP_FIRST	400

// Период последующих автоповторений кнопок [мс]
#define KEY_REP			100

// Длительность звукового сигнала при нажатии кнопок [мс]
#define BEEP_MS			10

// Звуковой сигнал при нажатии на кнопки
#define BOOZER_ON()
#define BOOZER_OFF()

} // namespace buttonsIO

namespace button {

class Button
{
public:
	Button() = default;
	Button(const Button&) = delete;

	typedef uint32_t button_t;			///< Тип для представления кнопок в виде битов
private:
	static unsigned keys_cnt;			///< Счетчик повторений для кнопок с антидребезгом
	static unsigned ext_key_cnt;		///< Счетчик повторений для внешнего контакта с антидребезгом
	static unsigned keys_cnt_rep;		///< Счетчик повторений для автоповтора кнопок
	static unsigned no_key_cnt;			///< Счетчик отсутствия нажатий
	static unsigned beep_cnt;			///< Счетчик для звукового сигнала
	static unsigned time_cnt;			///< Счетчик времени

	volatile static button_t keys;		///< Кнопки
	volatile static button_t keys_flt;	///< Кнопки с антидребезгом
	volatile static button_t keys_rep;	///< Кнопки с автоповтором
	volatile static bool no_key;		///< Флаг устанавливающийся при отсутствии нажатий в течении 5 сек
	volatile static bool time_flag;		///< Флаг для синхронизации (устанавливается 1 раз в секунду)
	static bool use_beep;				///< Флаг использования звукового сигнала при нажатии кнопок
public:
	/// Обработчик кнопок (нужно вызывать периодически с частотой TIMER_FRQ)
	static void handler(button_t buf);

	/// Чтение кнопок
	static button_t getKeys()
	{
		return keys_flt;
	}

	/// Чтение кнопок
	operator button_t()
	{
		return keys_flt;
	}

	/// Проверка на срабатывание клавиши
	static bool getKey(uint32_t key)
	{
		if(keys_flt & key)
			return true;
		return false;
	}

	/// Чтение кнопок с автоповтором
	static button_t getKeysRep();

	/// Проверка на срабатывание клавиши с автоповтором
	static bool getKeyRep(button_t key);

	/// Проверка на бездействие клавиш в течении определенного времени
	static bool getNoKey()
	{
		return no_key;
	}

	/// Проверка на синхронизирующий флаг
	static bool getTimeFlag()
	{
		return time_flag;
	}

	/// Сброс синхронизирующего флага
	static void clearTimeFlag()
	{
		time_flag = false;
	}

	/// Ожидание нажатия клавиши
	static void waitKeyDown(button_t key)
	{
		while(!getKey(key))
		{
//			wdt_reset();
		}
	}

	/// Ожидание отпускания всех клавиш
	static void waitKeysUp()
	{
		while(keys_flt != 0)
		{
//			wdt_reset();
		}
	}

	/// Звуковой сигнал
	static void beep(uint32_t cnt)
	{
		BOOZER_ON();
		beep_cnt = cnt;		// cnt * 1000 / TMR2_FRQ [мс]
	}

	/// Управление использованием звукового сигнала при нажатии кнопок
	static void setUseBeep(bool f) {use_beep = f;}
};

} // namespace button
} // namespace kip5



#endif /* INC_BUTTON_INC_BUTTON_HPP_ */
