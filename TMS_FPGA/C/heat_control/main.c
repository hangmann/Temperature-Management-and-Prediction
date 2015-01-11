
/*******************************************************************
*
* MAIN.C
*
* This example depicts how to use the heat core on different 
* intensity levels.
* 
* VERSION: 24.06.2013
*
* by Hendrik Hangmann
*
*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <xutil.h>
#include <xuartlite_l.h>
#include <xparameters.h>
//#include <parameters.h>
#include <time_base.h>
#include <system_monitor.h>
#include <heat_core.h>
#include <sensor_grid.h>
#include <limits.h>

unsigned int ** heat_cores;

unsigned int temperature, vccint;


 char * strtok(char * s, const char * toks)
 {
	  static char * last = 0;
	  char * ret = 0;
	  if (toks) {
			s = s ? s : last;
			if (s) {
				 unsigned long tokmap[1 << (CHAR_BIT - 5)] = {0};
				 /* ^ a map of token dividers, containing 256 bits. */
				 char * p;
				 unsigned char tmp;
				 while (tmp = (unsigned char) *toks++) {
					  tokmap[ (tmp & ~31) >> 5 ] |= 1u << (tmp & 31);
				 }
				 p = s;
				 /* We find the first character that is not a token
				  * divider. */
				 while (tokmap[(*p & ~31) >> 5 ] & (1u << (*p & 31))) {
					  ++p;
				 }
				 
				 /* It may be that there are no more tokens. */
				 if (! *p) {
					  last = 0;
				 } else {
					  /* But in this path, there are. */
					  
					  ret = p;
					  /* Now we loop until we get a nontoken
						* character. We want NUL to be a non-token
						* character, so we first modify our map to
						* take that in account.
						*/
					  tokmap[0] |= 1;
					  do {
							++p;
					  } while (!(tokmap[(*p & ~31) >> 5] &
							  (1u << (*p & 31))));
					  /* Now p points at a non-token character. */
						*p = 0;
					  last = p + 1;
				 }
			}
	  }
	  return ret;
 }

unsigned int * heat_core(int i)
{
	if (i == 0) return (unsigned int *) XPAR_HEAT_CORE_0_BASEADDR;
	if (i == 1) return (unsigned int *) XPAR_HEAT_CORE_1_BASEADDR;
	if (i == 2) return (unsigned int *)XPAR_HEAT_CORE_2_BASEADDR;
	if (i == 3) return (unsigned int *)XPAR_HEAT_CORE_3_BASEADDR;
	if (i == 4) return (unsigned int *)XPAR_HEAT_CORE_4_BASEADDR;
	if (i == 5) return (unsigned int *)XPAR_HEAT_CORE_5_BASEADDR;
	if (i == 6) return (unsigned int *)XPAR_HEAT_CORE_6_BASEADDR;
	if (i == 7) return (unsigned int *)XPAR_HEAT_CORE_7_BASEADDR;
	if (i == 8) return (unsigned int *)XPAR_HEAT_CORE_8_BASEADDR;
	if (i == 9) return (unsigned int *)XPAR_HEAT_CORE_9_BASEADDR;
	if (i == 10) return (unsigned int *)XPAR_HEAT_CORE_10_BASEADDR;
	if (i == 11) return (unsigned int *)XPAR_HEAT_CORE_14_BASEADDR;
	if (i == 12) return (unsigned int *)XPAR_HEAT_CORE_15_BASEADDR;
	if (i == 13) return (unsigned int *)XPAR_HEAT_CORE_16_BASEADDR;
	if (i == 14) return (unsigned int *)XPAR_HEAT_CORE_17_BASEADDR;
	if (i == 15) return (unsigned int *)XPAR_HEAT_CORE_18_BASEADDR;
	if (i == 16) return (unsigned int *)XPAR_HEAT_CORE_19_BASEADDR;
	if (i == 17) return (unsigned int *)XPAR_HEAT_CORE_20_BASEADDR;
	if (i == 18) return (unsigned int *)XPAR_HEAT_CORE_21_BASEADDR;
	if (i == 19) return (unsigned int *)XPAR_HEAT_CORE_22_BASEADDR;
	if (i == 20) return (unsigned int *)XPAR_HEAT_CORE_23_BASEADDR;
	if (i == 21) return (unsigned int *)XPAR_HEAT_CORE_24_BASEADDR;
}

void init_heat_cores()
{
//	heat_cores[0] = (unsigned int *) (0x84014000);
//	heat_cores[1] = (unsigned int *) (XPAR_HEAT_CORE_1_BASEADDR);
//	heat_cores[2] = (unsigned int *) (XPAR_HEAT_CORE_2_BASEADDR);
//	heat_cores[3] = (unsigned int *) (XPAR_HEAT_CORE_3_BASEADDR);
//	heat_cores[4] = (unsigned int *) (XPAR_HEAT_CORE_4_BASEADDR);
//	heat_cores[5] = (unsigned int *) (XPAR_HEAT_CORE_5_BASEADDR);
//	heat_cores[6] = (unsigned int *) (XPAR_HEAT_CORE_6_BASEADDR);
//	heat_cores[7] = (unsigned int *) (XPAR_HEAT_CORE_7_BASEADDR);
//	heat_cores[8] = (unsigned int *) (XPAR_HEAT_CORE_8_BASEADDR);
//	heat_cores[9] = (unsigned int *) (XPAR_HEAT_CORE_9_BASEADDR);
//	heat_cores[10] = (unsigned int *) (XPAR_HEAT_CORE_10_BASEADDR);
//	heat_cores[11] = (unsigned int *) (XPAR_HEAT_CORE_14_BASEADDR);
//	heat_cores[12] = (unsigned int *) (XPAR_HEAT_CORE_15_BASEADDR);
//	heat_cores[13] = (unsigned int *) (XPAR_HEAT_CORE_16_BASEADDR);
//	heat_cores[14] = (unsigned int *) (XPAR_HEAT_CORE_17_BASEADDR);
//	heat_cores[15] = (unsigned int *) (XPAR_HEAT_CORE_18_BASEADDR);
//	heat_cores[16] = (unsigned int *) (XPAR_HEAT_CORE_19_BASEADDR);
//	heat_cores[17] = (unsigned int *) (XPAR_HEAT_CORE_20_BASEADDR);
//	heat_cores[18] = (unsigned int *) (XPAR_HEAT_CORE_21_BASEADDR);
//	heat_cores[19] = (unsigned int *) (XPAR_HEAT_CORE_22_BASEADDR);
//	heat_cores[20] = (unsigned int *) (XPAR_HEAT_CORE_23_BASEADDR);
//	heat_cores[21] = (unsigned int *) (XPAR_HEAT_CORE_24_BASEADDR);
//	
	int i;
	for (i = 0; i<22;i++){
		disable(heat_core(i));
	}
}

void clear_array(char* c, int length)
{
	int i;
	for (i=0;i<length;i++)
	{
		c[i] = ' ';
	}
}

	enum ResponseStates {incoming, continuous, nothing};

	enum States state;
	enum ResponseStates response;

	int msec;
	int i;
	int inp;

	char input [15] = "";

enum States {waitForNext=0, startMeasure, inMeasure, endMeasure, pauseAveraging, printData, end};
	
	unsigned int averagingCount;
	unsigned int startTimebaseValue, duration;
	
	unsigned int sensorData[25];

	int i;

void printCounts()
{

	unsigned int time = 0, timeOld = -1;
	unsigned int timebaseValue = 0, timebaseValueOld = *timebase;
	unsigned int overtimeCounter = 0;
	
	state=waitForNext;
		
	while(state != end)
	{
	
		timebaseValue = *timebase;
		
		if (timebaseValueOld > timebaseValue) {
			overtimeCounter++;
		}
		
		time = currentTime(timebaseValue, overtimeCounter);
	
		switch(state){
			case waitForNext:
				if (timeOld < time) {
					averagingCount = 0;
					temperature = 0;
					vccint = 0;
					for (i = 0; i < SENSOR_WIDTH * SENSOR_HEIGHT; i++) {
						sensorData[i] = 0;
					}
					state = startMeasure;
				}
				break;
				
			case startMeasure:
				temperature += readSysmonTemp();
				vccint += readSysmonVccint();
				startMeasurement();
				
				startTimebaseValue = timebaseValue;
				state = inMeasure;
				
				break;
			
			case inMeasure:
				if (timebaseValue > startTimebaseValue) {
					duration = timebaseValue - startTimebaseValue;
				} else {
					duration = OVERTIME_BASEVALUE - startTimebaseValue + timebaseValue;
				}
				if (duration > MEASUREMENT_WAIT) {
					state = endMeasure;
				}
				break;
			
			case endMeasure:
				averagingCount++;
				
				for (i = 0; i < SENSOR_WIDTH * SENSOR_HEIGHT; i++) {
					sensorData[i] += readSensorData(i);
				}
				
				if (averagingCount == AVERAGINGLEVEL) {
					state = printData;
				} else {
					state = startMeasure;
				}
				break;
			
			case printData:
				temperature = temperature / AVERAGINGLEVEL;
				vccint = vccint / AVERAGINGLEVEL;
				
				for (i = 0; i < SENSOR_WIDTH * SENSOR_HEIGHT; i++) {
					xil_printf(" %d", sensorData[i] / AVERAGINGLEVEL);
				}
				
				
				state = end;
				break;
		}
		
		timebaseValueOld = timebaseValue;
		timeOld = time;
	}
}

int interpret_command(char* str)
{
	char * command;
	char * param;
	char * inp;
	int params[2];
	int heater;
	int intensity;
	int count = 0;
	
	//xil_printf("Input uB: %s\n",str);
	
	inp = strtok (str," ");
	
	while (inp != NULL)
	{
		if (count == 0) command = inp;
		else params[count-1] = atoi(inp);
		count++;
		inp = strtok (NULL, " ");
	}
	count = 0;
	clear_array(inp,15);

	if (strcmp(command, "setheater") == 0) 
	{
		if (response == incoming)
		{
			xil_printf("Setting Heater %d (at 0x%x) intensity to %d\n", params[0],
			heat_core(params[0]), params[1]); 
		}
		int ret;
		if (params[1] >= 1){
			enable(heat_core(params[0]));
			set_active_heater(heat_core(params[0]), params[1]);
			ret = 1;
		} else {
			disable(heat_core(params[0]));
			set_active_heater(heat_core(params[0]), 0);
			ret = 0;
		}
		if (params[1] > 32) {
			enable(heat_core(params[0]));
			set_active_heater(heat_core(params[0]), 32);
			ret = 1;
		}
		return ret;
	}
	else if (strcmp(command, "getheater") == 0)
	{
		if (response == incoming)
		{
			xil_printf("Intensity of Heater %d (at 0x%x) is %d\n", params[0], 
			heat_core(params[0]), get_active_heater(heat_core(params[0])));
		}
		return 0;
	}
	else if (strcmp(command, "gettemp") == 0)
	{
		if (response == incoming)
		{
			int temp = sysmon_get_mcelsius();
			xil_printf(" %03d.%03d\n", temp / 1000, temp % 1000);
		}
		return 0; //GET TEMPERATURE
	}
	else if (strcmp(command, "getvcc") == 0)
	{
		if (response == incoming)
		{
			int temp = sysmon_get_vcc();
			xil_printf(" %03d.%03d\n", temp / 1000, temp % 1000);
		}
		return 0;
	}
	else if (strcmp(command, "getcounts") == 0)
	{
		if (response == incoming)
		{
			printCounts();
			xil_printf("\n");
		}
	}
	else if (strcmp(command, "startex") == 0)
	{	
		msec = 0;	
		response = continuous;
		return 0; 
	}
	else if (strcmp(command, "stopex") == 0)
	{
		response = incoming;
		return 0; 
	}
	else if (strcmp(command, "toggleoutput") == 0)
	{
		if (response != nothing)
		{
			response = nothing;
		}
		else
		{
			response = incoming;
			xil_printf("Outputs enabled\n");
		}
		return 0;
	}
	else if (strcmp(command, "setoutput") == 0)
	{
		if (params[0] == 0)
		{
			response = nothing;
		}
		else
		{
			response = incoming;
			xil_printf("Outputs enabled\n");
		}
		return 0;
	}
	
	params[0]=0;
	params[1]=0;
}

	



// busy waiting for 'duration' milliseconds
void wait_for_ms(int duration)
{	
	i= 0;
	unsigned int * timebase 			= (unsigned int *) (XPAR_SIMPLE_TIMEBASE_0_BASEADDR);	
	unsigned int time_in_ms = 0, t_stop, t_start = *timebase;
	// busy waiting
	while (time_in_ms < duration)
	{
		print("");
		t_stop = *timebase;
		time_in_ms = (calc_timediff(t_start, t_stop) / 100000);
		if (!XUartLite_IsReceiveEmpty(XPAR_UARTLITE_1_BASEADDR)) {
			inp=XUartLite_RecvByte (XPAR_UARTLITE_1_BASEADDR);
			
			if (inp != '\n'){
				input[i] = inp;
				inp = 0;
				i++;
			} else {
				i=0;
				interpret_command(input);
				clear_array(input, 15);
			}
		}
	}
}

int main (void) 
{
	init_heat_cores();
	
	response = incoming;
	
	unsigned int tt;
	
	
//	inp=XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR); 
	//configureSysmon();
	int first = 1;

	while(1) {
		
		if (response == continuous)
		{
			if (first==1){
				xil_printf("##################47_24_latch#################\r\n");	
				first = 0;
			}
			xil_printf("%05d", msec);
			printCounts();
			xil_printf(" %03d.%03d", temperature / 1000, temperature % 1000);
			xil_printf(" %01d.%03d", vccint / 1000, vccint % 1000);
			
			int heater;
			for (heater = 0; heater < 22; heater ++)
			{
				xil_printf(" %d", get_active_heater(heat_core(heater)));
			}
			
			xil_printf("\r\n");
			msec++;
		}

		wait_for_ms(100);
	}

	return 0;
	
}


