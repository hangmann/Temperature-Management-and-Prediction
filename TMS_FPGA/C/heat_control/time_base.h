/*******************************************************************
*
* TIME_BASE.H
*
* Offers wait_for_ms(x) 
* 
* VERSION: 19.04.2013
*
* by Andreas Agne, Hendrik Hangmann
*
*******************************************************************/
#include <parameters.h>

// calculates difference for 32 bit register
unsigned int calc_timediff( unsigned int start, unsigned int stop )
{
	if ( start <= stop ) 
	{
		return ( stop - start );
	} 
	return ( 4294967295UL - start + stop ); 
}



