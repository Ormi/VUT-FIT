#File:		Makefile
#Author:	Michal Ormos - 1BIT
#		xormos00@stud.fit.vutbr.cz
#Description:	Makefile for third ITY project

NAME=proj4

all:
	latex $(NAME).tex
	bibtex $(NAME).aux
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm -f $(NAME).aux $(NAME).dvi $(NAME).ps $(NAME).bbl $(NAME).blg $(NAME).log
