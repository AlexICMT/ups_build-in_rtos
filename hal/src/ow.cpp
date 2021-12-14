/* ups_build-in
 * /ups_build-in/src/ow_src/ow.cpp
 * ow.cpp
 * 
 *  Created on: 29 нояб. 2021 г.
 *  	  Time: 11:11:27
 *      Author: AlexICMT
 */

#include "ow.hpp"
#include "delay.hpp"


namespace kip5 {
namespace ow {

DS2482::DS2482(i2c::I2c& i2c, uint8_t addr) : i2c(i2c), mAddress(addr)
{
	mError = 0;
}

uint8_t DS2482::getAddress()
{
	return mAddress;
}

uint8_t DS2482::getError()
{
	return mError;
}

void DS2482::begin()
{
	i2c.start(mAddress, Direction_Transmitter);
}

void DS2482::requestFrom()
{
	i2c.start(mAddress, Direction_Receiver);
}

void DS2482::end()
{
	i2c.stop();
}

void DS2482::writeByte(uint8_t data)
{
	i2c.rw(Direction_Transmitter, &data, 1);
}

uint8_t DS2482::readByte()
{
	uint8_t data;
	i2c.start(mAddress, Direction_Receiver);
	i2c.rw(Direction_Receiver, &data, 1);
	return data;
}

uint8_t DS2482::readByte(int ack)
{
	uint8_t data;
	i2c.start(mAddress, Direction_Receiver);
	i2c.rw(Direction_Receiver, &data, 1, ack);
	return data;
}

// Performs a global reset of device state machine logic. Terminates any ongoing 1-Wire communication.
void DS2482::deviceReset()
{
	begin();
	writeByte(DS2482_COMMAND_RESET);
	end();
}

// Sets the read pointer to the specified register. Overwrites the read pointer position of any 1-Wire communication command in progress.
void DS2482::setReadPointer(uint8_t readPointer)
{
	begin();
	writeByte(DS2482_COMMAND_SRP);
	writeByte(readPointer);
	end();
}

// Read the status register
uint8_t DS2482::readStatus()
{
	setReadPointer(DS2482_POINTER_STATUS);
	return readByte();
}

// Read the data register
uint8_t DS2482::readData()
{
	setReadPointer(DS2482_POINTER_DATA);
	return readByte();
}

// Read the config register
uint8_t DS2482::readConfig()
{
	setReadPointer(DS2482_POINTER_CONFIG);
    return readByte();
}

void DS2482::setStrongPullup()
{
	writeConfig(readConfig() | DS2482_CONFIG_SPU);
}

void DS2482::clearStrongPullup()
{
	writeConfig(readConfig() & !DS2482_CONFIG_SPU);
}

// Churn until the busy bit in the status register is clear
uint8_t DS2482::waitOnBusy()
{
	uint8_t status;

    for(int i=20; i>0; i--)
    {
    	status = readStatus();
    	if (!(status & DS2482_STATUS_BUSY))
    		break;
    	//vTaskDelay(1);//1мс
    	delay::delay_ms(1);
    }
	// if we have reached this point and we are still busy, there is an error
	if (status & DS2482_STATUS_BUSY)
		mError = DS2482_ERROR_TIMEOUT;

	// Return the status so we don't need to explicitly do it again
	return status;
}

// Write to the config register
void DS2482::writeConfig(uint8_t config)
{
	//volatile uint8_t config;
	//config = asd;
	waitOnBusy();
	begin();
	writeByte(DS2482_COMMAND_WRITECONFIG);
	if (config == 0xF0)
	config = 0xF0;// - ПРИ ОТЛАДКЕ ПОЧЕМУ ТО НЕ ПЕРЕДАЕТСЯ ПАРАМЕТР, ПРИХОДИТСЯ ТАК. РАСКОМЕНТИРОВАТЬ ПРИ ОТЛАДКЕ
	// Write the 4 bits and the complement 4 bits
	writeByte(config | ((~config)<<4));
	end();
	// This should return the config bits without the complement
	uint8_t temp = readByte();//readByteasd();

	//config &= 0x0F;
	if (temp != config)//if (readByte() != config)
		mError = DS2482_ERROR_CONFIG;
}

// Generates a 1-Wire reset/presence-detect cycle (Figure 4) at the 1-Wire line. The state
// of the 1-Wire line is sampled at tSI and tMSP and the result is reported to the host
// processor through the Status Register, bits PPD and SD.
uint8_t DS2482::wireReset()
{

	waitOnBusy();
	// Datasheet warns that reset with SPU set can exceed max ratings
	clearStrongPullup();// 10.08.17

	waitOnBusy();

	begin();
	writeByte(DS2482_COMMAND_RESETWIRE);
	end();


	uint8_t status = waitOnBusy();

	if (status & DS2482_STATUS_SD)
	{
		mError = DS2482_ERROR_SHORT;
	}

	return status & DS2482_STATUS_PPD ? true : false;
}

// Writes a single data byte to the 1-Wire line.
void DS2482::wireWriteByte(uint8_t data, uint8_t power)
{
	waitOnBusy();
	if (power)
		setStrongPullup();
	begin();
	writeByte(DS2482_COMMAND_WRITEBYTE);
	writeByte(data);
	end();
}

// Generates eight read-data time slots on the 1-Wire line and stores result in the Read Data Register.
uint8_t DS2482::wireReadByte()
{
	waitOnBusy();
	begin();
	writeByte(DS2482_COMMAND_READBYTE);
	end();
	waitOnBusy();
	return readData();
}

// Generates a single 1-Wire time slot with a bit value “V” as specified by the bit byte at the 1-Wire line
// (see Table 2). A V value of 0b generates a write-zero time slot (Figure 5); a V value of 1b generates a
// write-one time slot, which also functions as a read-data time slot (Figure 6). In either case, the logic
// level at the 1-Wire line is tested at tMSR and SBR is updated.
void DS2482::wireWriteBit(uint8_t data, uint8_t power)
{
	waitOnBusy();
	if (power)
		setStrongPullup();
	begin();
	writeByte(DS2482_COMMAND_SINGLEBIT);
	writeByte(data ? 0x80 : 0x00);
	end();
}

// As wireWriteBit
uint8_t DS2482::wireReadBit()
{
	wireWriteBit(1);
	uint8_t status = waitOnBusy();
	return status & DS2482_STATUS_SBR ? 1 : 0;
}

// 1-Wire skip
void DS2482::wireSkip()
{
	wireWriteByte(WIRE_COMMAND_SKIP);
}

void DS2482::wireSelect(const uint8_t *addr)
{
	wireWriteByte(WIRE_COMMAND_SELECT);
	for (int i=0;i<8;i++)
		wireWriteByte(*(addr+i));
}

void DS2482::wireSelectArr(const uint8_t rom[8])
{
	wireWriteByte(WIRE_COMMAND_SELECT);
	for (int i=0;i<8;i++)
		wireWriteByte(rom[i]);
}

//  1-Wire reset search algorithm
void DS2482::wireResetSearch()
{
	searchLastDiscrepancy = 0;
	searchLastDeviceFlag = 0;

	for (int i = 0; i < 8; i++)
	{
		searchAddress[i] = 0;
	}
}

// Perform a search of the 1-Wire bus
uint8_t DS2482::wireSearch(uint8_t *address)
{
	uint8_t direction;
	uint8_t last_zero=0;

	if (searchLastDeviceFlag)
		return 0;

	if (!wireReset())
		return 0;

	waitOnBusy();

	wireWriteByte(WIRE_COMMAND_SEARCH);

	for(uint8_t i=0;i<64;i++)
	{
		int searchByte = i / 8;
		int searchBit = 1 << i % 8;

		if (i < searchLastDiscrepancy)
			direction = searchAddress[searchByte] & searchBit;
		else
			direction = i == searchLastDiscrepancy;
		waitOnBusy();
		begin();
		writeByte(DS2482_COMMAND_TRIPLET);
		writeByte(direction ? 0x80 : 0x00);
		end();

		uint8_t status = waitOnBusy();
		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;

		if (id && comp_id)//11
		{
			return 0;
		}
		else
		{
			if (!id && !comp_id && !direction)
			{
				last_zero = i;
			}
		}

		if (direction)
			searchAddress[searchByte] |= searchBit;
		else
			searchAddress[searchByte] &= ~searchBit;

	}

	searchLastDiscrepancy = last_zero;

	if (!last_zero)
		searchLastDeviceFlag = 1;

	for (uint8_t i=0; i<8; i++)
		address[i] = searchAddress[i];

	return 1;
}

#if ONEWIRE_CRC8_TABLE
// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const uint8_t dscrc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
uint8_t DS2482::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		crc = pgm_read_byte(dscrc_table + (crc ^ *addr++));
	}
	return crc;
}
#else
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t DS2482::crc8(uint8_t *addr, uint8_t len)//uint8_t DS2482::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
#endif

// ****************************************
// These are here to mirror the functions in the original OneWire
// ****************************************

// This is a lazy way of getting compatibility with DallasTemperature
// Not all functions are implemented, only those used in DallasTemeperature
void DS2482::reset_search()
{
	wireResetSearch();
}

uint8_t DS2482::search(uint8_t *newAddr)
{
	return wireSearch(newAddr);
}

// Perform a 1-Wire reset cycle. Returns 1 if a device responds
// with a presence pulse.  Returns 0 if there is no device or the
// bus is shorted or otherwise held low for more than 250uS
uint8_t DS2482::reset(void)
{
	return wireReset();
}

// Issue a 1-Wire rom select command, you do the reset first.
void DS2482::select(const uint8_t *addr)//void DS2482::select(const uint8_t rom[8])
{
	wireSelect(addr);//(rom);
}

// Issue a 1-Wire rom skip command, to address all on bus.
void DS2482::skip(void)
{
	wireSkip();
}

// Write a byte.
// Ignore the power bit
void DS2482::write(uint8_t v, uint8_t power)
{
	wireWriteByte(v, power);
}

// Read a byte.
uint8_t DS2482::read(void)
{
	return wireReadByte();
}

// Read a bit.
uint8_t DS2482::read_bit(void)
{
	return wireReadBit();
}

// Write a bit.
void DS2482::write_bit(uint8_t v)
{
	wireWriteBit(v);
}

//------------------------DS2438------------------------
void DS2482::mode(uint8_t mode)
{
	_mode = mode & (DS2438_MODE_CHA | DS2438_MODE_CHB | DS2438_MODE_TEMPERATURE);

	_temperature = 0;
	_voltageA = 0.0;
	_voltageB = 0.0;
	_error = true;
}

void DS2482::update()
{
	if ((_mode & DS2438_MODE_CHA) || _mode == DS2438_MODE_TEMPERATURE)
	{
		uint8_t doConv = _mode & DS2438_MODE_TEMPERATURE;
		if (!startConversion(DS2438_CHA, doConv))
			return;
		if (!readPageZero(data))
			return;
		if (doConv)
		{
			_dtemperature = static_cast<double>((((static_cast<uint16_t>(data[2]) << 8) | (data[1] & 0x0ff)) >> 3) * 0.03125);
			//_temperature = ((static_cast<uint16_t>(data[2]) << 8) | (data[1] & 0x0ff)) >> 3;
		}
			if (doConv & DS2438_MODE_CHA)
			{
			_dvoltageA = (((data[4] << 8) & 0x00300) | (data[3] & 0x0ff)) / 100.0;

			}
	}

	if (_mode & DS2438_MODE_CHB)
	{
		uint8_t doConv = (_mode & DS2438_MODE_TEMPERATURE) && !(_mode & DS2438_MODE_CHA);
		if (!startConversion(DS2438_CHB, doConv))
			return;
		if (!readPageZero(data))
			return;
		if (doConv)
			_dtemperature = static_cast<double>((((static_cast<uint16_t>(data[2]) << 8) | (data[1] & 0x0ff)) >> 3) * 0.03125);
		_dvoltageB = (((data[4] << 8) & 0x00300) | (data[3] & 0x0ff)) / 100.0;
	}

}

uint16_t DS2482::getTemperatureAdc()
{
	return (_temperature = ((static_cast<uint16_t>(data[2]) << 8) | (data[1] & 0x0ff)));
}

double DS2482::getTemperature()
{
	return _dtemperature;
}

uint16_t DS2482::getVoltageAdc(uint8_t ch)
{
	if (ch == DS2438_CHA)
		return (_voltageA = (data[4] << 8) | data[3]);
		//return (_voltageA = ((data[4] << 8) & 0x00300) | (data[3] & 0x0ff));
	else if (ch == DS2438_CHB)
		return (_voltageB = ((data[4] << 8) & 0x00300) | (data[3] & 0x0ff));
	else return 0;
}

double DS2482::getVoltage(uint8_t ch)
{
	if (ch == DS2438_CHA)
		return (_dvoltageA);
	else if (ch == DS2438_CHB)
		return (_dvoltageB);
	else return 0.0;
}

uint8_t DS2482::startConversion(uint8_t ch, uint8_t mode)
{
	if (!selectChannel(ch))
		return 0;
	reset();
	select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	if (mode)
	{
		write(DS2438_TEMPERATURE_CONVERSION, 0);
		do status = read_bit();
		while(!read_bit());
		reset();
		select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	}
	write(DS2438_VOLTAGE_CONVERSION, 0);
	do status = read_bit();
	while(!read_bit());
	return 1;
}

uint8_t DS2482::selectChannel(uint8_t ch)
{
	//uint8_t data[9];
	if (readPageZero(data))
	{
		if (ch == DS2438_CHB)
			data[0] = data[0] | 0x08;
		else
			data[0] = data[0] & 0xf7;
	writePageZero(data);
	return 1;
	}
	return 0;
}

void DS2482::writePageZero(uint8_t *data)
{
	reset();
	select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	write(DS2438_WRITE_SCRATCHPAD, 0);
	write(DS2438_PAGE_0, 0);
	for (int i = 0; i < 8; i++) write(data[i], 0);
	reset();
	select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	write(DS2438_COPY_SCRATCHPAD, 0);
	write(DS2438_PAGE_0, 0);
}

uint8_t DS2482::readPageZero(uint8_t *data)
{
	reset();
	select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	write(DS2438_RECALL_MEMORY, 0);
	write(DS2438_PAGE_0, 0);
	reset();
	select((uint8_t*)(DS2438.Sensor[NUMBER_DS2438][0]) + NUMBER_DS2438 * OFFSET);
	write(DS2438_READ_SCRATCHPAD, 0);
	write(DS2438_PAGE_0, 0);
	for (int i = 0; i < 9; i++) data[i] = read();
	return (crc8(&data[0], 8) == data[8]);
}
//------------------------DS2438------------------------

} // namespace ow
} // namespace kip5


