CC = gcc
CXX = g++
AR = ar

CXXFLAGS_DEBUG = -std=c++0x -g
CXXFLAGS = -std=c++0x -Os
LDFLAGS = -lmysqlcppconn

INSTALL_DIR = /var/spool/html2faxcover/
LIB_INSTALL_DIR = /usr/local/lib/
CONF_DIR = /etc/html2faxcover/
AppName = html2faxcover
LibName = libhtml2faxcover.a
Conf = settings
Apache_owner = www-data
Apache_group = www-data

all: html2faxcover libhtml2faxcover clean html2faxcover_debug
	
html2faxcover_debug: cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o
	$(CXX) $(CXXFLAGS_DEBUG) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o $(LDFLAGS) -o html2faxcover_debug
	
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
	
clean_debug:
	rm -rf *o html2faxcover_debug

install: html2faxcover libhtml2faxcover
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)tmp/
	mkdir -p $(CONF_DIR)
	chown -R $(Apache_owner):$(Apache_group) $(INSTALL_DIR)
	chown -R $(Apache_owner):$(Apache_group) $(INSTALL_DIR)tmp/
	chown -R $(Apache_owner):$(Apache_group) $(CONF_DIR)
	cp $(AppName) $(INSTALL_DIR)$(AppName)
	cp $(LibName) $(LIB_INSTALL_DIR)$(LibName)
	cp $(Conf) $(CONF_DIR)$(Conf)
	
update_conf:
	cp $(Conf) $(CONF_DIR)$(Conf)

uninstall: clean
	rm -rf $(INSTALL_DIR)
	rm -rf $(LIB_INSTALL_DIR)$(LibName)
	rm -rf $(CONF_DIR)

