#include "liberty.h"
#include "liberty_utils.h"

static void printEntryLiberty(unsigned char *entry);

// Commands

// polhemus tracker names
const char* trackerNames[NUM_SUPP_TRKS]={
  "High Speed Liberty","Liberty","Patriot","Fastrak"};

// usb vid/pids for Polehemus trackers
USB_PARAMS usbTrkParams[NUM_SUPP_TRKS]={
  {0x0f44,0xff20,0x04,0x88},  // Lib HS
  {0x0f44,0xff12,0x02,0x82},   // Lib
  {0x0f44,0xef12,0x02,0x82},  // Patriot
  {0x0f44,0x0002,0x02,0x82}};  // Fastrak

// Configure Liberty before reading data
int configLiberty(LPLIBERTY_STRUCT l, std::string s)
{
  char buf[20];
  int key_return; 

  key_return = 0xFF0D;
  const char *com = s.c_str();
  int len, i;

  len = s.length();

  if (len > 20) 
    return -1;

  for (i = 0; i < len; i++) 
    buf[i] = com[i];

  l->cnxs.pTrak->WriteTrkData(buf, len);
  l->cnxs.pTrak->WriteTrkData(&key_return, 1);

  return 0;
}

// Connect to Liberty
int initLiberty( LPLIBERTY_STRUCT l, const char *dev )
{
    int cnxSuccess;
//  int status;

    // initialize all 0
    memset(l, 0, sizeof(LIBERTY_STRUCT));

    // load default parameters
    l->cnxs.cnxType=USB_CNX;  // defined in PiTracker.h
    l->cnxs.trackerType=TRKR_LIB_HS;  // defined in PiTerm.h

    // Debug code
    fprintf(stdout, "vid: %d\tpid: %d\twriteEp: %d\treadEp: %d\n", usbTrkParams[l->cnxs.trackerType].vid,
                                                           usbTrkParams[l->cnxs.trackerType].pid,
                                                           usbTrkParams[l->cnxs.trackerType].writeEp,
                                                           usbTrkParams[l->cnxs.trackerType].readEp);

    // get communication with the tracker
    l->cnxs.pTrak = new PiTracker();
    if(!l->cnxs.pTrak) {
        perror("Memory Allocation Error creating tracker communications module");
        return -3;
    }

    // connect to USB
    if(l->cnxs.cnxType == USB_CNX) {
        cnxSuccess = l->cnxs.pTrak->UsbConnect(usbTrkParams[l->cnxs.trackerType].vid, 
                                              usbTrkParams[l->cnxs.trackerType].pid, 
                                              usbTrkParams[l->cnxs.trackerType].writeEp, 
                                              usbTrkParams[l->cnxs.trackerType].readEp);
        // connect failed
        if(cnxSuccess != 0) {
            fprintf(stdout, "Unable to connect to %s over USB\n", trackerNames[l->cnxs.trackerType]);
            return -1;
        }

        // display connection success message
        fprintf(stdout, "Connected to %s over USB\n", trackerNames[l->cnxs.trackerType]);

        // do initial configure
        // set units to centimeters

 //       configLiberty(l, "P");       // single reading

      return 0;
    } else
      return -1;

}

// Read to the end of an entry
int readInitLiberty( LPLIBERTY_STRUCT l ) 
{

//  unsigned buf[4];
  unsigned char c;
  unsigned char tmpbuf[ENTRY_SIZE];

  c = '\0';

  while (1) {
    while (c != 0x4C) {
      readLiberty(l, &c, 1);
    }

    readLiberty(l, &c, 1);
    if (c == 0x59) {
      readLiberty(l, &c, 1);
      if ( c >= 1 && c <= 8 ) {
        readLiberty(l, &c, 1);
        if ( c == 0x43 ) {
          readLiberty(l, tmpbuf, ENTRY_SIZE-4);
          return 0;
        } else continue;
      } else continue;
    } else continue;
  }

// first clear out any residual trash data
//    do {
//        l->cnxs.pTrak->WriteTrkData((void*)"\r",1);  // send just a cr, should return an short "Invalid Command" response
//        usleep(100000);
//        len=l->cnxs.pTrak->ReadTrkData(tmpbuf,1024);  // keep trying till we get a response
//      } while (!len);

    return 0;

}

// Return positive for the number of sensor read
//         negative for error
int readRecLiberty (LPLIBERTY_STRUCT l)
{

  unsigned char buf[ENTRY_SIZE];
  int channel;
  float *fp;

  if ( readLiberty(l, buf, ENTRY_SIZE) != ENTRY_SIZE )
    return -2;

  if ( (buf[0] != 0x4C) || (buf[1] != 0x59) )
    return -1;

  if ( ! ((buf[STATION_BYTE] > 0) && (buf[STATION_BYTE] < 9))) {
    printf("Sensor number: %hu\n", buf[STATION_BYTE]);
    return -3;
  }

  channel = buf[STATION_BYTE];
  fp = (float *)&buf[DATA_START_BYTE];

  for (int i = 0; i < 7; i++, fp++) 
    l->sData[channel-1][i] = (i < 3) ? ((*fp) / 100.0) : (*fp);   

  return channel;

}


// Return length read
int readLiberty( LPLIBERTY_STRUCT l, unsigned char *buf, size_t left)
{
    int nread;
    int have;
    int toread;

    toread = left;
    have = l->tail - l->head;

    if ( have < left) {
      memmove(buf, l->lbuf+l->head, have);
      left -= have;

      nread = l->cnxs.pTrak->ReadTrkData(l->lbuf, BUFFER_SIZE);
      printf("Data read: %d\n", nread);

      if (nread == 0 || nread < left) {
        fprintf(stderr, "Read tracker data too few\n");
        exit(1);
      }

      memmove(buf + have, l->lbuf, left);
      l->head = left;
      l->tail = nread;
      
      return toread;

    } else {
      memmove(buf, l->lbuf+l->head, left);
      l->head += left;
      return left;
    }


}

void destroyLiberty( LPLIBERTY_STRUCT l ) {
    l->cnxs.pTrak->CloseTrk();
}

void printEntryLiberty(unsigned char *entry)
{
    float data[7];
    int i;
    unsigned char sensor;

    if ( ! ((entry[0] == 0x4C) && (entry[1] == 0x59) )) {
      fprintf(stderr, "print entry error\n");
      exit(1);
    }


    sensor = entry[2];
    if ( !(sensor < 9 && sensor > 0)) {
      fprintf(stderr, "print entry error\n");
      exit(1);
    }
    entry += DATA_START_BYTE;

    printf("Sensor: %hu\n", sensor);
    printf("Data: ");
    for (i = 0; i < 7; i++, entry += 4) {
        data[i] = *((float *)entry);
        printf("%f\t", data[i]);
    }

    printf("\n");
}

void printRecLiberty(LPLIBERTY_STRUCT l, int sensor)
{
    int i;

    printf("Sensor: %d\n", sensor);
    printf("Data: ");
    for (i = 0; i < 7; i++) {
        printf("%f\t", l->sData[sensor-1][i]); 
    }

    printf("\n");
}
