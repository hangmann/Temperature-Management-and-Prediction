/*******************************************************************
*
* MAIN.C
*
* Program's main file. Instantiates models, learns parameters, etc
*
*
* VERSION: 24.02.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include <limits.h>
#include "main.h"
#include "model.h"
#include "measurement_grid.h"

/**
 * Splits a char array in several char arrays
 * at given char 'toks'
 */
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

/**
 * tries top open file from path
 */
void open_measurements(FILE ** file, char ** path)
{
	// argv[1] should be filename
	*file = fopen( *path, "r" );

	/* fopen returns 0, the NULL pointer, on failure */
	if ( *file == 0 )
	{
		printf( "ERROR: Could not open file\n" );
		exit(EXIT_FAILURE);
	}
}

void parse_measurements(FILE ** file)
{
	char * timestep_measurement;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, *file)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);
		timestep_measurement = strtok (line," ");
		int i = 0;
		int second;
		float data[3];
		while (timestep_measurement != NULL)
		{
			if (i == 0) second = atoi(timestep_measurement);
			else data[i-1] = atof(timestep_measurement);
			i++;
			timestep_measurement = strtok (NULL, " ");
		}
		printf("%d: %f, %f, %f\n",second, data[0], data[1], data[2]);
	}
	fclose( *file );
}

int main (int argc, char **argv)
{
	FILE * file;
	rc_network * rcn;

	rcn = create(5,5,2);
	init(rcn);

	if ( argc != 2 )
	{
		/* We print argv[0] assuming it is the program name */
		printf( "Start this program as follows:\n %s PATH_TO_MEASUREMENTS\n", argv[0] );
	}
	else
	{
		open_measurements(&file, &argv[1]);
		parse_measurements(&file);
	}

    exit(EXIT_SUCCESS);
}
