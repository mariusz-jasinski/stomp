###################################################
#
# Makefile for libstomp-c-library.sl library
#
###################################################
 
CXX=aCC
CCOPT=+Z
CCINCL=-I.

.SUFFIXES: .c .o .cpp:

.c.o:
	$(CXX) -c $(CCOPT) $(CCINCL) $< -o $@

MODULY = \
	debug.o \
	hdr.o \
	frame.o \
	stomp.o

LIBNAME=libstomp-c-library.sl
SERVER=BC_Menadzer

default: $(MODULY)
	@echo STOMP: Library compilation [$(LIBNAME)]
	$(CXX) -b $(MODULY) -o ../lib/${LIBNAME}
  
clean: 
	@echo STOMP: Deleting temporary files
	@rm -f $(MODULY)

copy_sl:
	@echo KOPIOWANIE $(LIBNAME) DO: $(DOMENA_BIN_PATH)
	cp -Rp ../lib/${LIBNAME} $(DOMENA_BIN_PATH)/
  
copy:
	@echo STOP SERWERA: [$(SERVER)]
	@tmshutdown -s $(SERVER)
	@echo waiting 3s ...
	@sleep 3
	@echo KOPIOWANIE $(LIBNAME) DO: $(DOMENA_BIN_PATH)
	cp -Rp ../lib/${LIBNAME} $(DOMENA_BIN_PATH)/
	@echo START SERWERA
	@tmboot -s $(SERVER)
