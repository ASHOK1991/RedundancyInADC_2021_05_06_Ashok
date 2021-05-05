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
  float temp;
  float temp_1;
  float ma;
  int i,j,n=7;
  int m=5;
  float a;
  float s = 1, t =1, k=0;

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

	/************ADC1 value calculate*************/
  	temp = (0.5 + (0.1 * adc_value_0));
	//ADC2 value calculate
  	temp_1 = (1.5 + (0.08 * adc_value_1));
  	//Moving Avrage
  	ma = ((temp + temp_1)/2);
  	//convart to decimal
  	int ma_1 = ma;
	/********************************************/ 

  	/**********Display the ADC value from user input****************/
  	printf("ADC1= %f\n",temp);
	printf("ADC2= %f\n",temp_1);
	printf("MA(Angle)= %f\n",ma);
	printf("MA_1(Angle)= %d\n",ma_1);
	/***************************************************************/

	/******************Conditions -> If Throttle pedel angle exceed 30degree******************/
	if (ma >= 30)
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
				s=s*(ma_1-x[j]);
				t=t*(x[i]-x[j]);
			}
		}
			k = k+((s/t)*y[i]);
	}

	printf("Final Torque = %f\n", k);
	s=0; t=1; i=0; j=0; k=0; n=7;
	/**********************************************************************/
}
}
	/*****************************END**************************************/