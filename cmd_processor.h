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

    struct faxcover_args
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
        std::string date;//Today's date
        cstr pageSize;
        cstr username, password, database, engine, host; //Database connection information
	std::string Username, Password, Database, Engine, Host;
        cstr execMode; //Variable that will hold the execution mode flag.
	size_t argNum;

        faxcover_args()
        {
            appName = NULL;//file path + this application's name
            from = NULL;//sending person
            to = NULL;//destination person
            from_company = NULL;//destination company
            to_company = NULL;//sending company
            to_location = NULL;//destination location
            from_location = NULL;//sending location
            regarding = NULL;//Re: topic of the facsimile
            comments = NULL;//Comments in cover sheet
            to_fax = NULL;//destination fax number
            from_fax = NULL;//sending fax number
            to_voice = NULL;//destination phone number
            from_voice = NULL;//sending phone number
            template_path = NULL;//path to fax cover sheet
            email = NULL;//email of sender
            pageCount = NULL;//Number of pages in the fax job
            pageSize = NULL;//Size of the page
            date = "";//Today's date
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
    };

    cstr getDefaultSwitches();
    faxcover_args getParameters(int argc, char* argv[], const char switches[] = getDefaultSwitches());
}



#endif // CMD_PROCESSOR_H_INCLUDED
