rm -f src/guinsoodb.cpp src/guinsoodb.h src/guinsoodb.o src/guinsoodbr.o src/guinsoodb.so src/Makevars
rm -rf src/guinsoodb
R -q -e "if (is.element('guinsoodb', installed.packages()[,1])) { remove.packages('guinsoodb') }"
