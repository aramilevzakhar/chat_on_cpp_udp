CC = g++
 

.PHONY: default compile build run
default: compile run
compile: 
	$(CC) server.cpp -o binServer
	$(CC) client.cpp -o binClient
#run main:


