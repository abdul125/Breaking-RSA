breakrsa: main.cpp
	g++ -Wall -O3 -std=c++11 -lgmpxx -lgmp -o breakrsa main.cpp

.PHONY: run
run: breakrsa
	./breakrsa cipher-1141.txt

.PHONY: all
all: breakrsa
