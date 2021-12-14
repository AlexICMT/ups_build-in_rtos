/* ups_build-in
 * /ups_build-in/inc/ow_inc/ow.hpp
 * ow.hpp
 * 
 *  Created on: 29 нояб. 2021 г.
 *  	  Time: 11:11:14
 *      Author: AlexICMT
 */

#ifndef INC_OW_INC_OW_HPP_
#define INC_OW_INC_OW_HPP_

#include "i2c.hpp"
#include "helper.hpp"

namespace kip5 {
namespace ow {

constexpr uint32_t ONEWIRE_CRC8_TABLE = 0;

constexpr uint8_t DS2482_COMMAND_RESET	= 0xF0;	// Device reset

constexpr uint8_t DS2482_COMMAND_SRP = 0xE1; 	// Set read pointer
constexpr uint8_t DS2482_POINTER_STATUS = 0xF0;
constexpr uint32_t DS2482_STATUS_BUSY = (1<<0);
constexpr uint32_t DS2482_STATUS_PPD = (1<<1);
constexpr uint32_t DS2482_STATUS_SD	= (1<<2);
constexpr uint32_t DS2482_STATUS_LL	= (1<<3);
constexpr uint32_t DS2482_STATUS_RST = (1<<4);
constexpr uint32_t DS2482_STATUS_SBR =(1<<5);
constexpr uint32_t DS2482_STATUS_TSB = (1<<6);
constexpr uint32_t DS2482_STATUS_DIR = (1<<7);
constexpr uint32_t DS2482_POINTER_DATA = 0xE1;
constexpr uint32_t DS2482_POINTER_CONFIG=0xC3;
constexpr uint32_t DS2482_CONFIG_APU=(1<<0);
constexpr uint32_t DS2482_CONFIG_SPU=(1<<2);
constexpr uint32_t DS2482_CONFIG_1WS=(1<<3);


constexpr uint32_t DS2482_COMMAND_WRITECONFIG = 0xD2;
constexpr uint32_t DS2482_COMMAND_RESETWIRE = 0xB4;
constexpr uint32_t DS2482_COMMAND_WRITEBYTE = 0xA5;
constexpr uint32_t DS2482_COMMAND_READBYTE = 0x96;
constexpr uint32_t DS2482_COMMAND_SINGLEBIT = 0x87;
constexpr uint32_t  DS2482_COMMAND_TRIPLET = 0x78;

constexpr uint32_t WIRE_COMMAND_SKIP = 0xCC;
constexpr uint32_t WIRE_COMMAND_SELECT = 0x55;
constexpr uint32_t WIRE_COMMAND_SEARCH = 0xF0;

constexpr uint32_t DS2482_ERROR_TIMEOUT = (1<<0);
constexpr uint32_t DS2482_ERROR_SHORT = (1<<1);
constexpr uint32_t DS2482_ERROR_CONFIG = (1<<2);

#define ERR_T Byte[0]
#define ERR_NAN Byte[1]

//------------------------DS2438------------------------
constexpr uint32_t DS2438_TEMPERATURE_CONVERSION = 0x44;
constexpr uint32_t DS2438_VOLTAGE_CONVERSION = 0xB4;
constexpr uint32_t DS2438_WRITE_SCRATCHPAD = 0x4E;
constexpr uint32_t DS2438_COPY_SCRATCHPAD = 0x48;
constexpr uint32_t DS2438_READ_SCRATCHPAD = 0xBE;
constexpr uint32_t DS2438_RECALL_MEMORY = 0xB8;
constexpr uint32_t DS2438_PAGE_0 = 0x00;
constexpr uint32_t DS2438_PAGE_1 = 0x01;
constexpr uint32_t DS2438_PAGE_2 = 0x02;
constexpr uint32_t DS2438_PAGE_3 = 0x03;
constexpr uint32_t DS2438_PAGE_4 = 0x04;
constexpr uint32_t DS2438_PAGE_5 = 0x05;
constexpr uint32_t DS2438_PAGE_6 = 0x06;
constexpr uint32_t DS2438_PAGE_7 = 0x07;

constexpr uint32_t DS2438_CHA = 0;
constexpr uint32_t DS2438_CHB = 1;

constexpr uint32_t DS2438_MODE_CHA = 0x01;
constexpr uint32_t DS2438_MODE_CHB = 0x02;
constexpr uint32_t DS2438_MODE_TEMPERATURE = 0x4;

constexpr uint32_t DS2438_TEMPERATURE_DELAY = 10;
constexpr uint32_t DS2438_VOLTAGE_CONVERSION_DELAY = 8;
//------------------------DS2438------------------------

class DS2482
{

	const uint32_t Direction_Transmitter = 0x0;
	const uint32_t Direction_Receiver = 0x1;

	struct Bits8 : helper::Bits8 {};
	using RegStatus = helper::ByteUnion<Bits8, 0xFF>;
	using RegConfig = helper::ByteUnion<Bits8, 0xFF>;

public:

	static const uint32_t NUMBER_DS1820 = 2;
	static const uint32_t NUMBER_DS2438 = 1;
	static const uint32_t NUMBER = 3;

	union DS
	{
		uint32_t ArrayForSave[NUMBER*4];
		uint32_t Sensor[NUMBER][4];
		uint8_t Byte[NUMBER * 4 * 4];
	};

	union DS1820
	{
		uint32_t ArrayForSave[NUMBER_DS1820*4];
		uint32_t Sensor[NUMBER_DS1820][4];
		uint8_t Byte[NUMBER_DS1820 * 4 * 4];
	};

	union DS2438
	{
		uint32_t ArrayForSave[NUMBER_DS2438*4];
		uint32_t Sensor[NUMBER_DS2438][4];
		uint8_t Byte[NUMBER_DS2438 * 4 * 4];
	};

	union ERROR_DS1820
	{
		uint32_t Word;
		uint8_t Byte[4];
	};

	float Temperature[NUMBER_DS1820];
	ERROR_DS1820 Error_Channel[NUMBER_DS1820] = {0};
	static constexpr uint8_t ERROR_CHANELL_NAN = 5;
	static constexpr uint8_t ERROR_NAN = 5;
	DS1820 Sensor_T_AB;
	DS2438 DS2438;
	static constexpr uint32_t OFFSET = (sizeof(uint32_t)*4);
	static constexpr uint8_t FAMILY_CODE = 0x28;
	static constexpr uint8_t FAMILY_CODE_DS2438 = 0x26;

	DS2482(i2c::I2c& i2c, uint8_t addr);

	DS2482(const DS2482&) = delete;

//заимствование----------------------------------------------------------------
	uint8_t getAddress();
	uint8_t getError();
	void deviceReset();
	void setReadPointer(uint8_t readPointer);
	uint8_t readStatus();
	uint8_t readData();
	uint8_t waitOnBusy();
	uint8_t readConfig();

	void writeConfig(uint8_t config);
	void setStrongPullup();
	void clearStrongPullup();
	uint8_t wireReset();
	void wireWriteByte(uint8_t data, uint8_t power = 0);
	uint8_t wireReadByte();
	void wireWriteBit(uint8_t data, uint8_t power = 0);
	uint8_t wireReadBit();
	void wireSkip();
	void wireSelect(const uint8_t *addr);//(const uint8_t rom[8]);
	void wireSelectArr(const uint8_t rom[8]);
	void wireResetSearch();
	uint8_t wireSearch(uint8_t *address);

// emulation of original OneWire library
	void reset_search();
	uint8_t search(uint8_t *newAddr);
	static uint8_t crc8(uint8_t *addr, uint8_t len);//static uint8_t crc8(const uint8_t *addr, uint8_t len);
	uint8_t reset(void);
	void select(const uint8_t *addr);//(const uint8_t rom[8]);
	void skip(void);
	void write(uint8_t v, uint8_t power = 0);
	uint8_t read(void);
	uint8_t read_bit(void);
	void write_bit(uint8_t v);
//-----------------------------------------------------------------------------
	void begin();
	void requestFrom();
	void end();
	void writeByte(uint8_t);
	uint8_t readByte();
	uint8_t readByte(int ack);

//------------------------DS2438------------------------
/*
 * 64-bit lasered ROM
 * temperature sensor
 * battery voltage A/D
 * battery current A/D
 * current accumulators
 * elapsed time meter
 * 40-byte nonvolatile user-memory
 */

	void mode(uint8_t mode);
	void update();

	uint16_t getTemperatureAdc();
	double getTemperature();

	uint16_t getVoltageAdc(uint8_t ch);
	double getVoltage(uint8_t ch);

	unsigned long getTimestamp();
	uint8_t startConversion(uint8_t ch, uint8_t mode);
	uint8_t selectChannel(uint8_t ch);

	 void writePageZero(uint8_t *data);
	 uint8_t readPageZero(uint8_t *data);

	uint8_t status = 0;
	uint8_t data[9];
//------------------------DS2438------------------------
private:
    i2c::I2c& i2c;
    //uint8_t i2c_address;
    uint8_t reg_subaddr;
//заимствование-------------
	//void begin();
	//void requestFrom();
	//void end();
	//void writeByte(uint8_t);
	//uint8_t readByte();
	//uint8_t calcCRC(uint8_t *buf);
	//uint8_t readByteasd();
	uint8_t mAddress;
	uint8_t mError;

	uint8_t searchAddress[8];
	uint8_t searchLastDiscrepancy;
	uint8_t searchLastDeviceFlag;

	uint8_t _mode = 0;
	uint16_t _temperature = 0;
	uint16_t _voltageA = 0;
	uint16_t _voltageB = 0;
	uint16_t _error = 0;



	double _dtemperature = 0.0;
	double _dvoltageA = 0.0;
	double _dvoltageB = 0.0;


//--------------------------
};
/*
class DS2438 : public DS2482
{
public:
	DS2438(i2c::I2c& i2c, uint8_t addr);
	DS2438(const DS2438&) = delete;


};
*/
} // namespace ow
} // namespace kip5

#endif /* INC_OW_INC_OW_HPP_ */
