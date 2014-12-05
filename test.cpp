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
#include "mysql.h"
#include "data_base.h"
#include "conversion.h"

int main(int argc, char* argv[])
{
    int res;


    //Let's redirect cerr
    std::ofstream cerrLog("/tmp/cerrlog");
    std::streambuf *cerr_copy = std::cerr.rdbuf();
    if(cerrLog.is_open())
        std::cerr.rdbuf(cerrLog.rdbuf());

    /*Database test*/
    h2fax::cstr database = "avantfax\0";
    h2fax::cstr host = "localhost\0";
    h2fax::cstr password = "d58fe49\0";//Add your own MySQL/WAMP/LAMP account info
    h2fax::cstr username = "avantfax\0";

    std::cout << "Connecting to database: " << database << " with username " << username << " and password:"
              << password << "." << std::endl;
    MySQL db(database, username, password, host);

    std::string query = db.prepareStatement("UserAccount", "email", "username='kiseitai2'","","",SELECT|WHERE);
    std::cout << "Status: " << db.getStatus() << std::endl;
    std::cout << "Query: " << query << std::endl;
    std::string email;
    std::cout << "Querying database!" << std::endl;
    db.queryDB(query);
    std::cout << "Obtaining results!" << std::endl;
    db.getResult(email, 1);
    std::cout << "Email: " << email << std::endl;
    db.queryDB(db.prepareStatement("AddressBookFAX", "abook_id", std::string("faxnumber = ") + "'" + "3058610830" + "'","","",SELECT | FROM | WHERE));
    std::string id;
    std::cout << "Database returned results for AddressBookFAX: " << db.hasResults() << std::endl;
    db.getResult(id);
    std::cout << "Entry id: " << id << std::endl;
    query = db.prepareStatement("UserAccount", "email", std::string("modemdevs = ") + "'" + "ttyACM0" + "'","","",SELECT|FROM|WHERE);
    std::cout << "Querying database: " << query << std::endl;
    db.queryDB(query);
    db.getResult(email);
    std::cout << "E-Mail: " << email << std::endl;
    query = db.prepareStatement("AddressBook", "abook_id", std::string("company = ") + "'" + "305" + "'","","",SELECT|FROM|WHERE);
    db.queryDB(query, false);
    for(int i = 0; i < argc; i++)
    {
        std::cerr << argv[i] << std::endl;
    }

    std::cout << replaceStrInStr("Hi Worldi", "i", "") << std::endl;
    std::cin >> res;
//Restore cerr
    if(cerrLog.is_open())
        std::cerr.rdbuf(cerr_copy);

    return 0;
}
/*-C /var/www/avantfax/images/CoverPages/coverpage.html -c test -f kiseitai2 -M kiseitai2@gmail.com -l N/A -n 3058610830 -r N/A -s default -t N/A -v N/A -x N/A -L Miami Beach -N 3058655838 -V 3057334355 -X Aleyda\'s Home Care, Inc. -p 1
*/
