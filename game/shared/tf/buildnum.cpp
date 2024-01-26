//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//========================================================================//

#include <stdlib.h>
#include <string.h>
#include "tier1/strtools.h"

// from 01-Jan-1900 to 18-Feb-2023 = 44973
// 44973+1=44974
// 44974-365=44609

char *date = __DATE__ ;

char *mon[12] =
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

char mond[12] =
{
	31, // "Jan"
	28, // "Feb"
	31, // "Mar"
	30, // "Apr"
	31, // "May"
	30, // "Jun"
	31, // "Jul"
	31, // "Aug"
	30, // "Sep"
	31, // "Oct"
	30, // "Nov"
	31  // "Dec"
};

// returns days since Feb 18 2023
int build_number( void )
{
	int m = 0;
	int d = 0;
	int y = 0;
	static int b = 0;

	if( b != 0 )
	{
		return b;
	}

	for( m = 0; m < 11; m++ )
	{
		if( Q_strnicmp( &date[0], mon[m], 3 ) == 0 )
			break;
		d += mond[m];
	}

	d += atoi( &date[4] ) - 1;

	y = atoi( &date[7] ) - 1900;

	b = d + (int)( (y - 1) * 365.25 );

	if( ( (y % 4) == 0 ) && m > 1 )
	{
		b += 1;
	}

	b -= 44609; // Feb 18 2023

	return b;
}
