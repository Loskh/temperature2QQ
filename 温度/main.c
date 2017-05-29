#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit lcdrs=P2^0;
sbit lcdrw=P2^1;
sbit lcden=P2^2;
sbit dout=P1^0;
sbit sclk=P1^1;
sbit cs=P1^2;
uchar code table[]= {0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00};
uchar tt;
uchar dat;
bit send=0;
unsigned char UART_buff;
unsigned char b[5];
void delay(uint a) {
    uint x,y;
    for(x=a; x>0; x--)
			for(y=110; y>0; y--);
}
void uart_send() {
    uchar i;
    for(i=0; i<5; i++) {
        SBUF=b[i]+0x30;
        while(!TI);
        TI=0;
    }
    send=0;
}
void timeinit() {
    TMOD=0x21;
    TH1=0XFD;
    TH1=0XFD;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    TR1=1;
    ET0=1;
    TR0=1;
    SCON=0x50;
    ES=1;
}
uint read_adc() {
    uint voltage_temp=0;
    uchar ucloop=12;
    cs=1;
    sclk=0;
    cs=0;
    while(dout==0);
    sclk=1;
    sclk=0;
    while(ucloop--) {
        sclk=1;
        voltage_temp<<=1;
        if(dout==1)
            voltage_temp+=1;
        sclk=0;
    }
    cs=1;
    return voltage_temp;  
}
void lcdwrite_com(uchar com) {
    lcdrs=0;
    lcdrw=0;
    P0=com;
    delay(15);
    lcden=1;
    delay(15);
    lcden=0;
}
void lcdwrite_data(uchar dat) {
    lcdrs=1;
    lcdrw=0;
    P0=dat;
    delay(15);
    lcden=1;
    delay(15);
    lcden=0;
}
void lcd1602_init() {
    lcden=0;
    lcdwrite_com(0x38);
    lcdwrite_com(0x0c);
    lcdwrite_com(0x06);
    lcdwrite_com(0x01);
}
void lcd1602_ground() {
    uchar i;
    lcdwrite_com(0x80);
    lcdwrite_data('T');
    lcdwrite_data('E');
    lcdwrite_data('M');
    lcdwrite_data('P');
    lcdwrite_data(':');
    lcdwrite_com(0x40);
    for(i=0; i<8; i++)
        lcdwrite_data(table[i]);
    lcdwrite_com(0x80+0x46);
    lcdwrite_data(0x00);

}
void lcd1602_refresh(uint dat) {
    float tvalue;
    uint t;
    tvalue=dat*2.9841+0.5;
    t=(uint)tvalue;
    b[0]=(t/10000)%10;
    b[1]=(t/1000)%10;
    b[2]=(t/100)%10;
    b[3]=(t/10)%10;
    b[4]=(t/1)%10;
    lcdwrite_com(0x80+0x40);		
    lcdwrite_data(0x30+b[0]); 
    lcdwrite_data(0x30+b[1]);
    lcdwrite_data(0x30+b[2]);
		lcdwrite_data('.');
    lcdwrite_data(0x30+b[3]);
    lcdwrite_data(0x30+b[4]);
}



void main() {
    timeinit();
    lcd1602_init();
    lcd1602_ground();
    while(1) {
        if(!tt) {
            dat=read_adc();
            lcd1602_refresh();
				if(send==1)
            uart_send();
        }
    }
}
void time0()interrupt 1 {
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%266;
    tt++;
    if(tt>=2) {
        tt=0;
    }
}
void ser_int (void) interrupt 4 {
    if(RI == 1) {  
        RI = 0;  
        UART_buff = SBUF; 
        if(UART_buff == '&') {
            send = 1;
        } else  {     
            TI = 0;   
        }
    }
}
