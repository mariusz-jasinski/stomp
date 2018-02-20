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
SERVER=STOMP_Testy

default: $(MODULY)
	@echo STOMP: Library compilation [$(LIBNAME)]
	$(CXX) -b $(MODULY) -o ../lib/${LIBNAME}
  
clean: 
	@echo STOMP: Deleting temporary files
	@rm -f $(MODULY)

copy_sl:
	@echo STOMP: Copying $(LIBNAME) to: $(DOMENA_BIN_PATH)
	cp -Rp ../lib/${LIBNAME} $(DOMENA_BIN_PATH)/
  
copy:
	@echo STOMP: Stop server: [$(SERVER)]
	@tmshutdown -s $(SERVER)
	@echo waiting 3s ...
	@sleep 3
	@echo STOMP: Copying $(LIBNAME) to: $(DOMENA_BIN_PATH)
	cp -Rp ../lib/${LIBNAME} $(DOMENA_BIN_PATH)/
	@echo STOMP: Start server:
	@tmboot -s $(SERVER)
