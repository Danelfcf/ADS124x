#include <ADS124X.h>

ADS124X ad1;  // create ADS instance called ad1
int data;

void setup()
{
  Serial.begin(9600);

  //void PinMap(int cs, int drdy, int reset, int start) set operation pins if no start/reset pin write 99
  ad1.PinMap(10, 9, 8, 7);
    
  // start spi and prepare pins
  ad1.Initialize();

  void ResetHW(); // Hardware reset
  // ad1.ResetSW(); // software reset

}

void loop() {
  ad1.Setup((VREFCONON | REFSELT0 | MUXCALNOR), (PGA8 | DR5), (AINP4 | AINN5), (IDAC1K), (DISCONEC | DISCONEC2));
  ad1.Start(); // read data
  data = ad1.GetData();
  Serial.print("DEC: ");
  Serial.print(data, DEC);
  Serial.print("HEX: ");
  Serial.print(data, HEX);
  Serial.print("");

  ad1.Setup((VREFCONON | REFSELT0 | MUXCALNOR), (PGA8 | DR5), (AINP6 | AINN7), (IDAC1K), (DISCONEC | DISCONEC2));
  ad1.Start(); // read data
  data = ad1.GetData();
  Serial.print("DEC: ");
  Serial.print(data, DEC);
  Serial.print("HEX: ");
  Serial.print(data, HEX);
  Serial.print("");

  delay(200);

}
