package main

//#include <Windows.h>
//#include <stdio.h>
//#include <math.h>
/*HANDLE hCom;
double getvalue() {
    hCom=CreateFile(TEXT("COM2"),//COM1口
                    GENERIC_READ|GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
    if(hCom==(HANDLE)-1) {
        return -1;
    }
    SetupComm(hCom,1024,1024);
    DCB dcb;
    GetCommState(hCom,&dcb);
    dcb.BaudRate=9600;
    dcb.ByteSize=8;
    dcb.Parity=NOPARITY;
    dcb.StopBits=ONE5STOPBITS;
    SetCommState(hCom,&dcb);
    DWORD wCount;
    BOOL bReadStat,    bWriteStat;
    PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
    char str[5]= {0};
    bWriteStat=WriteFile(hCom,"&",1,&wCount,NULL);
    bReadStat=ReadFile(hCom,str,5,&wCount,NULL);
    if(!bReadStat) {
        return -2;
    } else {
        str[5]='\0';
        float value=0;
        for(int i=0; i<5; i++)
            value+=(str[i]-0x30)*pow(10,2-i);
        CloseHandle( hCom );
        return value;
    }
}
*/
import "C"

import "strconv"

func readserial() (f string) {
	v := float64(C.getvalue())
	f = strconv.FormatFloat(v, 'f', 2, 64)
	return f
}
