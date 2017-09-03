#include <Boards.h>
 
#include "EmonLib.h"
#include <SPI.h>
 
EnergyMonitor emon1;
EnergyMonitor emon2;
 
int rede = 127;
 
int pino_sct1 = A1;
int pino_sct2 = A2;

int pino_svt1 = A3;
int pino_svt2 = A4;

double Irms1;
double Irms2;

void setup()
{
  int i;
  Serial.begin(115200);
  //Pino, calibracao - Cur Const= Ratio/BurdenR. 2000/33 = 60
  emon1.current(pino_sct1, 54.82);
  emon1.voltage(pino_svt1, 116, 1.7);
  
  emon2.current(pino_sct2, 52.8);
  emon2.voltage(pino_svt2, 118, 1.7);
  pinMode(LED_BUILTIN, OUTPUT);

}

int i=0;
int stabilization = 10;
char byteRead;

void loop()
{
  if(i)
  {
    i = 0;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    i = 1;
    digitalWrite(LED_BUILTIN, LOW);
  }
   // Channel 1
//  Irms1 = emon1.calcIrms(1480);
  emon1.calcVI(20,2000);

   // Channel 2
//  Irms2 = emon2.calcIrms(1480);
  emon2.calcVI(20,2000);

   if(stabilization == 0)
   {
      Serial.write("@1 ");
      emon1.serialprint();           // Print out all variables
      Serial.write("@2 ");
      emon2.serialprint();           // Print out all variables
      delay(1000);
   }
   else
   {
      stabilization--;
   }

   // Reply know string to allow device detection by python sensor class  
   while (Serial.available()) {
    byteRead = Serial.read();
      if (byteRead == '#')
      {
       /*ECHO the value that was read, back to the serial port. */
       Serial.write('!');
      }
     }

 
}
