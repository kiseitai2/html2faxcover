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
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <random>
#include <time.h>
#include "html2faxcover.h"
#include "data_base.h"


//Prototypes
bool checkConnection(MySQL& db, const h2fax::faxrecvd_args& args, size_t numTrials);

int main(int argc, char* argv[])
{
    /* Init
    */
    //Check if the internal API is the right version
    if(h2fax::PROG_VERSION < 1.1f)
        return EXIT_FAILURE;
    h2fax::faxrecvd_args args;
    h2fax::fax fax_info;
    /*Reroute output streams*/
    //Let's redirect cerr
    std::ofstream cerrLog("/tmp/cerrlog");
    std::streambuf *cerr_copy = std::cerr.rdbuf();
    if(cerrLog.is_open())
        std::cerr.rdbuf(cerrLog.rdbuf());
    //Let's redirect cout
    std::ofstream coutLog("/tmp/coutlog");
    std::streambuf *cout_copy = std::cout.rdbuf();
    if(cerrLog.is_open())
        std::cout.rdbuf(cerrLog.rdbuf());
    /*Create other variables needed*/
    data_base settings("/etc/html2faxcover/settings");
    std::string tiff2pdf_path = settings.GetStrFromData("tiff2pdf");
    std::string fax_path = settings.GetStrFromData("fax_path");
    std::string tiffcp = settings.GetStrFromData("tiffcp");
    std::string fileinfo_prog = settings.GetStrFromData("fileinfo");
    std::string sendmail = settings.GetStrFromData("sendmail");
    std::string convert = settings.GetStrFromData("convert_cmd");
    std::string printer_cmd = settings.GetStrFromData("print_cmd");
    std::string printer_name = settings.GetStrFromData("printer");
    std::string printer_flag = settings.GetStrFromData("print_flag");
    std::string tmp_path = settings.GetStrFromData("tmp");
    std::string user = settings.GetStrFromData("usr");
    std::string group = settings.GetStrFromData("grp");
    bool print = settings.GetIntFromData("do_print");
    bool mail = settings.GetIntFromData("do_mail");
    std::string recipient = "";
    bool enable_fax_annotation = settings.GetIntFromData("fax_annotation");
    size_t trials = settings.GetIntFromData("reconnection_attempts");
    //Process arguments
    h2fax::getRecvdFaxParameters(argc, argv, args);
    /*Obtain database info*/
    //Let's store the database connection info
    args.Database = settings.GetStrFromData("database").c_str();
    args.Host = settings.GetStrFromData("host").c_str();
    args.Username = settings.GetStrFromData("username").c_str();
    args.Password = settings.GetStrFromData("password").c_str();
    //Let's save the c_string pointers in the regular spot for arguments
    args.database = args.Database.c_str();
    args.host = args.Host.c_str();
    args.username = args.Username.c_str();
    args.password = args.Password.c_str();
    /*Build a database connection*/
    MySQL db(args.database, args.username, args.password, args.host);
    /*Let's check the state of the connection. If the connection is down,
    block this process until a connection is made. Attempt this a fixed
    number of times with each try at a random interval. This is to make
    sure we get the data in the database.*/
    checkConnection(db, args, trials);
    
    /*Prog
    */
    std::string tiff_name = "fax.tiff";
    std::string pdf_name = "fax.pdf";
    std::string thumbnail = "thumb.gif";

    h2fax::logMsg("Obtain Fax Info!");
    //Let's copy the file to a writable location so we do not have permission clashes.
    std::string tmp_destination = tmp_path + "tmp.tif";
    /*Let's lock the tmp directory*/
    h2fax::exec_cmd(tmp_path.c_str(), "flock", "-c faxrcvd"); 
    /*Let's make sure the tmp location is empty.*/
    h2fax::exec_cmd(tmp_destination.c_str(), "rm -rf", "");
    h2fax::copyFile(args.tiff_file, tmp_destination.c_str());
    //Let's use our copy for the following couple of transactions!
    fax_info = h2fax::faxinfo(fileinfo_prog, tmp_destination);

    h2fax::logMsg("Copying tiff file!");
    std::string fax_file_dir = fax_info.docTime.Year + PATH_SLASH + fax_info.docTime.Month + PATH_SLASH + fax_info.docTime.Day + PATH_SLASH + fax_info.docTime.Hour + PATH_SLASH + fax_info.docTime.Minute + PATH_SLASH + fax_info.CallID1;
    std::string fax_dir = fax_path + fax_file_dir + PATH_SLASH;
    h2fax::exec_cmd("-p -v -m 744", "mkdir", fax_dir);
    std::string fax_file = fax_dir + tiff_name;
    h2fax::exec_cmd(tmp_destination.c_str(), tiffcp, fax_file);

    h2fax::logMsg("Creating PDF version...");
    std::string pdf_file = fax_dir + pdf_name;
    h2fax::exec_cmd((fax_file + " -o").c_str(), tiff2pdf_path, pdf_file);

    //Let's make a thumbnail!
    h2fax::logMsg("Creating thumbnail from PDF file...");
    h2fax::create_preview(fax_dir, pdf_name, thumbnail, convert, cStrToInt(fax_info.Pages.c_str()));

    //Let's get the fax category id before we move on!
    h2fax::logMsg("Getting fax category id...");
    std::string faxcatid = "0";
    //Let's get the category id
    checkConnection(db, args, trials);//Check the connection again!
    db.queryDB((db.prepareStatement("Modems", "faxcatid", std::string("device = ") +  "'" + args.modemdev + "'","","",SELECT|FROM|WHERE)));
    if(db.hasResults())
    {
        db.getResult(faxcatid);
    }

    //Check fax number in addressbook!
    h2fax::logMsg("Checking caller ID...");
    std::string faxnumid = "0";
    checkConnection(db, args, trials);//Check the connection again!
    db.queryDB(db.prepareStatement("AddressBookFAX", "abook_id", std::string("faxnumber = ") + "'" + fax_info.CallID1 + "'","","",SELECT | FROM | WHERE));
    if(db.hasResults())//Find out if the fax number is registered in the address book
    {
        db.getResult(faxnumid);
        db.queryDB(db.prepareStatement("AddressBook", "company", std::string("abook_id = ") + faxnumid,"","",SELECT|FROM|WHERE));
        db.getResult(fax_info.CallID2);
    }
    else//Create a new entry for this fax with the information available
    {
        //Create entry in table AddressBook
        db.queryDB(db.prepareStatement("AddressBook", "company", "'" + fax_info.CallID2 + "'","","",INSERT|INTO));
        //Select the autogenerated id for CallID2 in AddressBook after insertion
        db.queryDB(db.prepareStatement("AddressBook", "abook_id", std::string("company = ") + "'" + fax_info.CallID2 + "'","","",SELECT|FROM|WHERE));
        db.getResult(faxnumid);
        db.queryDB(db.prepareStatement("AddressBookFAX", std::string("abook_id,faxnumber,") + "faxcatid,printer",
                                    faxnumid + SQL_COMA + "'" + fax_info.CallID1 + "'" + SQL_COMA + faxcatid + SQL_COMA + SQL_NULL_STR,""
                                     ,"",INSERT|INTO));
    }

    //Let's enable DID routing!
    h2fax::logMsg("Enabling DID routing...");
    std::string didnum_id = "";
    checkConnection(db, args, trials);//Check the connection again!
    db.queryDB(db.prepareStatement("DIDRoute", "didr_id", std::string("routecode = ") + "'" + fax_info.CallID3 + "'","","",SELECT | FROM | WHERE));
    if(db.hasResults())//Find out if the DID number is registered in the database
    {
        db.getResult(didnum_id);
    }
    else//Create a new entry for this fax with the information available
    {
        //Create entry in table didroute
        db.queryDB(db.prepareStatement("DIDRoute", "routecode,alias,contact,printer,faxcatid",
                                    fax_info.CallID3 + SQL_COMA + SQL_NULL + SQL_COMA + SQL_NULL + SQL_COMA
                                     + SQL_NULL + SQL_COMA + SQL_NULL,"","",INSERT|INTO));
    }

    //Add Fax to inbox!
    h2fax::logMsg("Adding Fax to Inbox...");
    std::string faxid = "";
    //Generate Date
    std::string date = fax_info.docTime.Year + "-" + fax_info.docTime.Month + "-" + fax_info.docTime.Day + " " + fax_info.docTime.Hour + ":" + fax_info.docTime.Minute + ":" + fax_info.docTime.Second;
    //Write fax entry to the database (table faxarchive)
    checkConnection(db, args, trials);//Check the connection again!
    db.queryDB(db.prepareStatement("FaxArchive", "faxpath,pages,didr_id,archstamp,faxnumid,origfaxnum,faxcatid,modemdev",
                                   "'/faxes/recvd/" + fax_file_dir + "'" + SQL_COMA + fax_info.Pages + SQL_COMA + didnum_id + SQL_COMA +
                                   "'" + date + "'" + SQL_COMA + faxnumid
                                   + SQL_COMA + "'" + fax_info.Sender + "'" + SQL_COMA + faxcatid + SQL_COMA + "'" + args.modemdev + "'","","",INSERT|INTO));
    db.queryDB(db.prepareStatement("FaxArchive", "fid", std::string("origfaxnum = ") + "'" + fax_info.Sender + "'","","",SELECT | FROM | WHERE));
    if(db.hasResults())//Find out if we successfully added the fax entry
    {
        db.getResult(faxid);
        h2fax::logMsg(std::string("Successfully added fax entry to database! Houston, we have a lift off! *Applauses* ") + "With fax id: " + faxid);
    }
    else//log an error
    {
        h2fax::logMsg(std::string("Oops, the ship sunk! This is why we can't have nice things... The program was not able") +
                      "to add the fax entry to the database! :(");
    }

    //Fax annotation
    h2fax::logMsg("Processing fax annotation!");
    if(enable_fax_annotation)
    {
        //Future versions should attempt to have a block of code here like in AvantFax's faxrecvd!
    }

    //Let's send an email!
    if(mail)
    {
    h2fax::logMsg("Preparing to send e-mail!");
    size_t rows = 0;
    //Let's compile a list of destinations to which we want to send the email!
    checkConnection(db, args, trials);//Check the connection again!
    db.queryDB(db.prepareStatement("UserAccount", "email", std::string("modemdevs = ") + "'" + args.modemdev + "'","","",SELECT|FROM|WHERE));
    rows = db.rowCount();
    if(rows)
    {
        std::string tmp;
        recipient += "\"";
        for(size_t i = 0; i < rows; i++)
        {
     std::cerr << rows << tmp << std::endl;
            db.getResult(tmp);
            recipient += tmp;
            recipient += ",";
        }
        recipient += "\"";
    }
    //Send the fax to all users associated with the modem
    h2fax::exec_cmd(std::string("\"/a " + fax_file + " /a " + pdf_file + "\"").c_str(), "sendmail", recipient + "\"Fax_From: " + fax_info.CallID2 + "\"");
    }

    //Print Support
    h2fax::logMsg("Printing Fax to preferred printer...");
    if(printer_name == "" && print)//If empty, print in default printer!
    {
        h2fax::exec_cmd(pdf_file.c_str(), printer_cmd, "");
    }
    else if(print)//Otherwise, print to the printer indicated in the settings file!
    {
        h2fax::exec_cmd("", printer_cmd, "-" + printer_flag + " " + printer_name + " " + pdf_file);
    }


    //Because file permissions are copied with the tiff file, I have to change the permissions back to what we want!
    h2fax::logMsg("Resetting permissions!");
    //std::string permission_str = std::string("-R ") + group + "." + user + " ";
    //h2fax::exec_cmd(permission_str.c_str(), "chown ", fax_file.substr(0, fax_file.rfind("/")));
    h2fax::exec_cmd("-R ", "chmod a+rx ", fax_path + fax_info.docTime.Year + PATH_SLASH);//fax_file.substr(0, fax_file.rfind("/")) + PATH_SLASH);

    //Let's clean our tmp file!
    h2fax::exec_cmd(tmp_destination.c_str(), "rm -rf", "");

    //Restore cerr
    if(cerrLog.is_open())
        std::cerr.rdbuf(cerr_copy);

    //Restore cout
    if(coutLog.is_open())
        std::cout.rdbuf(cout_copy);

    return EXIT_SUCCESS;
}

bool checkConnection(MySQL& db, const h2fax::faxrecvd_args& args, size_t numTrials)
{
    srand(time(NULL));
    while(!db.validConnection(db.prepareStatement("UserAccount", "email", "","","",SELECT)) && numTrials > 0)
    {
        sleep(rand()%100 + 1000);
        db.reconnect(args.database, args.username, args.password, args.host);
        numTrials--;
    }
}

