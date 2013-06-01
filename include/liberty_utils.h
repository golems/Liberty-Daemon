#ifndef LIBERTY_UTILS_H_
#define LIBERTY_UTILS_H_

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "PiTracker.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>


#define ENTRY_SIZE				0x24 
#define STATION_BYTE			0x2
#define DATA_START_BYTE			0x8
#define CHANNEL_NUM				0x8

#define BUFFER_SIZE    1024

enum {TRKR_LIB_HS,TRKR_LIB,TRKR_PAT,TRKR_FT,NUM_SUPP_TRKS};

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
