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

String json_measures = String(100);

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

char byteRead;

void loop()
{
	// Channel 1
	//  Irms1 = emon1.calcIrms(1480);
	digitalWrite(LED_BUILTIN, HIGH);
	emon1.calcVI(20,2000);
	digitalWrite(LED_BUILTIN, LOW);

	// Channel 2
	//  Irms2 = emon2.calcIrms(1480);
	digitalWrite(LED_BUILTIN, HIGH);
	emon2.calcVI(20,2000);
	digitalWrite(LED_BUILTIN, LOW);

	// Read serial port and parse the received command  
	while (Serial.available())
	{
		byteRead = Serial.read();
		
		if (byteRead == '#')
		{
			/*ECHO the value that was read, back to the serial port. */
			Serial.write('!');
		}
		
		if (byteRead == '?')
		{
			// ugly json encoder
			// expected output: 
			// {"ch1":{"real_power":42.40,"apparent_power":69.70,"Vrms":128.63,"Irms":0.54,"power_factor":0.61},"ch2":{"real_power":0.20,"apparent_power":0.39,"Vrms":0.71,"Irms":0.55,"power_factor":0.52}}

			Serial.print( "{\"ch1\":{\"real_power\":" );
			Serial.print( emon1.realPower );
			Serial.print( ",\"apparent_power\":" );
			Serial.print( emon1.apparentPower );
			Serial.print( ",\"Vrms\":" );
			Serial.print( emon1.Vrms );
			Serial.print( ",\"Irms\":" );
			Serial.print( emon1.Irms );
			Serial.print( ",\"power_factor\":" );
			Serial.print( emon1.powerFactor );
			Serial.print("},");
			Serial.print( "\"ch2\":{\"real_power\":" );
			Serial.print( emon2.realPower );
			Serial.print( ",\"apparent_power\":" );
			Serial.print( emon2.apparentPower );
			Serial.print( ",\"Vrms\":" );
			Serial.print( emon2.Vrms );
			Serial.print( ",\"Irms\":" );
			Serial.print( emon2.Irms );
			Serial.print( ",\"power_factor\":" );
			Serial.print( emon2.powerFactor );
			Serial.print("}}\n\r");
		}
	}
	delay(1000);
}
