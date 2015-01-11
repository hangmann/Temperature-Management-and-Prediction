/*******************************************************************
*
* SYSTEM_MONITOR.H
*
* Read temperatures and supply voltage from the system monitor.
* 
* VERSION: 19.04.2013
*
* by Andreas Agne, Hendrik Hangmann
*
*******************************************************************/
unsigned int * sysmon	 		 = (unsigned int *) 0x83800000;

const float TEMPERATURE_FACTOR = 0.0076900482177734378;

unsigned int sysmon_diode2millikelvin(unsigned int adc_value)
{
	int raw = adc_value;
	raw = raw >> 6;
	raw = raw*503975/1024;
	return raw;
}

unsigned int sysmon_adc2vcc(unsigned int adc_value)
{
    return (adc_value*3000) >> 16;
} 

// get supply voltage
unsigned int sysmon_get_vcc()
{
	return sysmon_adc2vcc(sysmon[0x81]); 
}

unsigned int sysmon_diode2millikelvin2(unsigned int adc_value)
{
	int raw;
	float temp = adc_value;
	temp *= TEMPERATURE_FACTOR;
	raw = 1000*temp;
	return raw;
}

// get milli celsius
unsigned int sysmon_get_mcelsius()
{
	return sysmon_diode2millikelvin2(sysmon[0x80]) - 273150;
}
