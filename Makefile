CC = gcc
CXX = g++
AR = ar

CXXFLAGS = -std=c++0x -g
LDFLAGS = -lmysqlcppconn

INSTALL_DIR = /usr/local/bin/
LIB_INSTALL_DIR = /usr/local/lib/
AppName = html2faxcover
LibName = libhtml2faxcover.a
Conf = settings

all: html2faxcover libhtml2faxcover
	
html2faxcover: cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o
	$(CXX) $(CXXFLAGS) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o $(LDFLAGS) -o html2faxcover
	
libhtml2faxcover: conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o
	$(AR) -rcs libhtml2faxcover.a conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o

cmd_processor.o: cmd_processor.cpp

conversion.o: conversion.cpp conversion.h

data_base.o: data_base.cpp data_base.h

sqlkeywords.o: sqlkeywords.cpp sqlkeywords.h

mysql.o: mysql.cpp mysql.h sqlkeywords.h data_base.h conversion.h

main.o: main.cpp mysql.h sqlkeywords.h data_base.h conversion.h typedefs.h marker_parser.h

marker_parser.o: marker_parser.cpp marker_parser.h data_base.h conversion.h typedefs.h default.h

clean: 
	rm -rf *o html2faxcover

install: html2faxcover libhtml2faxcover
	cp $(AppName) $(INSTALL_DIR)$(AppName)
	cp $(LibName) $(LIB_INSTALL_DIR)$(LibName)
	cp $(Conf) $(INSTALL_DIR)$(Conf)

uninstall: clean
	rm -f $(INSTALL_DIR)$(AppName)
	rm -f $(LIB_INSTALL_DIR)$(LibName)
	rm -f $(INSTALL_DIR)$(Conf)
