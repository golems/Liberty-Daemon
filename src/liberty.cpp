#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "PiTracker.h"
#include "liberty.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#define BUFFER_SIZE 1000


int readLiberty(PiTracker *pTrak);

int main(int argc, char* argv[])
{

    CNX_STRUCT cnxStruct;
    CNX_PARAMS cp;
    int cnxSuccess;
    char *port;
    int status;
    // load default parameters
    cnxStruct.cnxType=USB_CNX;  // defined in PiTracker.h
    cnxStruct.trackerType=TRKR_LIB_HS;  // defined in PiTerm.h
    strncpy(cp.port,"/dev/ttyS0", 50); // copy device path to variable

    // Debug code
    fprintf(stdout, "vid: %d\tpid: %d\twriteEp: %d\treadEp: %d\n", usbTrkParams[cnxStruct.trackerType].vid,
                                                           usbTrkParams[cnxStruct.trackerType].pid,
                                                           usbTrkParams[cnxStruct.trackerType].writeEp,
                                                           usbTrkParams[cnxStruct.trackerType].readEp);

    // get communication with the tracker
    PiTracker* pTrak = new PiTracker;
    if(!pTrak)
    {
        printf("Memory Allocation Error creating tracker communications module\n");
        return -3;
    }

    // connect to USB
    if(cnxStruct.cnxType == USB_CNX)
    {
        do
        {
            cnxSuccess = pTrak->UsbConnect(usbTrkParams[cnxStruct.trackerType].vid, 
                                              usbTrkParams[cnxStruct.trackerType].pid, 
                                              usbTrkParams[cnxStruct.trackerType].writeEp, 
                                              usbTrkParams[cnxStruct.trackerType].readEp);
            if(cnxSuccess != 0)
            {
                return -1;
            }
        } while(cnxSuccess != 0);

        // set units to centimeters
        pTrak->WriteTrkData(&setUnits, 1);
        pTrak->WriteTrkData(&toCentimeters, 1);
        pTrak->WriteTrkData(&enter, 1);
        pTrak->WriteTrkData(&singleRead, 1);

        // display connection success message
        fprintf(stdout, "Connected to %s over USB\n", trackerNames[cnxStruct.trackerType]);
    }

    else
    {
        // define RS232 port
        port = "/dev/ttyS0";
        cnxSuccess = cnxStruct.pTrak->Rs232Connect(port);
        if(cnxSuccess != 0)
        {
            // display connection error message
            fprintf(stderr, "Unable to connect to RS232 port %s.\n", port);
            //free(port);
        }

        // display connection success message
        fprintf(stdout, "Connect over RS232 at port %s.\n", port);
    }
    status = readLiberty(pTrak);
    printf("status: %d\n", status);
    return status;
}

    int readLiberty(PiTracker *pTrak)
    {
    BYTE buf[BUFFER_SIZE];
//    LPREAD_WRITE_STRUCT prs;// = (LPREAD_WRITE_STRUCT)pParam;
    int len = 0;
    int initLen = 0;
    int bw;
    int keepLooping=1;
    int startupKey = 67;
    int keyval = 65293;
    int startup = 112;
    // first establish communication and clear out any residual trash data
//    do 
//    {
//        pTrak->WriteTrkData((void*)"\r",1);
//        usleep(100000);
//        len = pTrak->ReadTrkData(buf, BUFFER_SIZE); // keep trying to read until receiving a response
//    } while (!len);

        pTrak->WriteTrkData(&continuousRead,1);
        pTrak->WriteTrkData(&enter,1);
    while (keepLooping)
    {
        len = pTrak->ReadTrkData(buf, BUFFER_SIZE); // read tracker data
        if(len > 0 && len < BUFFER_SIZE)
        {
            buf[len] = 0; // null terminate
        }
        usleep(2000); // rest for 2ms
        printf("%s\n", buf);
    }
    pTrak->CloseTrk();
    }

