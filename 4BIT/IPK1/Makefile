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

PROJ=webclient
PROGS=$(PROJ)
CC=g++
CFLAGS=-pedantic -Wextra -Wall

all: $(PROGS)

$(PROJ): $(PROJ).cpp
	$(CC) $(CFLAGS) $(PROJ).cpp -o $(PROJ)

#Vycistenie po sebe
.PHONY:	clean

clean:
	rm -f *.o main