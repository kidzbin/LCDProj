# LCD compile test

all:
	gcc lcdtest.c DHT11/DHT11.c PCD8544/PCD8544.c -o LCDrun -L/usr/local/lib -lwiringPi

debug:
	gcc lcdtest.c DHT11/DHT11.c PCD8544/PCD8544.c -W -g -o LCDrun -L/usr/local/lib -lwiringPi

clean:
	rm -f LCDrun
