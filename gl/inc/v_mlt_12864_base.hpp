/* ups_build-in
 * /ups_build-in/test_inc_hpp/v_mlt_12864_base.hpp
 * v_mlt_12864_base.hpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 13:04:36
 *      Author: AlexICMT
 */

#ifndef TEST_INC_HPP_V_MLT_12864_BASE_HPP_
#define TEST_INC_HPP_V_MLT_12864_BASE_HPP_

#include "v_mlt_base.hpp"
#include "v_paint_device_base.hpp"

namespace kip5 {
namespace v_mlt {

class Mlt_12864_base : public gl::PaintDevice
{
	LcdIo* _io;

public:
	Mlt_12864_base(LcdIo* io) : PaintDevice(128, 64), _io(io) {_initialized = init();}
	Mlt_12864_base(const Mlt_12864_base&) = delete;

	FastData read(int column, int line) override;
	void write(int column, int line, FastData data) override;

public:
	typedef int C;			///< Тип для координат
	typedef unsigned D;		///< Тип для данных

	bool init();							///< Инициализация
	bool initialized() const {return _initialized;}

	bool reset();							///< Сброс контроллеров ЖКИ
	bool on(bool on);						///< Включить/выключить
	bool setStartLine(C x);					///< Установить стартовую линию (0..63)

	void clear(D b = 0);					///< Очистка экрана
	void copy(const uint8_t* p);			///< Копирование буфера на ЖКИ

protected:
	/// Контроллеры ЖКИ
	enum ChipNum : uint8_t
	{
		None	= 0,			///< Чип не выбран
		Left	= 1,			///< Левая половина ЖКИ
		Right	= 2,			///< Правая половина ЖКИ
		All		= Left | Right	///< Оба чипа
	};

	void set_channel_x(C x);				///< Установить канал (правый/левый)
	void set_pos_x(C x);					///< Установить текущую позицию x
	void set_pos_y(C y);					///< Установить текущую позицию y

	inline void selectChip(ChipNum num);	///< Выбор активного кристалла
	inline void deselectChip();				///< Окончание транзакций с ЖКИ

	inline bool writeCommand(D command);	///< Запись команды в индикатр
	inline D readStatus();					///< Чтение регистра статуса
	inline void writeData(D data);			///< Записсь данных в индикатор
	inline D readData();					///< Чтение данных из индикатора

	inline void resetPin(bool f);			///< Сброс контроллера ЖКИ

protected:
	/// Команды контроллера ЖКИ
	enum Commands : C
	{
		DisplayOn			= 0x3F,
		DisplayOff			= 0x3E,
		DisplayStartLine	= 0xC0,
		DisplaySetPage		= 0xB8,
		DisplaySetAddress	= 0x40
	};

	///  7   6    5     4   3 2 1 0
	/// Busy 0 ON/OFF Reset 0 0 0 0
	enum StatusRegister : C
	{
		StatusBusyBit		= 7,
		StatusOnOffBit		= 5,
		StatusResetBit		= 4,
		StatusBusyMask		= 1 << StatusBusyBit,
		StatusOnOffMask		= 1 << StatusOnOffBit,
		StatusResetMask		= 1 << StatusResetBit
	};

private:
	struct ChipState
	{
		static constexpr uint8_t UNDEFINED = 255;
		uint8_t page = UNDEFINED;			// 0..7
	//	uint8_t address = UNDEFINED;		// 0..63
	//	uint8_t startLine = UNDEFINED;		// 0..63
	//	bool switchedOn = false;			// 0..1
	};

	static constexpr uint8_t _chipCount = 2;
	ChipState _chip[_chipCount];
//	ChipState& _activeChip;

	inline uint8_t getActiveChipNum() const	///< Текущий активный кристалл
	{
		return _activeChipNum;
	}

private:
	bool _initialized = false;
	uint8_t _activeChipNum = None;
}; // class Mlt_12864_base

void Mlt_12864_base::selectChip(ChipNum num)	{_activeChipNum = num; _io->takeBus(num);}
void Mlt_12864_base::deselectChip()				{_io->releaseBus();}

bool Mlt_12864_base::writeCommand(D command)	{return _io->writeCommand(command);}
Mlt_12864_base::D Mlt_12864_base::readStatus()	{return _io->readStatus();}

void Mlt_12864_base::writeData(D data)			{_io->writeData(data);}
Mlt_12864_base::D Mlt_12864_base::readData()	{return _io->readData();}

void Mlt_12864_base::resetPin(bool on)			{_io->reset(on);}

} //namespace v_mlt
} //namespace kip5

#endif /* TEST_INC_HPP_V_MLT_12864_BASE_HPP_ */
