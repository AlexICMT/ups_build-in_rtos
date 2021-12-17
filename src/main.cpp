/* ups_build-in
 * /ups_build-in/src/main.cpp
 * main.cpp
 * 
 *  Created on: 11 нояб. 2021 г.
 *  	  Time: 14:15:52
 *      Author: AlexICMT
 */
#include "framework.hpp"
#include "app.hpp"
#include "adc.hpp"
#include "dma.hpp"

kip5::adc::Adc* asensors_switch = nullptr;
kip5::dma::Dma* dma_adc_switch = nullptr;

#include "eclipse_header_wrapper.hpp"

C_HEADERS_WRAPPER_BEGIN(freertos)
#include "task.h"
C_HEADERS_WRAPPER_END(freertos)

void vTask1(void *pv)
{

	for(;;)
	{
		dma_adc_switch->Dma_launch_adc();
		asensors_switch->readAdc();
	}
	vTaskDelete(NULL);
}

void vTask2(void *pv)
{
	for(;;)
	{
	//charPainterMin->setAutoNextLine(true);
	if(!buttons.getKey(kip5::KEY::ALL))
	{
	charPainterMin->clear();

	charPainterMin->printf("x00=%04d,  x01=%04d\n", asensors_switch->r_adc_ch0(), asensors_switch->r_adc_ch1());
	charPainterMin->printf("x02=%04d,  x03=%04d\n", asensors_switch->r_adc_ch2(), asensors_switch->r_adc_ch3());
	charPainterMin->printf("x04=%04d,  x05=%04d\n", asensors_switch->r_adc_ch4(), asensors_switch->r_adc_ch5());
	charPainterMin->printf("x06=%04d,  x07=%04d\n", asensors_switch->r_adc_ch6(), asensors_switch->r_adc_ch7());
	charPainterMin->printf("Trf =%04d,  Vrf =%04d", asensors_switch->r_adc_chT(), asensors_switch->r_adc_chV());

	charPainterMin->update();
	}
	if(buttons.getKey(kip5::KEY::SELECT))
	{
		charPainterMin->clear();
		charPainterMin->printf("SELECT is pressed");
		charPainterMin->update();
	}
	//kip5::delay::delay_ms(100);
	vTaskDelay(5000);
	}
	vTaskDelete(NULL);
}



int main()
{
	init_before_rtos();
	dma_adc_switch 	= 	new kip5::dma::Dma();
	asensors_switch = 	new kip5::adc::Adc();

	charPainterMin->setAutoNextLine(true);
	charPainterMin->clear();

	//xTaskCreate(vTask1, "1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	//xTaskCreate(vTask2, "2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vTask1, "1", 1000, NULL, 1, NULL);//configMINIMAL_STACK_SIZE
	xTaskCreate(vTask2, "2", 1000, NULL, 2, NULL);

	vTaskStartScheduler();
	//for(;;);
}

