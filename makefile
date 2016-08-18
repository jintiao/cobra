CC=g++

cobra: cobra.cpp
	$(CC) -o cobra cobra.cpp -std=c++11 -O2 -Wall
