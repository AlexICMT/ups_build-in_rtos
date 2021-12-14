/* ups_build-in
 * /ups_build-in/test_inc_hpp/v_mlt_base.hpp
 * v_mlt_base.hpp
 * 
 *  Created on: 9 ����. 2021 �.
 *  	  Time: 12:18:32
 *      Author: AlexICMT
 */

#ifndef TEST_INC_HPP_V_MLT_BASE_HPP_
#define TEST_INC_HPP_V_MLT_BASE_HPP_

#include <cstdint>

namespace kip5 {
namespace v_mlt {

/*
 * @brief ���� ������ ���
 *
 * �������� � �� ���:
 * 1. ������ �������
 * 2. ������ �������
 * 3. ������ ������
 * 4. ������ ������
 * 5. ������ ���� � ����� ��������� ��� �������������
 * 6. ������������ ����
 * 7. ���������� �����
 * 8. ���������� ����������
 */

class LcdIo
{
public:
	LcdIo() = default;								///< �����������
	LcdIo(const LcdIo&) = delete;					///< ����������� �����������
	virtual ~LcdIo() = default;						///< ����������

	using Data = unsigned;							///< ��� ��� ������

	virtual bool writeCommand(Data command) = 0;	///< ������ ������� � ��������
	virtual Data readStatus() = 0;					///< ������ �������� �������

	virtual bool writeData(Data data) = 0;			///< ������� ������ � ���������
	virtual Data readData() = 0;					///< ������ ������ �� ����������

	virtual void takeBus(int n) = 0;				///< ������ ���� � ����� ��������� ��� �������������
	virtual void releaseBus() = 0;					///< ������������ ����

	virtual void reset(bool on) = 0;				///< ���������� �����
	virtual void backlight(bool on) = 0;			///< ���������� ����������
};

} // namespace v_mlt
} // namespace kip5



#endif /* TEST_INC_HPP_V_MLT_BASE_HPP_ */
