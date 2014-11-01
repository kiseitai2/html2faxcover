/*
    Copyright (C) 2014 Luis M. Santos
    Contact: luismigue1234@hotmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with This program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <getopt.h>
#include "cmd_processor.h"
#include "conversion.h"


h2fax::cstr SWITCHES = "c:C:d:E:f:H:l:L:M:n:N:p:P:r:s:t:u:U:v:V:x:X:\0";

h2fax::faxcover_args h2fax::getParameters(int argc, char* argv[], const char switches[])
{
    char o = 0;
    faxcover_args args;

    args.appName = argv[0];

    if(argc == 1)
    {
	std::cerr << "Error: The program was invoked with no arguments!"
		<< std::endl;
	args.argNum = 1;
        return args;
    }

    while((o = getopt(argc, argv, switches)) != -1)
    {
        switch(o)
        {
        case 'c':
            args.comments = optarg;
            break;
        case 'C':
            args.template_path = optarg;
            break;
        case 'l':
            args.to_location = optarg;
            break;
        case 'L':
            args.from_location = optarg;
            break;
        case 'M':
            args.email = optarg;
            break;
        case 'n':
            args.to_fax = optarg;
            break;
        case 'N':
            args.from_fax = optarg;
            break;
        case 'p':
            args.pageCount = optarg;
            break;
        case 'r':
            args.regarding = optarg;
            break;
        case 'f':
            args.from = optarg;
            break;
        case 't':
            args.to = optarg;
            break;
        case 'v':
            args.to_voice = optarg;
            break;
        case 'V':
            args.from_voice = optarg;
            break;
        case 'x':
            args.to_company = optarg;
            break;
        case 'X':
            args.from_company = optarg;
            break;
            /*Database information switches!*/
        case 'u':
            args.username = optarg;
            break;
        case 'U':
            args.engine = optarg;
            break;
        case 'P':
            args.password = optarg;
            break;
        case 'd':
            args.database = optarg;
            break;
        case 'H':
            args.host = optarg;
            break;
        /*Execution mode flag*/
        case 'E':
            args.execMode = optarg;
            break;
        /*Page size*/
        case 's':
            args.pageSize = optarg;
            break;
        /*Default cases*/
        case '?':
        default:
            std::cerr << "Invalid parameters! Read on faxcover usage! "
                << "http://hylafax.sourceforge.net/man/faxcover.php"
                << std::endl;
        }
    }
    args.argNum = 2;
    return args;

}

h2fax::cstr h2fax::faxcover_args::operator[](cstr arg)
{
    switch(hashFunction(arg))
    {
    case 0x324c: //to
        return to;
        break;
    case 0xeae7: //from
        return from;
        break;
    case 0x1076b: //to-company
        return to_company;
        break;
    case 0x2a36f:
        return from_company;
        break;
    case 0x10761:
        return to_location;
        break;
    case 0x299e5:
        return from_location;
        break;
    case 0x3761d:
        return from_voice;
        break;
    case 0x264b0:
        return to_voice;
        break;
    case 0x31356:
        return from_fax;
        break;
    case 0x136fb:
        return to_fax;
        break;
    case 0x37d85:
        return email;
        break;
    case 0x1a265:
        return regarding;
        break;
    case 0x1b30a:
        return comments;
        break;
    case 0x1e4bb:
        return date.c_str();
        break;
    case 0x1fd8b:
        return pageCount;
        break;
    /*Database variables*/
    case 0xfdfb:
        return host;
        break;
    case 0x19466:
        return username;
        break;
    case 0x173c5:
        return password;
        break;
    case 0x19247:
        return database;
        break;
    /*Execution mode flag*/
    case 0x16e6d:
        return execMode;
        break;
    case 0x163bb:
        return pageSize;
        break;
    default:
        std::cerr << "Wrong parameter! This instance of faxcover_args will not return a reference to an internal data member!"
        << std::endl;
    }

    return NULL;
}
h2fax::uint h2fax::faxcover_args::hashFunction(cstr arg)
{
    std::string tmp = arg;
    switch(tmp.size())
    {
    case 2:
        return tmp[0] * tmp[1];
        break;
    case 1:
        return tmp[0];
        break;
    case 0:
        return 0;
        break;
    case 3:
    default:
        return tmp[0] * tmp[1] + tmp[2] * tmp.size() * tmp[tmp.size() - 1] + tmp[tmp.size() / 2];
    }
    return 0;
}

h2fax::cstr h2fax::getDefaultSwitches()
{
    return SWITCHES;
}


