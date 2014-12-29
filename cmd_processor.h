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
#ifndef CMD_PROCESSOR_H_INCLUDED
#define CMD_PROCESSOR_H_INCLUDED

#ifndef NULL
#define NULL 0
#endif

#include <iostream>
#include <string>
#include "typedefs.h"
#include "default.h"

namespace h2fax
{
static const char* na = "N/A\0";
typedef struct h2fax_args
{
    cstr appName;//file path + this application's name
    cstr from;//sending person
    cstr to;//destination person
    cstr from_company;//destination company
    cstr to_company;//sending company
    cstr to_location;//destination location
    cstr from_location;//sending location
    cstr regarding;//Re: topic of the facsimile
    cstr comments;//Comments in cover sheet
    cstr to_fax;//destination fax number
    cstr from_fax;//sending fax number
    cstr to_voice;//destination phone number
    cstr from_voice;//sending phone number
    cstr template_path;//path to fax cover sheet
    cstr email;//email of sender
    cstr pageCount;//Number of pages in the fax job
    cstr tiff_file;//Location of received tiff fax file
    cstr modemdev;//Modem device on which the fax was received
    cstr commID;//Communication identifier for incoming call
    cstr errormsg;//Error message if a problem ocurred during the receive operation
    cstr CIDNumber;
    cstr CIDName;
    cstr DIDNum;
    std::string date;//Today's date
    cstr pageSize;
    cstr username, password, database, engine, host; //Database connection information
    std::string Username, Password, Database, Engine, Host;
    cstr execMode; //Variable that will hold the execution mode flag.
    size_t argNum;

    h2fax_args()
    {
        appName = NULL;//file path + this application's name
        from = na;//sending person
        to = na;//destination person
        from_company = na;//destination company
        to_company = na;//sending company
        to_location = na;//destination location
        from_location = na;//sending location
        regarding = na;//Re: topic of the facsimile
        comments = na;//Comments in cover sheet
        to_fax = na;//destination fax number
        from_fax = na;//sending fax number
        to_voice = na;//destination phone number
        from_voice = na;//sending phone number
        template_path = NULL;//path to fax cover sheet
        email = na;//email of sender
        pageCount = NULL;//Number of pages in the fax job
        pageSize = NULL;//Size of the page
        date = "";//Today's date
        /*Faxrecvd parameters!*/
        tiff_file = NULL;
        modemdev = NULL;
        commID = NULL;
        errormsg = NULL;
        CIDNumber = NULL;
        CIDName = NULL;
        DIDNum = NULL;
        /*Database connection info*/
        engine = NULL;
        username = NULL;
        password = NULL;
        database = NULL;
        host = NULL;
        /*Execution mode flag*/
        execMode = "0";
        argNum = 0;
    }
    cstr operator[](cstr arg);
    uint hashFunction(cstr arg);
} faxcover_args, faxrecvd_args;


/*Because AvantFax can send us an argument string with empty options when options are expected (-u -P), I made this
class to set those empty args to "" instead of NULL. Doing so will prevent malfunctioning of the program!
*/


cstr getDefaultSwitches();
faxcover_args getFaxcoverParameters(int argc, char* argv[], const char switches[] = getDefaultSwitches());
void getRecvdFaxParameters(int argc, char* argv[], faxrecvd_args& args);
}



#endif // CMD_PROCESSOR_H_INCLUDED

