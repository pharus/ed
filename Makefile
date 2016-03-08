#
# Makefile for ed
#

CFLAGS=-W -Wall -pedantic -g

ed: ed.c

clean:
	@echo cleaning
	@$(RM) ed

