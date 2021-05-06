/** @file
 *  @brief Main file.
 *  @description Just contains some example code. Adapt it in the way you like.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "drivers/adc_driver/adc_driver.h"
#include "drivers/error_led/error_led.h"

int main(int argc, char *argv[]) 
{
  adc_value_t value = 0;

  error_led_init();
  adc_init(ADC_CHANNEL0);
  adc_init(ADC_CHANNEL1);
  int adc_value_0;
  int adc_value_1;
  float adc0;
  float adc1;
  float ma;
  int angle;
  int i,j;
  int n=7; //0kmph
  //int n=8; //50kmph
  float s = 1, t =1, ft=0;

while(1)
{
    /***As per the given graph deatails i have calculated angle and torque values for 0kmph***/
    float y[7] = {0,18,35,50,62,82,103}; //--> 0kmph
    float x[7] = {0,3,6,9,12,18,24}; //--> 0kmph
    /*****************************************************************************************/

    /***As per the given graph deatails i have calculated angle and torque values for 50kmph***/
    //float y[8] = {-30,-10,10,30,45,72,95,120}; //--> 50kmph
    //float x[8] = {0,3,6,9,12,18,24,30}; //--> 50kmph
    /******************************************************************************************/

	/******Get user input ADC1 and ADC2 values******/
  	scanf("%d",&adc_value_0);
	scanf("%d",&adc_value_1);
	/************************************/

	/************ADC value calculate*************/
  	//ADCCHANNEL0 value calculate
    	adc0 = (0.5 + (0.1 * adc_value_0));
	//ADCCHANNEL1 value calculate
  	adc1 = (1.5 + (0.08 * adc_value_1));
  	//Find Moving Avrage
  	ma = ((adc0 + adc1)/2);
  	//Find Throttle Pedal Angle
  	angle = (int)ma;
	/********************************************/ 

  	/**********Display the ADC value from user input****************/
  	printf("ADC1= %f\n",adc0);
	printf("ADC2= %f\n",adc1);
	printf("Moving Average = %f\n",ma);
	printf("Pedal Angle = %d\n",angle);
	/***************************************************************/

	/******************Conditions -> If Throttle pedel angle exceed 30degree******************/
	if (angle >= 30)
		{
		printf("Pedal Angle value is wrong and below mentioned ADC channel is fault\n");
		
		/*******ADC0 wrong value and ADC1 is correct value --> LED ON*******/ 
		if((adc_value_0 >= 323) && (adc_value_1 <= 322))
  		{

			adc_read_set_output(ADC_CHANNEL0, adc_value_0, ADC_RET_NOK);
			adc_read(ADC_CHANNEL0, &value);
			error_led_set(true);
		}
		/*******ADC0 channel value is correct and ADC1 channel value is wrong val--> LED ON*******/
		else if((adc_value_1 >= 322) && (adc_value_0 < 323))
  		{
			adc_read_set_output(ADC_CHANNEL1, adc_value_1, ADC_RET_NOK);
			adc_read(ADC_CHANNEL1, &value);
			error_led_set(true);
		}
		/*******Both ADC channel values are wrong value  --> LED ON*******/
		else if((adc_value_0 >= 323) && (adc_value_1 >= 323))
  		{
  			adc_read_set_output(ADC_CHANNEL0, adc_value_0, ADC_RET_NOK);
			adc_read(ADC_CHANNEL0, &value);
			adc_read_set_output(ADC_CHANNEL1, adc_value_1, ADC_RET_NOK);
			adc_read(ADC_CHANNEL1, &value);
			error_led_set(true);
		}
		else{}
		}
	/*******Throttele pedal angle value below 30 degree and
	/*******ADC0 wrong value and ADC1 is correct value --> LED ON*******/ 	
	else if((adc_value_0 >= 323) && (adc_value_1 <= 323))
  		{

			adc_read_set_output(ADC_CHANNEL0, adc_value_0, ADC_RET_NOK);
			adc_read(ADC_CHANNEL0, &value);
			error_led_set(true);
		}
	/*******Throttele pedal angle value below 30 degree and
	/*******ADC0 value is correct and ADC1 value is wrong --> LED ON*******/ 
	else if((adc_value_1 >= 323) && (adc_value_0 <= 323))
  		{
			adc_read_set_output(ADC_CHANNEL1, adc_value_1, ADC_RET_NOK);
			adc_read(ADC_CHANNEL1, &value);
			error_led_set(true);
		}
	/*******Throttele pedal angle value below 30 degree and
	/*******Both ADC channel values are correct value  --> LED OFF*******/ 
	else
		{
			adc_read_set_output(ADC_CHANNEL0, adc_value_0, ADC_RET_OK);
			adc_read(ADC_CHANNEL0, &value);
			adc_read_set_output(ADC_CHANNEL1, adc_value_1, ADC_RET_OK);
			adc_read(ADC_CHANNEL1, &value);
			error_led_set(false);
		}
	/********Final Torque calculation with exact throttle pedal angle*******/
	for (i=0; i<n; i++)
	{
		s=1;
		t=1;
		for (j=0; j<n; j++)
		{
			if (j!=i)
			{
				s=s*(angle-x[j]);
				t=t*(x[i]-x[j]);
			}
		}
			ft = ft+((s/t)*y[i]);
	}

	printf("Final Torque = %f\n", ft);
	s=0; t=1; i=0; j=0; ft=0; n=7;
	/**********************************************************************/
}
}
	/*****************************END**************************************/
