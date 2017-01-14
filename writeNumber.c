#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <wiringShift.h>

#define NANO_SECOND_MULTIPLIER 1000000

const long INTERVAL_MS = 10 * NANO_SECOND_MULTIPLIER;

struct timespec sleepValue;

int input = 0;
int clockk = 2;
int latch = 1;

int value = 0;
int high = 1;
int low = 0;

uint8_t zahlen[10] = {0xE7,0x84,0xD3,0xD6,0xB4,0x76,0x77,0xC4,0xF7,0xF6};
uint8_t zwei = 0xD3;

void setup() {
	wiringPiSetup();
	int i = 0;
	for(i = 0; i < 3; i++) {
		pinMode(i,OUTPUT);
	}
}

void getBits(uint8_t number,uint8_t* bits)
{
	int i = 7;
	for (i=7;i>=0;i--) {
  		bits[i]=number&1;
  		number>>=1;
	}
}

void done() {
	digitalWrite(input,low);
	digitalWrite(clockk,low);
	digitalWrite(latch,low);
}

void writeReg(int value) {
	sleepValue.tv_nsec = INTERVAL_MS;
	digitalWrite(latch,0); //Latch low
	digitalWrite(clockk,0);//Clock low
	nanosleep(&sleepValue,NULL);
	digitalWrite(input,value); //Input
	nanosleep(&sleepValue,NULL);
	digitalWrite(clockk,high);//Clock
	nanosleep(&sleepValue,NULL);
	digitalWrite(latch,high); //Apply saved values to outputs
}


int main(int argc, char*argv[])
{

	printf("Bananapi Shift register Test\n");
	//printf("%b\n",n_0);
	setup();
	int i = 0;
	//shiftOut(input,clockk,MSBFIRST,n_0);
	int argin = atoi(argv[1]);
	uint8_t test[8];
	printf("%d\n",argin);
	int number = argin;
	if(number < 10)
	{
		shiftOut(input,clockk,MSBFIRST,zahlen[number]);
		shiftOut(input,clockk,MSBFIRST,zahlen[0]);
	} else
	{
		while(number)
		{
			shiftOut(input,clockk,MSBFIRST,zahlen[number % 10]);
			number /= 10;
		}
	}
	nanosleep(&sleepValue,NULL);
        digitalWrite(latch,high); //Apply saved values to outputs
	done();
}


