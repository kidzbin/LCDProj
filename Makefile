# LCD compile test

all:
	gcc lcdtest.c -o LCDrun -L/usr/local/lib -lwiringPi

debug:
	gcc lcdtest.c -W -g -o LCDrun -L/usr/local/lib -lwiringPi

clean:
	rm -f LCDrun
