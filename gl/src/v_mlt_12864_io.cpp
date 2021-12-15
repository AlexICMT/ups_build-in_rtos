/* ups_build-in
 * /ups_build-in/test_src_cpp/v_mlt_12864_io.cpp
 * v_mlt_12864_io.cpp
 * 
 *  Created on: 9 ����. 2021 �.
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
	// ���� ���� ����� ������� ������ ����������.
	// ����� � ������� 4 ����� ����������.
	// 450�� / 12.5�� = 36������
	// 9 * 4 * 12.5�� = 450��
	for(unsigned i = 8; i > 0; i--)
		__NOP();
}

__attribute__((always_inline))
inline void Mlt_12864_io::Delay20ns()
{
	// 2 * 12.5�� = 25��
	__NOP();
	__NOP();
}

void Mlt_12864_io::init()
{
	auto configureAsDigital = [](MDR_PORT_TypeDef *port, int bit)
	{
		port->FUNC &= ~(0b11 << (bit * 2));		// ������� �����
		port->ANALOG |= (0b1 << bit);			// ��������
		port->PWR |= (0b11 << (bit * 2));		// ����������� ������� ����� (������� 10��)
	};

	auto configureAsDigitalOutput = [configureAsDigital](MDR_PORT_TypeDef *port, int bit)
	{
		configureAsDigital(port, bit);			// �������� ����
		port->OE |= 1 << bit;					// �����
	};

	// ������������� ������ ���
	configureAsDigitalOutput(MDR_PORTB, 7);

	setPortInputMode();		// ��-��������� ���� �� �� ����
	clrE();					// ��-��������� �� �������� �����
	enable1(false);			// ��-��������� �� ������ ��� 1
	enable2(false);			// ��-��������� �� ������ ��� 2
	readMode();				// ��-��������� ����� ������
	commandMode();			// ��-��������� ����� ������
}

/**
 * ������������������ �������� ��� ������ �� ���
 * @return ����������� ������ �� ���
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
 * ������������������ �������� ��� ������ � ���
 * @param data ������ ������������ � ���
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
 * ������ ������� � ��������� MT12864
 * @param command
 */
bool Mlt_12864_io::writeCommand(Data command)
{
	writeSequence(command);
	return waitBusy();
}

/**
 * ������ �������� �������
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
 * ������ ������ � ���������
 */
bool Mlt_12864_io::writeData(Data data)
{
	dataMode();
	writeSequence(data);
	commandMode();
	return waitBusy();
}

/**
 * ������ ������ �� ����������
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
 * ������ ���� � ����� ���������
 * @param n ����� ���������
 */
void Mlt_12864_io::takeBus(int n)
{
	lockBus();
	readButtonPort();
	enable1(n & 1);
	enable2(n & 2);
}

/**
 * ������������ ����
 */
void Mlt_12864_io::releaseBus()
{
	enable1(false);
	enable2(false);
	unlockBus();
}

/**
 * ���������� �����
 * @param f
 */
void Mlt_12864_io::reset(bool on)
{
	setReset(on);
}

/**
 * ���������� ����������
 * @param on
 */
void Mlt_12864_io::backlight(bool on)
{
	setBackLight(on);
}


/**
 * �������� ���������� ��������
 * @return true - �������� ����������� ������, false - ����� �� ��������
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


