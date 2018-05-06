// ADS.h
#ifndef ADS124X_H
#define ADS124X_H

#include <Arduino.h>
#include <stdarg.h>

#pragma region Commands

#pragma region ADS 1247, 1248 commands

#define NOP			 0xff
#define NOR			 0x00
#define WAKEUP		 0X00
#define SLEEP	     0X02
#define SYNC		 0X04
#define REST	     0X06    /// double
#define RDATAC		 0X14
#define WRITE	     0X40
#define READ	     0X20
#define RDATA		 0X12
#define RESET		 0x06    /// double
#define STOPDATACONT 0x16
#define SYSOCAL		 0X60
#define SYSGCAL		 0X61
#define SELFOCAL	 0x62

#pragma endregion ADS 1247, 1248 commands

#pragma region  ADS 1247 and ADS1248 Register Map

#define MUX0    0x00
#define VBIAS   0x01
#define MUX1    0x02
#define SYS0    0x03
#define OFC0    0x04
#define OFC1    0x05
#define OFC2    0x06
#define FSC0    0x07
#define FSC1    0x08
#define FSC2    0x09
#define IDAC0   0x0A
#define IDAC1   0x0B
#define GPIOCFG 0x0C
#define GPIODIR 0x0D
#define GPIODAT 0x0E

#pragma endregion  ADS 1247 and ADS1248 Register Map

#pragma region  Multiplexer Control Register 0 Register - MUX0
// Burn-out Detect Current Source Registe
#define BOOFF   0B00000000
#define BO5U    0B01000000
#define BO2U    0B10000000
#define BO10u   0B11000000
// Multiplexer Selection - ADC Positive Input
#define AINP0   0B00000000
#define AINP1   0B00001000
#define AINP2   0B00010000
#define AINP3   0B00011000
#define AINP4   0B00100000   // ADS1248
#define AINP5   0B00101000   // ADS1248
#define AINP6   0B00110000   // ADS1248
#define AINP7   0B00111000   // ADS1248
// Multiplexer Selection - ADC Negative Input
#define AINN0   0B00000000
#define AINN1   0B00000001
#define AINN2   0B00000010
#define AINN3   0B00000011
#define AINN4   0B00000100   // ADS1248
#define AINN5   0B00000101   // ADS1248
#define AINN6   0B00000110   // ADS1248
#define AINN7   0B00000111   // ADS1248
#pragma endregion MUX0

#pragma region Bias Voltage Register

#define VBIASA3DIS  0B00000000
#define VBIASA3EN   0B00001000
#define VBIASA2DIS  0B00000000
#define VBIASA2EN   0B00000100
#define VBIASA1DIS  0B00000000
#define VBIASA1EN   0B00000010
#define VBIASA0DIS  0B00000000
#define VBIASA0EN   0B00000001

#pragma endregion Bias Voltage Register

#pragma region MULTIPLEX CONTROL REGISTER 1 - MUX1

#define CLKSTATINT    0B00000000  // CLOCK STATUS
#define CLKSTATEXT    0B10000000
#define VREFCONOFF    0B00000000  // INTERNAL REFERENCE CONTROL
#define VREFCONON     0B00100000
#define VREFCONCON    0B01000000
#define REFSELT0      0B00000000  // REFERENCE SELECT CONTROL
#define REFSELT1      0B00001000
#define REFSELTINT    0B00010000
#define REFSELTINT0   0B00011000
#define MUXCALNOR     0B00000000  // SYSTEM MONITOR CONTROL
#define MUXCALOFFSET  0B00000001
#define MUXCALGAIN    0B00000010
#define MUXCALTEMP    0B00000011
#define MUXCALREF1    0B00000100
#define MUXCALREF0    0B00000101
#define MUXCALANAL    0B00000110
#define MUXCALDIGI    0B00000111

#pragma endregion MULTIPLEX CONTROL REGISTER 1 - MUX1

#pragma region SYSTEM CONTROL REGISTER 0 - SYS0

#define PGA1    0B00000000  // GAIN SETTING FOR PGA
#define PGA2    0B00010000
#define PGA4    0B00100000
#define PGA8    0B00110000
#define PGA16   0B01000000
#define PGA32   0B01010000
#define PGA64   0B01100000
#define PGA128  0B01110000
#define DR5     0B00000000  // DATA OUTPUT RATE SETTING
#define DR10    0B00000001
#define DR20    0B00000010
#define DR40    0B00000011
#define DR80    0B00000100
#define DR160   0B00000101
#define DR320   0B00000110
#define DR640   0B00000111
#define DR1K    0B00001000
#define DR2K    0B00001001

#pragma endregion SYSTEM CONTROL REGISTER 0

#pragma region IDAC CONTROL REGISTER 0 - IDAC0

#define DRDYONLY  0B00000000  // DATA READY MODE SETTING
#define DRDYREADY 0B00001000
#define IDACOFF   0B00000000  // IDAC EXCITATION CURRENT MAGNITUDE
#define IDAC50    0B00000001
#define IDAC100   0B00000010
#define IDAC250   0B00000011
#define IDAC500   0B00000100
#define IDAC750   0B00000101
#define IDAC1K    0B00000110
#define IDAC1K5   0B00000111

#pragma endregion IDAC CONTROL REGISTER 0 - IDAC0

#pragma region IDAC CONTROL REGISTER 1 - IDAC1

#define I1DIR0    0B00000000  // IDAC EXCITATION CURRENT OUTPUT 1
#define I1DIR1    0B00010000
#define I1DIR2    0B00100000
#define I1DIR3    0B00110000
#define I1DIR4    0B01000000
#define I1DIR5    0B01010000
#define I1DIR6    0B01100000
#define I1DIR7    0B01110000
#define IEXC11    0B10000000
#define IEXC12    0B10010000
#define DISCONEC  0B11000000
#define I2DIR0    0B00000000  // IDAC EXCITATION CURRENT OUTPUT 2
#define I2DIR1    0B00000001
#define I2DIR2    0B00000010
#define I2DIR3    0B00000011
#define I2DIR4    0B00000100
#define I2DIR5    0B00000101
#define I2DIR6    0B00000110
#define I2DIR7    0B00000111
#define IEXC21    0B00001000
#define IEXC22    0B00001001
#define DISCONEC2  0B00001100

#pragma endregion IDAC CONTROL REGISTER 1 - IDAC1

#pragma endregion Commands


class ADS124X
{
public:
	void PinMap(int cs, int drdy, int reset, int start); // set operation pins if no start/reset pin write 99
	void Initialize(); // start spi and prepare pins
	void EndCOM();
	void Setup(	byte mux1,	// set up basic operation
				byte sys0,
				byte mux0,
				byte idac0,
				byte idac1);
	void ResetHW();	   // if there is a reset pin use it in operation
	void ResetSW();	   // software reset
	void Start();	   // if there is a start pin use it in operation
	int Command(int settings, ...); //  number of settings Variadic function ie. use as many args as needed
	
	int32_t GetData(); //  number of bytes.
	int32_t GetReg(int bytes, byte reg);  //  number of bytes, register to be read (eg: RDATA).
	int32_t OffSetReg(); // gets the offset register value
	int32_t FullScaleReg(); // gets the full-scale calibration value from register
	
	void SetReg(int reg, byte val);  //register and bytes to write
	int Gain();						 // returns value of gain
	void SysOffSetCal();			 // Orders system offset Calibration.
	void SysGainCal();				 // 
	void SelfOCal();
	
	
	// useful functions
	double pt100_conv(int32_t x,  double correction, double Rref, int gain);	// transform raw data to pt100, correction offset ie 0.79586, set gain to 0 if autodetected is to be used
	void SetupInternalTemp();
	double inTemp(int scale); // 0 = F (not implemented); 1 = C; 2 = K(not implemented will return in C)


private:	
	// for pic portability
	void pinc(int pin, int lvl); // digitalwrite()
	void pint(int pin, int type);// pinmode - 0 out, 1 in
	void wait(int time);		 // delay()
	void setSPI();
	void spiWrite(byte x);
	byte spiRead(byte x);
	void spiEnd();	
	bool pinState(byte pin);
};


#endif
