/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/v_paint_device_base.hpp
 * v_paint_device_base.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:43:22
 *      Author: AlexICMT
 */

#ifndef GL_INC_V_PAINT_DEVICE_BASE_HPP_
#define GL_INC_V_PAINT_DEVICE_BASE_HPP_

#include <cstdint>

namespace kip5 {
namespace gl {

class PaintDevice
{
public:
protected:
	typedef int16_t Coord;
	typedef int16_t Line;
	typedef uint8_t Data;
	typedef uint_fast8_t FastData;
	typedef uint_fast16_t FastOp;

public:
	PaintDevice(int w, int h) : _width(w), _height(h) {}
	virtual ~PaintDevice() {}

	inline int width() const {return _width;}
	inline int height() const {return _height;}
	inline int lines() const {return _height / (8 * sizeof(Data));}

	virtual FastData read(int column, int line) = 0;
	virtual void write(int column, int line, FastData data) = 0;
	void modify(int column, int line, FastOp op);

	virtual void update() {}	///< Update of paint device

private:
	Coord _width;		///< Width of display region
	Coord _height;		///< Height of display region
};

} // namespace gl
} // namespace kip5



#endif /* GL_INC_V_PAINT_DEVICE_BASE_HPP_ */
