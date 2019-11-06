#!/bin/bash
#g++  -c LoRa.cpp -o LoRa.o -lpigpio -lrt -pthread -lm
g++ pongtest.cpp DataBase.cpp epolls.cpp  -o pongtest LoRa.o -lpigpio -lrt -pthread -lm
