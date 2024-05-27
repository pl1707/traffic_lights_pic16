#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#define _XTAL_FREQ 4000000
signed char dem,i,countxanh,countdo;
signed char setting_confirmed = 0;
const unsigned char code_7seg[10] = // giai ma led 7 doan
    {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void interrupt ngat(void)
{   
    INTF=0;
    RBIF=0;
    if (RB0==0)
    {   
        __delay_ms(10);
        while (RB0==0)
        {       
            for ( i=0;i>=0;i++)// cho led vang chop tat mai mai
            {
                RE0=0;// led xanh tat
                RE1=!RE1;// led vang chop tat
                RE2=0;// led do tat           
                PORTC=0xff;// tat led 7 doan
                PORTD=0xff;// led 7 doan tat
                __delay_ms(1000);
            }            
        }
        
    }
    if (RB1==0)// xanh luon bat de lam duong uutien
    {
        __delay_ms(10);
        while (RB1==0)//
        {
            for (i=0;i>=0;i++)// led xanh sang mai mai~
            {
                RE0=1;// xanh luon sang
                RE1=0;// led vang tat
                RE2=0;// led do tat
                PORTC=0xff;// tat led 7 doan
                PORTD=0xff;// tat led 7 doan
                __delay_ms(1000);
            }
        }
    }
    if (RB2==0)
    {
        __delay_ms(10);
        while (RB2==0)
        {   
            RE0=1;
            RE1=RE2=0;
            countxanh++;
            dem=countxanh;
            PORTC=code_7seg[dem/10];
            PORTD=code_7seg[dem%10];
            __delay_ms(1000);
        
        }
    }
    if (RB3==0)
    {   
        __delay_ms(10);
        while (RB3==0)
        {   
            RE0=0;
            RE1=0;
            RE2=1;
            countdo=countdo++;
            dem=countdo;
            PORTC=code_7seg[dem/10];
            PORTD=code_7seg[dem%10];
            __delay_ms(1000);
        }
    }
    if (RB4 == 0) // Nút xác nh?n th?i gian
    {
        __delay_ms(10);
        while (RB4 == 0); // Ch? th? nút
        setting_confirmed = 1; // ??t c? xác nh?n
    }
}
void main(void) 
{   
    ANSEL=ANSELH=0x00;
    TRISC=0;// so hang chuc
    TRISD=0;//so hang donvi
    TRISE0=0;//led xanh
    TRISE1=0;//led vang
    TRISE2=0;//led do
    TRISB0=1;//22h-5h sang
    TRISB1=1;//duong uu tien
    TRISB2=1;// set time led xanh
    TRISB3=1;//set time led do
    TRISB4=1;// nut xac nhan time
    nRBPU=0;// dientro keo len o PORTB
    WPUB=0xff;
    
    GIE=1;//ngat portB
    RBIF=0;
    RBIE=1;
    IOCB0=IOCB1=IOCB2=IOCB3=1;//ngat o RB0 RB1 RB2 3
    IOCB4 = 1; // Kích ho?t ng?t t? RB4
    PORTC=PORTD=0xC0;
    RE0=RE1=RE2=0;
    __delay_ms(5000);
    while (1)
    {   if (setting_confirmed) // Ch? xác nh?n tr??c khi b?t ??u ch??ng trình
        {
                for (dem=countxanh;dem>=00;dem--)/// denxanh sang tu 20 ve 0
                {
                    RE0=1;//led xanh sang
                    RE1=0;// led vang tat
                    RE2=0;// led do tat
                    PORTC=code_7seg[dem/10];// giai ma so chuc
                    PORTD=code_7seg[dem%10];// so don vi
                    __delay_ms(1000);
                }
                for (dem=5;dem>=00;dem--)// led vang sang tu 5 ve 0
                {
                    RE0=0;// xanh tat
                    RE1=1;// vang sang
                    RE2=0;// do tat
                    PORTC=code_7seg[dem/10];
                    PORTD=code_7seg[dem%10];
                    __delay_ms(1000);
                }
                for (dem=countdo;dem>=0;dem--)// led do sang tu 20 ve 0
                {
                    RE0=0;// led xanh tat
                    RE1=0;// led vang tat
                    RE2=1;// led do sang
                    PORTC=code_7seg[dem/10];
                    PORTD=code_7seg[dem%10];
                    __delay_ms(1000);
                }
        }
    }
}   
    
        
