#include "liberty.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <assert.h>
#include <ach.h>

using namespace std;

int main()
{
    ach_channel_t chan;
    int r;

    
//	FILE *myfile;
	LIBERTY_STRUCT lbt;

//    printf("size of sData is %ld\n", sizeof(lbt.sData));
//  myfile = fopen("record", "w"); 
//  if (myfile != NULL)
//  	cout << "file opened" << endl;

    r = ach_open(&chan, "liberty", NULL);

    assert( ACH_OK == r );
    cout <<"liberty channel open correctly" << endl;

    if (initLiberty(&lbt, NULL) < 0) {
        cout << "Initialize Liberty failed" << endl;
        exit(1);
    }

    configLiberty(&lbt, "U1");      // data in centimeter
    configLiberty(&lbt, "O*,2,7");  // XYZ and Quaternion
    configLiberty(&lbt, "F1");      // in binary 
    configLiberty(&lbt, "C");       // continuous reading

//    // later read start from the beginning of a new record
    readInitLiberty(&lbt);

    int ret;
    while ( (ret = readRecLiberty(&lbt)) > 0 ) {
//    if ( (ret = readRecLiberty(&lbt, &rec)) > 0 ) {
        ach_put(&chan, lbt.sData, sizeof(lbt.sData));
//        cout << "put data on ach" << endl;
        if (ret == 2)
            printRecLiberty(&lbt, ret);
//        usleep(500);
//        usleep(100000);
    }
//    }
//    }
//        fprintf(stderr, "readRec error, error code: %d\n", ret);  
//        exit(1);
  


//    cout << "nread: " << nread << endl;

//      nread = readLiberty(&lbt, buf, BUFFER_SIZE);
//      cout << "toread" << BUFFER_SIZE << endl;
 //     cout << "nread" << nread << endl;
//		readRecLiberty(&lbt, rec); 
////	   	printf("len is %u\n", len);
//	   	if ( (len > BUFFER_SIZE - 1) || (len < 0) )
//	   		exit(1);
//		buf[len] = '\n';
//		fwrite(rec, 1, RECORD_SIZE, stdout);
//		fwrite(rec, 1, nread, stdout);
//		fwrite(rec, 1, nread, myfile);
//	}
 //  	fclose(myfile);
////    }

 //   destroyLiberty(&lbt);

}
