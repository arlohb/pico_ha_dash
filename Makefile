compiledb:
	pio run -t compiledb

upload:
	pio run -t upload

serial:
	pio device monitor -b 115200

uploadserial:
	make upload && make serial

