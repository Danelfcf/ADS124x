/*  ----- KAKI 2xPt100 ADS1248 -----
 * Utima verificacao (Last check): 
 * 05/06/2020
 * 
 * Pt-BR
 * Exemplo de aplicacao do ADS1248
 * lendo dois Pt100 a quatro fios,
 * construido com base no nota de 
 * aplicação SLAU520A da Textas.
 * 
 * English
 * ADS1248 application example, 
 * reading two Pt100 with four-wire, 
 * based on SLAU520A Texas 
 * application note.
 * 
 * SLAU520A:
 * http://www.ti.com/lit/ug/slau520a/slau520a.pdf?ts=1588773614510
 */

#include <math.h>
#include <Wire.h>
#include <ADS124X.h>


// valores para correcao    A         B
double	CorrecaoA45 [2] = {0.07981, 1.00164};	 // AIn 45   -- Placa_1
double	CorrecaoA67 [2] = {0.08164, 1.00159};	 // AIn 67	 -- Placa_1
double  OHM2TEMPA45 [2] = {-1.03776, 1.00961}; // AIn 45   -- Placa_1
double  OHM2TEMPA67 [2] = {-1.26977, 1.01156}; // AIn 67   -- Placa_1

// Constantes PT100
double A = 3.9083 * pow(10, -3);
double B = -5.775 * pow(10, -7);
double C=(-4.183) * pow(10,-12);
double R0 = 100;

ADS124X ad1;  // create ADS instance called ad1

void setup(){
	Serial.begin(9600);

  //ADS
  ad1.PinMap(10, 9, 8, 7);
  ad1.Initialize();
  ad1.ResetSW();
}

void loop()
{
  //ADS
  ad1.Initialize(); // start spi for ADS
	// read 4 and 5
	ad1.Setup((VREFCONON | REFSELT0 | MUXCALNOR), (PGA8 | DR5), (AINP4 | AINN5), (IDAC1K), ( IEXC11| DISCONEC2));
	// read data
	ad1.Start();
	double AIn45 = ((double)ad1.GetData()* (double)2000.13763)/(8*0x7fffff);
	AIn45 = Correcao(CorrecaoA45, AIn45);
 
  //  Converter para Temperatura
  AIn45 = Correcao(OHM2TEMPA45, AIn45);
  AIn45 = Temperatura(AIn45);
  
	ad1.Setup((VREFCONON | REFSELT0 | MUXCALNOR), (PGA8 | DR5), (AINP6 | AINN7), (IDAC1K), (DISCONEC | IEXC22));
	// read data
	ad1.Start();
	double AIn67 = ((double)ad1.GetData()* (double)2000.13763)/(8*0x7fffff);
	AIn67 = Correcao(CorrecaoA67, AIn67);
 
  //  Converter para Temperatura
  AIn67 = Correcao(OHM2TEMPA67, AIn67);
  AIn67 = Temperatura(AIn67);

  Serial.print(AIn45);
  Serial.print('\t');
  Serial.println(AIn67);

  ad1.EndCOM(); //leave the SPI bus free for others devices
  
}

double Correcao(double x[], double AdIn){
	double A = x[0];
	double B = x[1];
	return double(A + B*AdIn);
}

double Temperatura(double R){
  long double a,b,c,delta;
  a = B*R0;
  b = (R0*A);
  c = (R0-R);
  delta=(b*b)-(4*c*a);
  return double((-b + sqrt(delta))/(2*a));
}
