/* ups_build-in
 * /ups_build-in/test_inc_hpp/v_mlt_base.hpp
 * v_mlt_base.hpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 12:18:32
 *      Author: AlexICMT
 */

#ifndef TEST_INC_HPP_V_MLT_BASE_HPP_
#define TEST_INC_HPP_V_MLT_BASE_HPP_

#include <cstdint>

namespace kip5 {
namespace v_mlt {

/*
 * @brief Шина данных ЖКИ
 *
 * Операции с ШД ЖКИ:
 * 1. Запись команды
 * 2. Чтение статуса
 * 3. Запись данных
 * 4. Чтение данных
 * 5. Захват шины и выбор кристалла при необходимости
 * 6. Освобождение шины
 * 7. Аппаратный сброс
 * 8. Управление подсветкой
 */

class LcdIo
{
public:
	LcdIo() = default;								///< Конструктор
	LcdIo(const LcdIo&) = delete;					///< Конструктор копирования
	virtual ~LcdIo() = default;						///< Деструктор

	using Data = unsigned;							///< Тип для данных

	virtual bool writeCommand(Data command) = 0;	///< Запись команды в индикатр
	virtual Data readStatus() = 0;					///< Чтение регистра статуса

	virtual bool writeData(Data data) = 0;			///< Записсь данных в индикатор
	virtual Data readData() = 0;					///< Чтение данных из индикатора

	virtual void takeBus(int n) = 0;				///< Захват шины и выбор кристалла при необходимости
	virtual void releaseBus() = 0;					///< Освобождение шины

	virtual void reset(bool on) = 0;				///< Аппаратный сброс
	virtual void backlight(bool on) = 0;			///< Управление подсветкой
};

} // namespace v_mlt
} // namespace kip5



#endif /* TEST_INC_HPP_V_MLT_BASE_HPP_ */
