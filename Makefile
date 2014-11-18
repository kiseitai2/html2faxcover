CC = gcc
CXX = g++
AR = ar

CXXFLAGS_DEBUG = -std=c++0x -g
CXXFLAGS = -std=c++0x -Os
LDFLAGS = -lmysqlcppconn
Mode = 
H2Fax = cleannothing

INSTALL_DIR = /var/spool/html2faxcover/
LIB_INSTALL_DIR = /usr/local/lib/
CONF_DIR = /etc/html2faxcover/
Hylafax_DIR = /var/spool/hylafax/
AppName = html2faxcover
App2Name = faxrecvd
LibName = libhtml2faxcover.a
Conf = settings
Apache_owner = www-data
Apache_group = www-data

all: clean html2faxcover$(Mode) libhtml2faxcover faxrecvd$(Mode)
	
html2faxcover_debug: cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o
	$(CXX) $(CXXFLAGS_DEBUG) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o $(LDFLAGS) -o html2faxcover_debug
	
html2faxcover: cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o
	$(CXX) $(CXXFLAGS) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o $(LDFLAGS) -o html2faxcover
	
libhtml2faxcover: conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o faxfunctions.o
	$(AR) -rcs libhtml2faxcover.a conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o faxfunctions.o

faxrecvd_debug: faxrecvd.o cmd_processor.o conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o faxfunctions.o
	$(CXX) $(CXXFLAGS_DEBUG) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o $(LDFLAGS) -o faxrecvd_debug
	
faxrecvd: faxrecvd.o cmd_processor.o conversion.o data_base.o sqlkeywords.o mysql.o marker_parser.o faxfunctions.o
	$(CXX) $(CXXFLAGS) cmd_processor.o conversion.o data_base.o sqlkeywords.o main.o mysql.o marker_parser.o faxfunctions.o $(LDFLAGS) -o faxrecvd
	
cmd_processor.o: cmd_processor.cpp

conversion.o: conversion.cpp conversion.h

data_base.o: data_base.cpp data_base.h

sqlkeywords.o: sqlkeywords.cpp sqlkeywords.h

mysql.o: mysql.cpp mysql.h sqlkeywords.h data_base.h conversion.h

main.o: main.cpp mysql.h sqlkeywords.h data_base.h conversion.h typedefs.h marker_parser.h

marker_parser.o: marker_parser.cpp marker_parser.h data_base.h conversion.h typedefs.h default.h

faxfunctions.o: faxfunctions.cpp marker_parser.h faxfunctions.h

faxrecvd.o: faxrecvd.cpp cmd_processor.cpp cmd_processor.h html2faxcover.h

clean: 
	rm -rf *o html2faxcover
	
cleanhtml2faxcover:
	rm -rf $(Hylafax_DIR)bin/faxrcvd
	cp $(Hylafax_DIR)bin/faxrcvd_old_script $(Hylafax_DIR)bin/faxrcvd
	
cleannothing:
	echo "This is the first time you install Html2faxcover, so make will not delete the current faxrecvd script."

msg:
	echo "Compiling and installing html2faxcover$(Mode), libhtml2faxcover, and faxrecvd$(Mode)!"
	
clean_debug:
	rm -rf *o html2faxcover_debug

install: clean msg all $(H2Fax)
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)tmp/
	mkdir -p $(INSTALL_DIR)faxes/recvd/
	mkdir -p $(CONF_DIR)
	chown -R $(Apache_owner):$(Apache_group) $(INSTALL_DIR)
	chown -R $(Apache_owner):$(Apache_group) $(INSTALL_DIR)tmp/
	chown -R $(Apache_owner):$(Apache_group) $(INSTALL_DIR)faxes/recvd/
	chown -R $(Apache_owner):$(Apache_group) $(CONF_DIR)
	cp $(AppName)$(Mode) $(INSTALL_DIR)$(AppName)$(Mode)
	cp $(App2Name)$(Mode) $(INSTALL_DIR)$(App2Name)$(Mode)
	cp $(LibName) $(LIB_INSTALL_DIR)$(LibName)
	cp $(Conf) $(CONF_DIR)$(Conf)
	cp $(Hylafax_DIR)bin/faxrcvd $(Hylafax_DIR)bin/faxrcvd_old_script
	rm -rf $(Hylafax_DIR)bin/faxrcvd
	ln -s $(INSTALL_DIR)$(App2Name)$(Mode) $(Hylafax_DIR)bin/faxrcvd
	
update_conf:
	cp $(Conf) $(CONF_DIR)$(Conf)

uninstall: clean
	rm -rf $(INSTALL_DIR)
	rm -rf $(LIB_INSTALL_DIR)$(LibName)
	rm -rf $(CONF_DIR)
	rm -rf $(Hylafax_DIR)bin/faxrcvd
	cp $(Hylafax_DIR)bin/faxrcvd_old_script $(Hylafax_DIR)bin/faxrcvd

