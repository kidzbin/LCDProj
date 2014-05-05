# LCD compile test

all:
	gcc lcdtest.c DHT11/DHT11.c MyPCD8544/MyPCD8544.c -o LCDrun -lwiringPi
	gcc DetectT.c DHT11/DHT11.c -o DetectT -lwiringPi
debug:
	gcc lcdtest.c DHT11/DHT11.c PCD8544/PCD8544.c -W -g -o LCDrun -L/usr/local/lib -lwiringPi

clean:
	rm -f LCDrun
