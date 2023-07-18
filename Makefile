make:
	pio run

upload: make
	picotool load .pio/build/picow/firmware.uf2 -f

serial:
	printf "\nQuit with <C-a><C-X>\n\n"
	picocom -b 115200 -l /dev/ttyACM0 -q

uploadserial: upload
	sleep 1.5
	make serial

compiledb:
	pio run -t compiledb

