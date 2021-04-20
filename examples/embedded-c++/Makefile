

.PHONY: guinsoodb clean main

all: guinsoodb main

clean:
	rm -rf build

guinsoodb:
	cd ../.. && make

main:
	mkdir -p build
	cd build && cmake .. && make
	build/example


