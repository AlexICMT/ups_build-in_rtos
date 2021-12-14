/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/src/v_paint_device_base.cpp
 * v_paint_device_base.cpp
 * 
 *  Created on: 14 ���. 2021 �.
 *  	  Time: 14:43:53
 *      Author: AlexICMT
 */

#include "v_paint_device_base.hpp"
#include <cassert>

namespace kip5 {
namespace gl {

/*
 * ��������������� ����� �����������
 * @param column ������� ����������� (���������� x)
 * @param line ������ ����������� (���������� y)
 * @param op ������� ����� �������� (0xooxx, o=or, x=xor)
 */

void PaintDevice::modify(int column, int line, FastOp op)
{
	assert(column >= 0 && column < width() && line >= 0 && line < lines());

	if(op == 0)
		return;
	if((op >> 8) == 0xFF)
	{
		write(column, line, ~op);
		return;
	}
	FastData data = read(column, line);
	data |= op >> 8;
	data ^= op;
	write(column, line, data);
}

} // namespace gl
} // namespace kip5

