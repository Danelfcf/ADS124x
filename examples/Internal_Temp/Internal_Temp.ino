#include<ADS124X.h>

ADS124X ad1;  // create ADS instance called ad1

void setup() {
  Serial.begin(9600);

  //void PinMap(int cs, int drdy, int reset, int start) set operation pins if no start/reset pin write 99
  ad1.PinMap(10, 9, 8, 7);
    
  // start spi and prepare pins
  ad1.Initialize();

  void ResetHW(); // Hardware reset
  // ad1.ResetSW(); // software reset
    
  ad1.SetupInternalTemp(); // Set ads124x to get ADS's internal temperature
  ad1.EndCOM();
}

void loop() {
  ad1.Initialize(); // start spi for ADS
  Serial.print("internal Temp. : ");
  // double inTemp(int scale) 0 = F (not implemented); 1 = C; 2 = K(not implemented will return in C)
  Serial.println(ad1.inTemp(1));
  ad1.EndCOM(); //leave the SPI bus free for others devices
  delay(100);
}
