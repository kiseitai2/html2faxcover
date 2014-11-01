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
#include "mysql.h"
#include "data_base.h"

int main()
{
    int res;


    /*Database test*/
    h2fax::cstr database = "avantfax_test\0";
    h2fax::cstr host = "localhost\0";
    h2fax::cstr password = "\0";//Add your own MySQL/WAMP/LAMP account info
    h2fax::cstr username = "\0";

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
    std::cin >> res;
    return 0;
}
