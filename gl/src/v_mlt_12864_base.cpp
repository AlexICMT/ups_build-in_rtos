/* ups_build-in
 * /ups_build-in/test_src_cpp/v_mlt_12864_base.cpp
 * v_mlt_12864_base.cpp
 * 
 *  Created on: 9 нояб. 2021 г.
 *  	  Time: 13:04:47
 *      Author: AlexICMT
 */

#include "v_mlt_12864_base.hpp"
#include "port.hpp"
#include <cassert>
#include "delay.hpp"

namespace kip5 {
namespace v_mlt {

Mlt_12864_base::FastData Mlt_12864_base::read(int column, int line)
{
	set_channel_x(column);
	set_pos_x(column);
	set_pos_y(line);

	readData();
	FastData data = readData();

	deselectChip();
	return data;
}

void Mlt_12864_base::write(int column, int line, FastData data)
{
	set_channel_x(column);
	set_pos_x(column);
	set_pos_y(line);

	writeData(data);

	deselectChip();
}

bool Mlt_12864_base::init()
{
	if(!reset())
		return false;

	if(!setStartLine(0))
		return false;

	if(!on(true))
		return false;

	return true;
}

// Сброс контроллеров ЖКИ
bool Mlt_12864_base::reset()
{
	resetPin(true);
	delay::delay_us(2000);
	resetPin(false);

	auto checkResetStatus = [this]() -> bool
	{
		unsigned tryCount = 2000;
		do {
			if((readStatus() & StatusResetMask) == 0)
			{
				deselectChip();
				return true;
			}
			delay::delay_us(1);
		} while(--tryCount);
		deselectChip();
		return false;
	};

	static_assert(sizeof(checkResetStatus) == 4, "");

	selectChip(Left);
	if(!checkResetStatus())
		return false;

	selectChip(Right);
	if(!checkResetStatus())
		return false;

	return true;
}

// Включить/выключить
bool Mlt_12864_base::on(bool on)
{
	selectChip(Left);
	if(!writeCommand(on ? DisplayOn : DisplayOff))
	{
		deselectChip();
		return false;
	}
	deselectChip();

	selectChip(Right);
	if(!writeCommand(on ? DisplayOn : DisplayOff))
	{
		deselectChip();
		return false;
	}
	deselectChip();

	return true;
}

// Установить стартовую линию (0..63)
bool Mlt_12864_base::setStartLine(C x)
{
	assert(x >= 0 && x < width());

	selectChip(Left);
	if(!writeCommand(DisplayStartLine | (x & 0x3F)))
	{
		deselectChip();
		return false;
	}
	deselectChip();

	selectChip(Right);
	if(!writeCommand(DisplayStartLine | (x & 0x3F)))
	{
		deselectChip();
		return false;
	}
	deselectChip();

	return true;
}

// Установить канал (правый/левый)
void Mlt_12864_base::set_channel_x(C x)
{
	assert(x >= 0 && x < width());

	if(x < width() / 2)
		selectChip(Left);
	else if(x < width())
		selectChip(Right);
}

// Установить текущую позицию x
void Mlt_12864_base::set_pos_x(C x)
{
	assert(x >= 0 && x < width());

	writeCommand(DisplaySetAddress | (x & 0x3F));
}

// Установить текущую позицию y
void Mlt_12864_base::set_pos_y(C y)
{
	assert(y >= 0 && y < height());
	assert(getActiveChipNum() == Left || getActiveChipNum() == Right);

	ChipState& activeChip = _chip[getActiveChipNum() - Left];
	if(activeChip.page == (y & 0x07))
		return;
	activeChip.page = y & 0x07;

	writeCommand(DisplaySetPage | (y & 0x07));
}

// Очистка экрана
void Mlt_12864_base::clear(D b)
{
	auto clearLine = [this](D b)
	{
		set_pos_x(0);
		for(C i = 0; i < width() / 2; i++)
			writeData(b);
	};

	for(C j = 0; j < lines(); j++)
	{
		selectChip(Left);
		set_pos_y(j);
		clearLine(b);
		deselectChip();

		selectChip(Right);
		set_pos_y(j);
		clearLine(b);
		deselectChip();
	}
}

// Копирование буфера на ЖКИ
void Mlt_12864_base::copy(const uint8_t* p)
{
	for(C j = 0; j < lines(); j++)
	{
		selectChip(Left);
		set_pos_y(j);
		set_pos_x(0);
		for(C i = 0; i < width() / 2; i++)
			writeData(*p++);
		deselectChip();

		selectChip(Right);
		set_pos_y(j);
		set_pos_x(0);
		for(C i = 0; i < width() / 2; i++)
			writeData(*p++);
		deselectChip();
	}
}

} //namespace v_mlt
} //namespace kip5
