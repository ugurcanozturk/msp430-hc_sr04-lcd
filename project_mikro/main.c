#include "io430.h"
#include "in430.h"
#ifndef _LCD_4BIT_H_
#define _LCD_4BIT_H_
 #define    LCD_DATA         P1OUT

#define    LCD_RS           P2OUT_bit.P3                 // P2.3 is defined as lcd reset
#define    LCD_EN           P2OUT_bit.P4               // P2.4 is defines as lcd enable


#define    TRIG              P2OUT_bit.P0                // P2.0 is defined as trigger

#define    ECH            P2IN_bit.P1                 // P2.1 is defined as echo

#define    LCD_STROBE()     ((LCD_EN=1),(LCD_EN=0)) //  LCD AYARLARI

int time;
extern void lcd_write(unsigned char);//LCD ILE ILGILI ALT PROGRAM TANIMLAMALARI
extern void lcd_clear(void);
extern void lcd_puts(const char *);
extern void lcd_putch(char c);
extern void lcd_goto(unsigned char, unsigned char);
extern void lcd_init(void);
 #endif //_LCD_4BIT_H_

  void lcd_write(unsigned char data)
{
   __delay_cycles(40);
   LCD_DATA = ( ( data >> 4 ) & 0x0F );
   LCD_STROBE();
   LCD_DATA = ( data & 0x0F );
   LCD_STROBE();
}
 void lcd_clear(void)    {
  LCD_RS = 0;
  lcd_write(0x01);
  __delay_cycles(2000);
  }

void lcd_puts(const char * x)
{
   LCD_RS = 1;
   while(*x)
   lcd_write(*x++);
}

void lcd_putch(char c)
{
   LCD_RS = 1;
   lcd_write(c);
}

void lcd_goto (unsigned char row, unsigned char column)
{
   LCD_RS = 0;
   if(row==1)
      lcd_write(0x80+((column-1)%16));
   else if (row==2)
      lcd_write(0xC0+((column-1)%16));
}

void lcd_init() // LCD BASLANGIC AYARLARI
{
   LCD_RS = 0;
   LCD_EN = 0;
    __delay_cycles(15000);
   LCD_DATA = 0x03;
   LCD_STROBE();
   __delay_cycles(5000);
   LCD_STROBE();
   __delay_cycles(200);
   LCD_STROBE();
   __delay_cycles(200);
   LCD_DATA = 2;
   LCD_STROBE();

   lcd_write(0x28);
   lcd_write(0x0C);
   lcd_clear();
   lcd_write(0x06);
}
void LCD_Write_Int (unsigned int val,char mul)
{
  char i;
  int temp=1;

  if(val>99) ////// % ISARETINDEN SONRA 100 DEN KUCUK DEGERLERDE 0 YAZMASINI ÖNLER
  {
  unsigned char str[4];
  for(i=0;i<mul;i++)
    {str[i] = ((val/temp)%10+48); temp*=10;}
  for(i=0;i<mul;i++)
    lcd_putch(str[mul-1-i]);
  }
   ///////////////////77
     else if(10<=val)
  {
  unsigned char str[4];
  for(i=0;i<mul-1;i++)
    {str[i] = ((val/temp)%10+48); temp*=10;}
  for(i=0;i<mul-1;i++)
    lcd_putch(str[mul-2-i]);
  }

else if (val<10)
  {
  unsigned char str[4];
  for(i=0;i<mul-2;i++)
    {str[i] = ((val/temp)%10+48); temp*=10;}
  for(i=0;i<mul-2;i++)
    lcd_putch(str[mul-3-i]);
  }

}

unsigned char mesafe , mesafe2 ;
void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer

     __delay_cycles(100000); // BEKLEME SÜRESI
     TA0CTL=0x00;
     TA0CTL=TASSEL_2 +MC_0 + ID_0;

   P1SEL = 0x00;
   P1OUT = 0x00;
   P1DIR = 0xFF;
   P2SEL = 0x00;
   P2OUT = 0x00;
   P2DIR = 0xd9;                //   P2’s  5.1.2.bit is echo           1101 1001

   lcd_init();
   lcd_goto(1,1); // LCD BASLANGIC KONUMLANMASI 1. SATIR 1. SUTUN
   lcd_puts(" UGUR");     //  LCD ÇALISMASINI KONTROL
   lcd_goto(2,1);// LCD KONUMLANMASI 2. SATIR 1. SUTUN
    lcd_puts("190207033");              // LCD KONTROL
    __delay_cycles(1000000);

   TA0CTL=0x00;  // TIMER SIFIRLAMASI
   TA0CTL=TASSEL_2+MC_0+ID_0;

   while(1)  // SONSUZ DÖNGU
   {
   /////////////// /*//////////////
  TRIG = 1 ; // TRIGER CIKISINI HIGH YAPTIK
 __delay_cycles(10);
   TRIG = 0;   // TRIGER CIKISINI LOW YAPTIK
  while(!(ECH)); // ECHO DAN DÖNUS OLANA KADAR BEKLEYECEK
   TA0CTL |=MC_2;  // SAYICI BASLATILIYOR
  while(ECH);  // BILGI GELDIGINDE
   TA0CTL ^=MC_2;//
    time=TA0R; // SAYICIDAKI DEGER SURE ICERISINE ALINIYOR

    TA0R=0; // SAYICI SIFIRLANIYOR
     __delay_cycles(100);
     mesafe2=time/58;  time=0;// UZAKLIK 1 DEGISKENINE SURE DEGISKENI
     //ICERISINDEKI DEGERIN 58' E BÖLÜMÜ ALINIYOR VE SURE SIFIRLANIYOR
     // 58' BÖLÜNMESININ NEDENI SES SINYALI GÖNDERILDIGINDEN
     //SESIN SANIYEDEKI SURATI CM YE CEVRILIYOR
     if(mesafe2>50)  // MAKSIMUM 50 CM YE KADAR ÖLCÜM YAPMA AYARI
       mesafe2=0;





 if (mesafe2>0) //  sensor deger aldiysa
 {
     mesafe=mesafe2; // MESAFE2 DEGISKENI MESAFE DEGISKENINE AKTARIYORUZ
}
 else
 {
mesafe2=0;
}
    if(mesafe>50)
      mesafe=100;
   else
      mesafe=mesafe*2; // % YE ÇEVIRMEK MAKSADIYLA 2 ILE ÇARPILIYOR
     mesafe=100-mesafe;    // 100 DEN ÇIKARMAKTAKI AMAC DOLULUK UZUNLUGUNU BULMAK
     // BURADA BUNU YAPMAZSAK COPUN BOS KISMINI GÖSTERIR

   lcd_init();
   lcd_goto(1,1);
   lcd_puts("DOLULUK SEVIYESI");

   lcd_goto(2,2);
   lcd_puts("%");

   __delay_cycles(10);
   if(100>mesafe&&mesafe!=0)
  {
   lcd_goto(2,2);
   lcd_puts("%");  // LCD 2. SATIR 2. SUTUNA % ISARETINI YAZAR

    lcd_goto(2,4);
   lcd_puts("            ");  // LCD IKINCI SATIRIN 4. SUTUNUNDAN ITIBAREN BOSLUK YAZAR

    lcd_goto(2,4);
    LCD_Write_Int(mesafe,3);  // LCD IKINCI SATIR 4. SUTUNUNDAN ITIBAREN YUKARIDA HESAPLANAN
    //UZAKLIK DEGERINI YAZAR

  }
   else
   {
     lcd_goto(2,1); // EGER UZAKLIK DEGERI 100 DEN BÜYÜK DEGILSE VE 0 ISE
     lcd_puts("COP KUTUSU BOS");
     __delay_cycles(10);
   }
   __delay_cycles(1000000); // BEKLEME SÜRESI
   }// SONSUZ DÖNGÜ SONU

}
