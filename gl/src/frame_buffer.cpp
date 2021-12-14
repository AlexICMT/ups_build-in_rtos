/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/frame_buffer.cpp
 * frame_buffer.cpp
 *
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:38:45
 *      Author: AlexICMT
 *
 * History:
 *
 * @file frame_buffer.cpp
 * @date 27.08.2015
 * @author vlunyov
 * @brief Frame buffer
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */
#include "frame_buffer.hpp"
#include "v_mlt_12864_base.hpp"	// TODO
#include <cstring>
#include <cassert>

namespace kip5 {
namespace gl {

FrameBuffer::FrameBuffer(int w, int h, PaintDevice* paintDevice) : PaintDevice(w, h), _paintDevice(paintDevice)
{
	_buf = new Data[size()];
	clear();
}

FrameBuffer::~FrameBuffer()
{
	delete[] _buf;
}

FrameBuffer::FastData FrameBuffer::read(int column, int line)
{
	assert(column >= 0 && column < width() && line >= 0 && line < lines());

	return _buf[line * width() + column];
}

void FrameBuffer::write(int column, int line, FastData data)
{
	assert(column >= 0 && column < width() && line >= 0 && line < lines());

	_buf[line * width() + column] = data;
}

//void FrameBuffer::modify(int column, int line, FastOp op)
//{
//	assert(column >= 0 && column < width() && line >= 0 && line < lines());
//
//	const int index = line * width() + column;
//	FastData data = _buf[index];
//	data |= op >> 8;
//	data ^= op;
//	_buf[index] = data;
//}

void FrameBuffer::update()
{
	// TODO !!!!!!!!!!!!!!!
	v_mlt::Mlt_12864_base* lcd = static_cast<v_mlt::Mlt_12864_base*>(_paintDevice);
	if(lcd->initialized())//9.01.18 asd
	{//asd - если нет экрана, то и не выводить
		assert(lcd);
		lcd->on(true);
		lcd->setStartLine(0);
		lcd->copy(getBuffer());
	}
}

void FrameBuffer::clear()
{
	std::memset(_buf, 0, size());
}

void FrameBuffer::copy(const Data* p)
{
	std::memcpy(_buf, p, size());
}

} // namespace gl
} // namespace kws
