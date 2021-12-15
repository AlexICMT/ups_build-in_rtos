/* ups_build-in
 * /ups_build-in/test_src_cpp/v_mlt_12864_io.cpp
 * v_mlt_12864_io.cpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 14:08:54
 *      Author: AlexICMT
 */

#include "v_mlt_12864_io.hpp"
#include "framework.hpp"


namespace kip5 {
namespace v_mlt {

//extern void readButtonPort();
__attribute__((optimize("O1"), noinline))
void Mlt_12864_io::Delay450ns()
{
	// Один цикл равен четырем тактам процессора.
	// Вызов и возврат 4 такта процессора.
	// 450нс / 12.5нс = 36тактов
	// 9 * 4 * 12.5нс = 450нс
	for(unsigned i = 8; i > 0; i--)
		__NOP();
}

__attribute__((always_inline))
inline void Mlt_12864_io::Delay20ns()
{
	// 2 * 12.5нс = 25нс
	__NOP();
	__NOP();
}

void Mlt_12864_io::init()
{
	auto configureAsDigital = [](MDR_PORT_TypeDef *port, int bit)
	{
		port->FUNC &= ~(0b11 << (bit * 2));		// Функция порта
		port->ANALOG |= (0b1 << bit);			// Цифровой
		port->PWR |= (0b11 << (bit * 2));		// Максимально быстрый фронт (порядка 10нс)
	};

	auto configureAsDigitalOutput = [configureAsDigital](MDR_PORT_TypeDef *port, int bit)
	{
		configureAsDigital(port, bit);			// Цифровой порт
		port->OE |= 1 << bit;					// Выход
	};

	// Инициализация портов ЖКИ
	configureAsDigitalOutput(MDR_PORTB, 7);

	setPortInputMode();		// По-умолчанию порт МК на вход
	clrE();					// По-умолчанию не активный режим
	enable1(false);			// По-умолчанию не выбран чип 1
	enable2(false);			// По-умолчанию не выбран чип 2
	readMode();				// По-умолчанию режим чтения
	commandMode();			// По-умолчанию режим команд
}

/**
 * Последовательность действий для чтения из ЖКИ
 * @return Прочитанные данные из ЖКИ
 */
Mlt_12864_io::Data Mlt_12864_io::readSequence()
{
	Delay450ns();
	setE();
	Delay450ns();
	Data data = readPort();
	clrE();
	return data;
}

/**
 * Последовательность действий для записи в ЖКИ
 * @param data Данные передаваемые в ЖКИ
 */
void Mlt_12864_io::writeSequence(Data data)
{
	writeMode();
	Delay450ns();
	setPortOutputMode();
	writePort(data);
	setE();
	Delay450ns();
	clrE();
	Delay20ns();
	setPortInputMode();
	readMode();
}

/**
 * Запись команды в индикатор MT12864
 * @param command
 */
bool Mlt_12864_io::writeCommand(Data command)
{
	writeSequence(command);
	return waitBusy();
}

/**
 * Чтение регистра статуса
 * @return
 */
Mlt_12864_io::Data Mlt_12864_io::readStatus()
{
	// Status register:
	//  7   6    5     4   3 2 1 0
	// Busy 0 ON/OFF Reset 0 0 0 0
	Data status = readSequence();
	return status;
}

/**
 * Запись данных в индикатор
 */
bool Mlt_12864_io::writeData(Data data)
{
	dataMode();
	writeSequence(data);
	commandMode();
	return waitBusy();
}

/**
 * Чтение данных из индикатора
 * @return
 */
Mlt_12864_io::Data Mlt_12864_io::readData()
{
	dataMode();
	Data data = readSequence();
	commandMode();
	waitBusy();
	return data;
}

/**
 * Захват шины и выбор кристалла
 * @param n Номер кристалла
 */
void Mlt_12864_io::takeBus(int n)
{
	lockBus();
	readButtonPort();
	enable1(n & 1);
	enable2(n & 2);
}

/**
 * Освобождение шины
 */
void Mlt_12864_io::releaseBus()
{
	enable1(false);
	enable2(false);
	unlockBus();
}

/**
 * Аппаратный сброс
 * @param f
 */
void Mlt_12864_io::reset(bool on)
{
	setReset(on);
}

/**
 * Управление подсветкой
 * @param on
 */
void Mlt_12864_io::backlight(bool on)
{
	setBackLight(on);
}


/**
 * Ожидание выполнения операции
 * @return true - ожидание завершилось удачно, false - выход по таймауту
 */
bool Mlt_12864_io::waitBusy()
{
	Delay450ns();
	setE();
	Delay450ns();
	unsigned tryCount = 1000;
	do
	{
		if(!readBusy())
		{
			clrE();
			return true;
		}
	} while(--tryCount);

	clrE();
	return false;
}


} // namespace v_mlt
} // namespace kip5


