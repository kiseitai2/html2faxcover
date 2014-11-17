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
#ifndef MYSQL_H_INCLUDED
#define MYSQL_H_INCLUDED

#include <iostream>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "sqlkeywords.h"


#define SQL_COMA ","
#define SQL_NULL "NULL"
#define SQL_ZERO "0"
#define SQL_NONE "None"

class MySQL : public SQLGenerator
{
public:
    //Typedefs and static vars
    typedef const char* cstr;
    static cstr DEFAULT_HOST;
    static const int DEFAULT_COL_INDEX;
    //Ctors
    MySQL(cstr database = NULL, cstr username = NULL, cstr password = NULL, cstr host = MySQL::DEFAULT_HOST);
    //Main interface
    void connect(cstr database, cstr username, cstr password, cstr host);
    void disconnect();
    void queryDB(const std::string& query);
    void CleanResults();
    bool hasResults();
    size_t rowCount();
    /*Results getters*/
    void getResult(int& response, const std::string& col_name);
    void getResult(char& response, const std::string& col_name);
    void getResult(std::string& response, const std::string& col_name);
    void getResult(bool& response, const std::string& col_name);
    void getResult(double& response, const std::string& col_name);
    void getResult(int& response, size_t col_index = DEFAULT_COL_INDEX);
    void getResult(char& response, size_t col_index = DEFAULT_COL_INDEX);
    void getResult(std::string& response, size_t col_index = DEFAULT_COL_INDEX);
    void getResult(bool& response, size_t col_index = DEFAULT_COL_INDEX);
    void getResult(double& response, size_t col_index = DEFAULT_COL_INDEX);
    bool getStatus() const;
    //Dtors
    ~MySQL();
private:
    bool status;
    sql::Connection* conn;
    sql::Driver* driver;
    sql::ResultSet* res;
    sql::Statement* statement;

    //methods
    void error_log(sql::SQLException& err);
};

#endif // MYSQL_H_INCLUDED
