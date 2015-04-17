								/*******************************************************************
*
* HEAT_CORE.H
*
* Activation and configuration of heat cores.
* 
* VERSION: 19.04.2013
*
* by Hendrik Hangmann
*
*******************************************************************/

#include <math.h> 
#include <stdio.h> 
#include <limits.h>

#define B(x) ( \
  (x[0] == '1') << 31 | \
  (x[1] == '1') << 30 | \
  (x[2] == '1') << 29 | \
  (x[3] == '1') << 28 | \
  (x[4] == '1') << 27 | \
  (x[5] == '1') << 26 | \
  (x[6] == '1') << 25 | \
  (x[7] == '1') << 24 | \
  (x[8] == '1') << 23 | \
  (x[9] == '1') << 22| \
  (x[10] == '1') << 21 | \
  (x[11] == '1') << 20 | \
  (x[12] == '1') << 19 | \
  (x[13] == '1') << 18 | \
  (x[14] == '1') << 17 | \
  (x[15] == '1') << 16 | \
  (x[16] == '1') << 15 | \
  (x[17] == '1') << 14 | \
  (x[18] == '1') << 13 | \
  (x[19] == '1') << 12 | \
  (x[20] == '1') << 11 | \
  (x[21] == '1') << 10 | \
  (x[22] == '1') << 9 | \
  (x[23] == '1') << 8 | \
  (x[24] == '1') << 7 | \
  (x[25] == '1') << 6 | \
  (x[26] == '1') << 5 | \
  (x[27] == '1') << 4 | \
  (x[28] == '1') << 3 | \
  (x[29] == '1') << 2 | \
  (x[30] == '1') << 1 | \
  (x[31] == '1') << 0 )
  

int j =0;  
int active_heaters = 0;
char zero[] = "00000000000000000000000000000000";

// calculates 32 bit binary representation of an int
char *dec_to_bin (int number)
{
	int i;
	int k;
	int n = number;
	char * erg;
	
	for (i = 31; i >= 0; i--)
	{
		k = n >> i;

		if (k & 1)
			erg[31-i] = '1';
		else
			erg[31-i] = '0';
	}
}


// get active heaters (1-32) in the heat core as char, representing 32 bit binary code
//char *get_heater_activator( unsigned int* heat_core )
//{
//	return bin;
//}

int read_active( unsigned int* heat_core)
{
	return heat_core[1];
}

int resolve_heater_lvl (int reg_value)
{
	if (reg_value == 0)
		return 0;
	if (reg_value == INT_MIN)//-2147483648LL)
		return 1;
	if (reg_value == -1073741824)
		return 2;
	if (reg_value == -536870912)
		return 3;
	if (reg_value == -268435456)
		return 4;
	if (reg_value == -134217728)
		return 5;
	if (reg_value == -67108864)
		return 6;
	if (reg_value == -33554432)
		return 7;
	if (reg_value == -16777216)
		return 8;
	if (reg_value == -8388608)
		return 9;
	if (reg_value == -4194304)
		return 10;
	if (reg_value == -2097152)
		return 11;
	if (reg_value == -1048576)
		return 12;
	if (reg_value == -524288)
		return 13;
	if (reg_value == -262144)
		return 14;
	if (reg_value == -131072)
		return 15;
	if (reg_value == -65536)
		return 16;
	if (reg_value == -32768)
		return 17;
	if (reg_value == -16384)
		return 18;
	if (reg_value == -8192)
		return 19;
	if (reg_value == -4096)
		return 20;
	if (reg_value == -2048 )
		return 21;
	if (reg_value == -1024)
		return 22;
	if (reg_value == -512)
		return 23;
	if (reg_value == -256)
		return 24;
	if (reg_value == -128)
		return 25;
	if (reg_value == -64)
		return 26;
	if (reg_value == -32)
		return 27;
	if (reg_value == -16)
		return 28;
	if (reg_value == -8)
		return 29;
	if (reg_value == -4)
		return 30;
	if (reg_value == -2)
		return 31;
	if (reg_value == -1)
		return 32;
}

// get active heaters (1-32) in the heat core
int get_active_heater( unsigned int* heat_core )
{
	return resolve_heater_lvl(heat_core[1]);
	/*char* binNum = dec_to_bin(heat_core[1]);
	xil_printf("%d %s\n", heat_core[1], binNum);
	int count = 0;
	int i;
	for( i = 0; i < 31 ; i++)
	{
		if (binNum[i] == '1')
			count++;
	}
	
	if (count==0)
		return 0;
	else if (count == 1)
		return 1;
	else 
		return count + 1;*/
}


// set active heaters (1-32) in the heat core
void set_active_heater( unsigned int* heat_core, int active )
{
	int i;
	char bin[] = "00000000000000000000000000000000";
	int temp = active;
	
	for( i = 0; i < 31 ; i++)
	{ 	
		if (i < temp)
		{ bin[i] = '1';
		} else 
		 bin[i] = '0';
		
	}
	int decimal = B(bin);
	heat_core[1] = decimal;
	
}	

// turns on heat core
void enable( unsigned int* heat_core )
{
	heat_core[0] = 1;
}

//turns off heat core
void disable( unsigned int* heat_core)
{
	heat_core[0] = 0;
}

//increases to the next higher percentage
//void increase( unsigned int* heat_core)
//{
//	for ( j = 31 ; j > 0; j--) 
//	{
//		bin[j] = bin[j-1];
//	}
//	bin[0] = '1';
//	active_heaters = active_heaters + 1;
//	heat_core[1] = B(bin);
//}

//decreases to the next lower percentage
//void decrease( unsigned int* heat_core)
//{
//	for ( j = 0 ; j < 31; j++) 
//	{
//		bin[j] = bin[j+1];
//	}
//	bin[31] = '0';
//	active_heaters = active_heaters - 1;	
//	heat_core[1] = B(bin);
//}

// 1 if heater is heater enabled, 0 otherwise
int enabled( unsigned int* heat_core )
{
	return heat_core[0];
}
