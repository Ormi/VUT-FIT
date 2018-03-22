 #
 # @file Makefile
 #
 # Prvy projekt pre predmet IPK
 # Webovy klient
 #
 # @author xormos00, Michal Ormos
 # xormos00@stud.fit.vutbr.cz
 # @date 12.3.2016
 #

PROJ=dhcp-stats
PROGS=$(PROJ)
CC=g++
CFLAGS= -std=c++11 -pedantic -Wall -lncurses -lpthread
LIBS=-lpcap

all: $(PROGS)

$(PROJ): $(PROJ).cpp
	$(CC) $(CFLAGS) $(PROJ).cpp $(LIBS) -o $(PROJ)

#Vycistenie po sebe
.PHONY:	clean

clean:
	rm -f *.o main
