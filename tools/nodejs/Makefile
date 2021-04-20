all: build

src/guinsoodb.cpp:
	./configure

./node_modules:
	npm install --build-from-source

build: ./node_modules src/guinsoodb.cpp
	./node_modules/.bin/node-pre-gyp build --loglevel=silent

debug: ./node_modules src/guinsoodb.cpp
	./node_modules/.bin/node-pre-gyp build --debug --verbose

clean:
	@rm -rf ./build
	rm -rf lib/binding/
	rm -f test/support/big.db-journal
	rm -rf ./node_modules/
	rm -rf src/guinsoodb.*

test:
	npm test

check: test

.PHONY: test clean build
