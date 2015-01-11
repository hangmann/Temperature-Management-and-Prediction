
unsigned int* timebase = (unsigned int*)XPAR_SIMPLE_TIMEBASE_0_BASEADDR;
unsigned int* sensors = (unsigned int*)XPAR_THERMAL_MONITOR_0_BASEADDR;

// frequency of the processor in Hz
const int PROCESSOR_FREQUENCY = 100000000;
// refresh frequenzy in Hz
const int REFRESH_FREQUENCY = 10;
// level of averaging
const int AVERAGINGLEVEL = 1;
// number of cycles to wait for measurement
const int MEASUREMENT_WAIT = 200000;

const int OVERTIME_BASEVALUE = 0xFFFFFFFF;

const int SENSOR_WIDTH = 5;
const int SENSOR_HEIGHT = 5;



unsigned int currentTime(unsigned int pTimebaseValue, unsigned int pOvertimeCounter) {
	unsigned int divider = PROCESSOR_FREQUENCY / REFRESH_FREQUENCY;
	unsigned int timebaseValueToTime, overtimeCounterToTime;
	
	timebaseValueToTime = pTimebaseValue / divider;
	overtimeCounterToTime = (unsigned int) (pOvertimeCounter * (OVERTIME_BASEVALUE / divider));
	
	return timebaseValueToTime + overtimeCounterToTime;
}

void startMeasurement() {
	sensors[0] = 0;
}

unsigned int readSensorData(int pNumber) {
	return sensors[pNumber];
}

// origin is at lower left: (1, 1)
int coordinatesToNumber(int pX, int pY) {
	return (pY - 1) * SENSOR_WIDTH + (pX - 1);
}

void configureSysmon() {
		// reset sysmon
		sysmon[0x0] = 0xA;
		
		// sequence register 0: 0000 0111 0000 0001
		// sequence register 1: 0000 0000 0000 0000
		// sequence register 2: 0000 0111 0000 0000
		sysmon[0xC8] = 0x701;
		sysmon[0xC9] = 0x0;
		sysmon[0xCA] = 0x700;
		sysmon[0xCC] = 0x0;
		sysmon[0xCD] = 0x0;
		
		// configuration register 0: 0011 0000 0000 0000
		// configuration register 1: 0010 0000 1111 0000
		sysmon[0xC0] = 0x3000;
		sysmon[0xC1] = 0x20F0;
		//sysmon[0xC2] = 0x2000;
		
		sysmon[0x1];
}

unsigned int convTemp(unsigned int pTemp) {
	//return (((pTemp >> 6) * 503975) >> 10) - 273150;
	return ((pTemp * TEMPERATURE_FACTOR) - 273.15) * 1000;
}

unsigned int convVccint(unsigned int pVccint) {
	return ((pVccint) * 3 * 1000) >> 16;
	//return ((pVccint >> 6) * 3 * 1000) >> 10;
}

unsigned int readSysmonTemp() {
	return convTemp(sysmon[0x80]);
}

unsigned int readSysmonVccint() {
	return convVccint(sysmon[0x81]);
}


