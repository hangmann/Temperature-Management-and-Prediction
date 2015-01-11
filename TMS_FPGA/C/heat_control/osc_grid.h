
//#include <sensor_grid.h>

enum States {waitForNext=0, startMeasure, inMeasure, endMeasure, pauseAveraging, printData, end};

enum States state;



unsigned int averagingCount;
unsigned int startTimebaseValue, duration;

unsigned int temperature, vccint;

int i;

unsigned int sensorData[25];

void printCount(){

	unsigned int time = 0, timeOld = -1;
	unsigned int timebaseValue = 0, timebaseValueOld = *timebase;
	unsigned int overtimeCounter = 0;
	
	state = waitForNext;

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
				
				//xil_printf("%04d", time - 1);
				for (i = 0; i < SENSOR_WIDTH * SENSOR_HEIGHT; i++) {
					xil_printf(" %d", sensorData[i] / AVERAGINGLEVEL);
				}				
				state = end;
				break;
			case end:
				//nothing
				xil_printf("\r\n");
			}
		}
		timebaseValueOld = timebaseValue;
		timeOld = time;
	}
	
