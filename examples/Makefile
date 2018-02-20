###################################################
#
# Makefile for send, listen example
#
###################################################

CXX=aCC
CCOPT=+z
CCINCL=-I. -I $(STOMPLIBSRCDIR)


.SUFFIXES: .c .o .cpp:

.c.o:
	$(CXX) -c $(CCOPT) $(CCINCL) -o $@ $<


PROGNAME1=send
PROGNAME2=listen
PROGNAME3=sendshort

default: $(PROGNAME1).o $(PROGNAME2).o $(PROGNAME3).o
	@echo STOMP: Example program compilation [$(PROGNAME1)]
	$(CXX) $(CCOPT) $(CCINCL) $(PROGNAME1).c -o $(PROGNAME1) -L$(STOMPLIBDIR) -lstomp-c-library
	@echo STOMP: Example program compilation [$(PROGNAME2)]
	$(CXX) $(CCOPT) $(CCINCL) $(PROGNAME2).c -o $(PROGNAME2) -L$(STOMPLIBDIR) -lstomp-c-library
	@echo STOMP: Example program compilation [$(PROGNAME3)]
	$(CXX) $(CCOPT) $(CCINCL) $(PROGNAME3).c -o $(PROGNAME3) -L$(STOMPLIBDIR) -lstomp-c-library

clean: 
	@echo STOMP: Deleting temporary files
	@rm -f $(PROGNAME1).o  
	@rm -f $(PROGNAME2).o  
	@rm -f $(PROGNAME3).o  
