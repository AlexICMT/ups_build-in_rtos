/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/frame_buffer.hpp
 * frame_buffer.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:38:45
 *      Author: AlexICMT
 *
 * History:
 *
 * @file frame_buffer.hpp
 * @date 27.08.2015
 * @author vlunyov
 * @brief Frame buffer
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (Ср, 11 янв 2017) $
 *
 */

#ifndef GL_INC_FRAME_BUFFER_HPP_
#define GL_INC_FRAME_BUFFER_HPP_

#include "v_paint_device_base.hpp"
#include <cstdint>
#include <cstddef>

namespace kip5 {
namespace gl {

/**
 * @brief \~russian Буффер изображения в ОЗУ \~english Image buffer in the RAM
 */
class FrameBuffer : public PaintDevice
{
public:
	FrameBuffer(int w, int h, PaintDevice* paintDevice);
	virtual ~FrameBuffer();

	FastData read(int column, int line) override;
	void write(int column, int line, FastData data) override;
//	void modify(int column, int line, FastOp op) override;

	void update() override;

	void clear();
	void copy(const Data* p);

	inline Data* getBuffer() {return _buf;}

protected:
	inline int size() const {return width() * lines() * sizeof(Data);}

private:
	Data* _buf;					///< Указатель на буфер изображения
	PaintDevice* _paintDevice;	///< Указатель на реальное устройство отрисовки
};

} // namespace gl
} // namespace kip5



#endif /* GL_INC_FRAME_BUFFER_HPP_ */
