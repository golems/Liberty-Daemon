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

#define LIBERTY_RECEIVERS 8

#include <termios.h>
#include <stdio.h>

enum{TRKR_LIB_HS,TRKR_LIB,TRKR_PAT,TRKR_FT,NUM_SUPP_TRKS};

// structure definitions

typedef struct _CNX_PARAMS {
  int cnxType;
  int tracker;
  char port[50];
}*LPCNX_PARAMS,CNX_PARAMS;

typedef struct _CNX_STRUCT {
  int cnxType;
  int trackerType;
  PiTracker* pTrak;
}*LPCNX_STRUCT,CNX_STRUCT;

typedef struct _USB_PARAMS {
  int vid;
  int pid;
  int writeEp;
  int readEp;
}*LPUSB_PARAMS,USB_PARAMS;

typedef struct _READ_WRITE_STRUCT {
  int& keepLooping;
  pthread_t* pthread;
  void* pParam;
}*LPREAD_WRITE_STRUCT,READ_WRITE_STRUCT;


// usb vid/pids for Polehemus trackers
USB_PARAMS usbTrkParams[NUM_SUPP_TRKS]={
  {0x0f44,0xff20,0x04,0x88},  // Lib HS
  {0x0f44,0xff12,0x02,0x82},   // Lib
  {0x0f44,0xef12,0x02,0x82},  // Patriot
  {0x0f44,0x0002,0x02,0x82}};  // Fastrak

// polhemus tracker names
const char* trackerNames[NUM_SUPP_TRKS]={
  "High Speed Liberty","Liberty","Patriot","Fastrak"};


typedef struct {
    struct termios tio;
    int tty_fd;
    FILE *fp;
    char *dev;
    double tf_vq[LIBERTY_RECEIVERS][7];
} liberty_t;

typedef struct{
    float data[LIBERTY_RECEIVERS][7];
} liberty_c_t; 

/* Function: initLiberty(liberty_t *l, const char *dev)
 * Description: 
*/
int initLiberty( liberty_t *l, const char *dev );

/* Function: destroyLiberty(liberty_t *l)
 * Description: 
*/
int destroyLiberty( liberty_t *l );

/* Function: readLiberty(liberty_t *l, liberty_c_t *liberty)
 * Description: 
*/
//int readLiberty( liberty_t *l, liberty_c_t *liberty );

/* Function: readLiberty(void*)
 * Description: read sensor data from the Polhemus
 * Liberty tracker.
*/
void* readLiberty(void*);

#endif
