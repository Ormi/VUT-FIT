#
# @file Makefile
#
# Druhy projekt pre predmet IPK
# Prenos suborov
#
# @author xormos00, Michal Ormos
# xormos00@stud.fit.vutbr.cz
# @date 9.4.2016
#

CC=g++
CFLAGS= -std=c++98 -pedantic -Wall -W -g3 -pthread

all: server client

server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client

#Vycistenie po sebe
.PHONY:	clean

clean:
	rm -f *.o main
