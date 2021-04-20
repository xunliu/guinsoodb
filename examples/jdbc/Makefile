.PHONY: guinsoodb clean main

all: guinsoodb main

clean:
	rm -f *.class
	rm -rf target

guinsoodb:
	BUILD_JDBC=1 make -C ../..

main:
	javac GuinsooDBExample.java
	java -cp .:../../build/release/tools/jdbc/guinsoodb_jdbc.jar GuinsooDBExample

maven:
	mvn compile exec:java -Dexec.mainClass="GuinsooDBExample"