/* ups_build-in
 * /ups_build-in/inc/adc_inc/adc.hpp
 * adc.hpp
 * 
 *  Created on: 17 нояб. 2021 г.
 *  	  Time: 11:31:05
 *      Author: AlexICMT
 */

#ifndef INC_ADC_INC_ADC_HPP_
#define INC_ADC_INC_ADC_HPP_

#include <cstdint>

#include "thread.hpp"

namespace kip5 {

namespace handlers {
extern "C" void ADC_IRQHandler();
} // namespace handlers

namespace adc {

class Adc : public thread::Thread
{
public:
	Adc();

	Adc(const char* name, int stackDepth, int priority);

	Adc(const Adc&) = delete;

	struct AdcData
		{
			uint16_t adc_ch0;
			uint16_t adc_ch1;
			uint16_t adc_ch2;
			uint16_t adc_ch3;
			uint16_t adc_ch4;
			uint16_t adc_ch5;
			uint16_t adc_ch6;
			uint16_t adc_ch7;
		};

	struct AdcDataAdd
			{
				uint16_t adc_chT;
				uint16_t adc_chV;
			};


static constexpr int adcBufferSize = 8;
static constexpr int adcHalfBufferSize = adcBufferSize / 2;

static AdcData adcBuffer[adcBufferSize];
static AdcData adcBuffer1;
static AdcDataAdd adcaddBuffer1;
static int adcBufferIndex;

static int ADC_READY_4_SAMPLES;

//zxc выбираем либо первые 4 , либо вторые 4. в зависимости от того какой индекс текущий.те старую половину усредняем
static inline const AdcData* getBuffer() {return adcBuffer + adcHalfBufferSize - (adcBufferIndex & ~(adcHalfBufferSize - 1));}

static void initInternalADCPorts();

void initAdcForSingleConvertion();
void initAdcForSwitchingConvertion();
void initAdcForSwitchingConvertionSynch();

static void initAdcForSwitchingConvertion_2048();

static void startConversionAdc1(int channel);
static void startConversionAdc2(int channel);

static int waitConversionAndReadAdc1();
static int waitConversionAndReadAdc2();

static int readAdc1(int channel);
static int readAdc2(int channel);

void readAdc();

inline uint16_t r_adc_ch0() const {return adc_ch0;}
inline uint16_t r_adc_ch1() const {return adc_ch1;}
inline uint16_t r_adc_ch2() const {return adc_ch2;}
inline uint16_t r_adc_ch3() const {return adc_ch3;}
inline uint16_t r_adc_ch4() const {return adc_ch4;}
inline uint16_t r_adc_ch5() const {return adc_ch5;}
inline uint16_t r_adc_ch6() const {return adc_ch6;}
inline uint16_t r_adc_ch7() const {return adc_ch7;}
inline uint16_t r_adc_chT() const {return adc_chT;}
inline uint16_t r_adc_chV() const {return adc_chV;}

inline float r_CH0() const {return CH0;}
inline float r_CH1() const {return CH1;}
inline float r_CH2() const {return CH2;}
inline float r_CH3() const {return CH3;}
inline float r_CH4() const {return CH4;}
inline float r_CH5() const {return CH5;}
inline float r_CH6() const {return CH6;}
inline float r_CH7() const {return CH7;}

protected:

virtual void execute() override;

private:
	static constexpr int CHANNEL = 8;
	static constexpr int WINDOWS = 4;
	static constexpr int DENOMINATOR = 2;

	int adc_ch0 = 0;
	int adc_ch1 = 0;
	int adc_ch2 = 0;
	int adc_ch3 = 0;
	int adc_ch4 = 0;
	int adc_ch5 = 0;
	int adc_ch6 = 0;
	int adc_ch7 = 2048;
	int adc_chT = 0;
	int adc_chV = 0;

	float CH0 = 0.0;
	float CH1 = 0.0;
	float CH2 = 0.0;
	float CH3 = 0.0;
	float CH4 = 0.0;
	float CH5 = 0.0;
	float CH6 = 0.0;
	float CH7 = 0.0;

	friend void handlers::ADC_IRQHandler();
};

} // namespace adc
} // namespace kip5

#endif /* INC_ADC_INC_ADC_HPP_ */
