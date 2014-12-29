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
//const char* h2fax::na = "N/A\0";

h2fax::faxcover_args h2fax::getFaxcoverParameters(int argc, char* argv[], const char switches[])
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
            if(optarg)
                args.comments = optarg;
            else
                args.comments = na;
            break;
        case 'C':
            if(optarg)
                args.template_path = optarg;
            else
                args.template_path = na;
            break;
        case 'l':
            if(optarg)
                args.to_location = optarg;
            else
                args.to_location = na;
            break;
        case 'L':
            if(optarg)
                args.from_location = optarg;
            else
                args.from_location = na;
            break;
        case 'M':
            if(optarg)
                args.email = optarg;
            else
                args.email = na;
            break;
        case 'n':
            if(optarg)
                args.to_fax = optarg;
            else
                args.to_fax = na;
            break;
        case 'N':
            if(optarg)
                args.from_fax = optarg;
            else
                args.from_fax = na;
            break;
        case 'p':
            if(optarg)
                args.pageCount = optarg;
            else
                args.pageCount = na;
            break;
        case 'r':
            if(optarg)
                args.regarding = optarg;
            else
                args.regarding = na;
            break;
        case 'f':
            if(optarg)
                args.from = optarg;
            else
                args.from = na;
            break;
        case 't':
            if(optarg)
                args.to = optarg;
            else
                args.to = na;
            break;
        case 'v':
            if(optarg)
                args.to_voice = optarg;
            else
                args.to_voice = na;
            break;
        case 'V':
            if(optarg)
                args.from_voice = optarg;
            else
                args.from_voice = na;
            break;
        case 'x':
            if(optarg)
                args.to_company = optarg;
            else
                args.to_company = na;
            break;
        case 'X':
            if(optarg)
                args.from_company = optarg;
            else
                args.from_company = na;
            break;
            /*Database information switches!*/
        case 'u':
            if(optarg)
                args.username = optarg;
            else
                args.username = na;
            break;
        case 'U':
            if(optarg)
                args.engine = optarg;
            else
                args.engine = na;
            break;
        case 'P':
            if(optarg)
                args.password = optarg;
            else
                args.password = na;
            break;
        case 'd':
            if(optarg)
                args.database = optarg;
            else
                args.database = na;
            break;
        case 'H':
            if(optarg)
                args.host = optarg;
            else
                args.host = na;
            break;
            /*Execution mode flag*/
        case 'E':
            if(optarg)
                args.execMode = optarg;
            else
                args.execMode = na;
            break;
            /*Page size*/
        case 's':
            if(optarg)
                args.pageSize = optarg;
            else
                args.pageSize = na;
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

void h2fax::getRecvdFaxParameters(int argc, char* argv[], h2fax::faxrecvd_args& args)
{
    for(int i = 0; i < argc; i++)
    {
        switch(i)
        {
        case 0:
            args.appName = argv[i];
            break;
        case 1:
            std::cerr << argv[i] << "Hi";
            args.tiff_file = argv[i];
            break;
        case 2:
            args.modemdev = argv[i];
            break;
        case 3:
            args.commID = argv[i];
            break;
        case 4:
            args.errormsg = argv[i];
            break;
        default:
            std::cerr << "Error: More parameters were passed to the program than expected!" << std::endl;
        }
    }
}

