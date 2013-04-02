// liberty.h

/*
Polhemus Tracker Terminal version 1.0.0 -- Terminal Interface to Polhemus Trackers: Fastrak, Patriot, and Liberty
Copyright  ©  2009  Polhemus, Inc.

This file is part of Tracker Terminal.

Tracker Terminal is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tracker Terminal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tracker Terminal.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************

Tracker Terminal version 1.0.0 uses the libusb library version 1.0
libusb Copyright © 2007-2008 Daniel Drake <dsd@gentoo.org>
libusb Copyright © 2001 Johannes Erdfelt <johannes@erdfelt.com>
Licensed under the GNU Lesser General Public License version 2.1 or later.
*/

#ifndef LIBERTY_H_
#define LIBERTY_H_

#define LIBERTY_RECEIVERS		8
#define ENTRY_SIZE				0x24 
#define STATION_BYTE			0x2
#define DATA_START_BYTE			0x8
#define CHANNEL_NUM				0x8

#include "liberty_utils.h"

typedef struct {
    CNX_STRUCT cnxs;
    float sData[CHANNEL_NUM][7];		// data read from CHANNEL_NUM sensors
    unsigned char lbuf[BUFFER_SIZE+1];
    int head;
    int tail;
} *LPLIBERTY_STRUCT, LIBERTY_STRUCT;

int initLiberty( LPLIBERTY_STRUCT, const char * );

void destroyLiberty( LPLIBERTY_STRUCT );

int readLiberty( LPLIBERTY_STRUCT, unsigned char *, size_t );
int readInitLiberty( LPLIBERTY_STRUCT );
int readEntryLiberty (LPLIBERTY_STRUCT);

int configLiberty( LPLIBERTY_STRUCT, std::string s);
void printSensorLiberty( LPLIBERTY_STRUCT, int sensor);

#endif
