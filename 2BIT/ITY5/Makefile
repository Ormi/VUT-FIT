#File:		Makefile
#Author:	Michal Ormos - 1BIT
#		xormos00@stud.fit.vutbr.cz
#Description:	Makefile for fifth ITY project

NAME=proj5

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm -f $(NAME).aux $(NAME).log $(NAME).dvi $(NAME).ps $(NAME).out
