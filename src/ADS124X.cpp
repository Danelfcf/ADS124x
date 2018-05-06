#include <SPI.h>
#include <Arduino.h>
#include <stdarg.h>
#include <ADS124X.h>
// ADS1247 pinout
int CS, DRDY, RESET_, START;

// ADS1247 Variables
const int ADResetPulse = 1;          // ADC reset (min. 4x244 = 976 ns, low), 244 ns = 1/4,096 MHz;
const int ADResetDelay = 1;          // ADC delay after reset in ms min 0.6 ms
int mux1settings;
int a =1;

// Save user temperary variables
byte _mux0;
byte _mux1;
byte _sys0;
byte _idac0;
byte _idac1;
byte _gain;


#pragma region Basic functions

void ADS124X::PinMap(int cs, int drdy, int reset, int start)
{
	//CS, DRDY, RESET, START = cs, drdy, reset, start;
	CS = cs;
	DRDY=drdy;
	RESET_=reset;
	START = start;
}

void ADS124X::Initialize()
{
	// Arduino ADC interface (wire accordingly)
	// inverse means that when pin is low relavent function is active e.g.:
	// DRDY low means ADC has data to send
	pint(CS, 0);
	pint(DRDY, 1);
	pinc(CS, 1);
	if (START != 99)	// if start pin is used
	{
		pint(START, 0);	// PIN TYPE OUTPUT
		pinc(START, 1); // PIN LOGIC STATE 1
	}
	if (RESET != 99)	// if reset pin is used
	{
		pint(RESET, 0); // PIN TYPE OUTPUT
		pinc(RESET, 1); // PIN LOGIC STATE 1
	}
	
	setSPI();
}

void ADS124X::EndCOM()
{
	spiEnd();
}

void ADS124X::Setup(byte mux1, byte sys0, byte mux0, byte idac0, byte idac1) // set up basic operation
{
	_mux0 = mux0; _mux1 = mux1; _sys0 = sys0; _idac0 = idac0; _idac1 = idac1; 
	_gain = sys0>>4;
	
	pinc(CS, 0);
	wait(1);
	spiWrite(RESET);
	wait(ADResetDelay);
	spiWrite(STOPDATACONT);
	wait(210);
	
	spiWrite(WRITE | MUX1);	// mux1      ( write command )
	spiWrite(MUX1 ^ SYS0);	// sys0      ( write command )
	spiWrite(mux1);			// user input
	spiWrite(sys0);			// user input
	
	spiWrite(WRITE | MUX0);	// mux0      ( write command )
	spiWrite(NOR);			// no register
	spiWrite(mux0);			// user input
	
	spiWrite(WRITE | IDAC0);
	spiWrite(NOR);			// no register
	spiWrite(idac0);		// user input
	
	spiWrite(WRITE | IDAC1);
	spiWrite(NOR);			// no register
	spiWrite(idac1);		// user input
	
	wait(1);
	pinc(CS, 1);
}

void ADS124X::ResetHW()
{
	if (RESET_ != 99)
	{
		pinc(RESET_, 0);
		wait(ADResetPulse);
		pinc(RESET_, 1);
		wait(ADResetDelay);
	}
}

void ADS124X::ResetSW()
{
	pinc(CS, 0);
	spiWrite(RESET);
	pinc(CS, 1);
}

void ADS124X::Start()
{
	if (START != 99)
	{
		pinc(START, 0);
		wait(10);
		pinc(START, 1);
	}

}

int ADS124X::Command(int settings, ...)
{
	va_list ap;
	int x;
	int comm = 0;
	va_start(ap, settings); //Requires the last fixed parameter (to get the address)
	for (x=0; x<settings; x++)
	{
		comm = comm | va_arg(ap, int); //Requires the type to cast to.
	}
	va_end(ap);
	return comm;
}

int32_t ADS124X::GetData()
{   
	while(pinState(DRDY) == 1){;} // wait for data to be ready
	pinc(CS, 0);
	wait(1);
	spiWrite(RDATA);
	int32_t data;
	for (int i=0; i<3; i++)
	{
		data |= spiRead(NOP);
		if (i == 0 | i == 1)
		{ data <<= 8;}
	}
	pinc(CS, 1);
	return (int32_t)data;
	
}

int32_t ADS124X::GetReg(int bytes, byte reg)
{
	pinc(CS, 0);
	wait(1);
	spiWrite(READ | reg);
	spiWrite(NOR);
	int32_t data;
	for (int i=0; i<bytes; i++)
	{
		data = spiRead(NOP);
	}
	pinc(CS, 1);
	return data;
}

void ADS124X::SetReg(int reg, byte val)
{
	if (reg == SYS0)
	{
		_gain= val<<4;
	}
	pinc(CS, 0);
	wait(1);
	
	spiWrite(WRITE | reg);
	spiWrite(NOR);
	spiWrite(val);
	
	wait(1);
	pinc(CS, 1);
}

// check mux 1 datasheet
void ADS124X::SysOffSetCal()
{
	pinc(CS, 0);
	//            flex        flex
	SetReg(MUX1, VREFCONON | REFSELT0 | REFSELTINT | MUXCALOFFSET);
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	SetReg(IDAC1, DISCONEC | DISCONEC);
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	wait(1);
	spiWrite(SYSOCAL);
	wait(10);
	//SetReg(MUX1, _mux1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	SetReg(MUX1, _mux1);  // change back to usr's settings
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	SetReg(IDAC1, _idac1);  // change back to usr's settings
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
}

// check mux 1 datasheet    // make more flexable for user
void ADS124X::SysGainCal()
{
	pinc(CS, 0);
	//
	SetReg(MUX1, VREFCONON | REFSELT0 | MUXCALGAIN);
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	spiWrite(SYSGCAL);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	SetReg(MUX1, _mux1);  // change back to usr's settings
	wait(1);
	pinc(CS, 1);
	
	while(pinState(DRDY) == 1){;}
}

// check mux 1 datasheet
void ADS124X::SelfOCal()
{
	pinc(CS, 0);
	SetReg(MUX1, VREFCONON | REFSELT0 | MUXCALOFFSET);
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	spiWrite(SELFOCAL);
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
	
	pinc(CS, 0);
	SetReg(MUX1, _mux1);  // change back to usr's settings
	wait(1);
	pinc(CS, 1);
	while(pinState(DRDY) == 1){;}
		
}

#pragma endregion Basic functions

#pragma region Useful functions

double ADS124X::pt100_conv(int32_t x,  double correction, double Rref, int gain)
{
	if (gain=0)
	{
		int gain = _gain;
	}
	
	double data = x;
	data =((data * Rref)/(8388607 * gain)) + correction;
	
	long double a,b,c,delta, A=(3.9083/1000), B=(-5.775/10000000), C=(-4.183/1000000000000);
	int R0=100;
	a = B*R0;
	b = (R0*A);
	c = (R0-data);
	delta=(b*b)-(4*c*a);
	
	return double((-b + sqrt(delta))/(2*a));
}

void ADS124X::SetupInternalTemp()
{
	Setup(0x33, 0x00, 0x00,0x00,0x00);
}

double ADS124X::inTemp(int scale)
{
	double data = GetData();
	double mv = (data*2048)/8388607;
	if ( scale == 1 )
	{
		return ((mv - 118)/0.405) + 25;
	}
	
	if (scale == 2)
	{
		return ((mv - 118)/0.405) + 25 + 273.15;
	}
}

int32_t ADS124X::OffSetReg()
{
	int32_t data;
	data |= GetReg(1, 0x06);
	data <<= 8;
	data |= GetReg(1, 0x05);
	data <<= 8;
	data |= GetReg(1, 0x04);
	
	return data;
}
//   5626610  / 4194304
int32_t ADS124X::FullScaleReg()
{
	int32_t data;
	data |= GetReg(1, 0x09);
	data <<= 8;
	data |= GetReg(1, 0x08);
	data <<= 8;
	data |= GetReg(1, 0x07);
	
	return data;
}

int ADS124X::Gain()
{
	return (_gain);
}

#pragma endregion Useful functions

#pragma region Private functions

void ADS124X::pinc(int pin, int lvl)
{
	if (lvl == 1)
	{
		digitalWrite(pin, HIGH);
	}
	if (lvl == 0)
	{
		digitalWrite(pin, LOW);
	}
}

void ADS124X::pint(int pin, int type)
{
	if (type == 1)
	{
		pinMode(pin, INPUT);
	}
	if (type == 0)
	{
		pinMode(pin, OUTPUT);
	}
}

void ADS124X::wait(int time)
{
	delay(time);
}

void ADS124X::setSPI()
{
	// Arduino SPI settings (change first line according to IC specifications)
	// defines SPI communication type and initializes module
	SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE1)); // SPI setup SPI communication
	wait(200);          // SPI delay for initialization
	SPI.begin();         // SPI Module 'wake up'
}

void ADS124X::spiWrite(byte x)
{
	SPI.transfer(x);
}

byte ADS124X::spiRead(byte x)
{
	return SPI.transfer(x);
}

void ADS124X::spiEnd()
{
	SPI.end();
	SPI.endTransaction();
}

bool ADS124X::pinState(byte pin)
{
	if (digitalRead( pin ) == true) { return true; }
	if (digitalRead( pin ) == false){ return false;	}
}
#pragma endregion Private functions

