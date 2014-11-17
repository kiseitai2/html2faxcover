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
#include "mysql.h"


MySQL::cstr MySQL::DEFAULT_HOST = "localhost";
const int MySQL::DEFAULT_COL_INDEX = 1;

MySQL::MySQL(cstr database, cstr username, cstr password, cstr host)
{
    conn = NULL;
    driver = NULL;
    statement = NULL;
    res = NULL;
    if(!database || !username || !password)
        std::cerr << "Warning: One or more pieces of information is missing. The program can't connect to the database without its name, "
            << "username, or password! Optionally, a host name should be specified for remote databases! Please, make sure this "
            << "information is present!" << std::endl;
    else
        connect(database, username, password, host);

    status = false;//False = everything is fine
}

void MySQL::connect(cstr database, cstr username, cstr password, cstr host)
{
    try
    {
        /*Create a connection to MySQL! */
        driver = get_driver_instance();//Init driver
        conn = driver->connect(host, username, password);//connect to MySQL
        /*Connect to specific database within MySQL server.*/
        conn->setSchema(database);
    }
    catch(sql::SQLException& e)
    {
        error_log(e);
    }
}

void MySQL::disconnect()
{
    try
    {
        if(res)
            delete res;
        if(statement)
            delete statement;
        delete conn;
        conn = NULL;
        res = NULL;
        statement = NULL;
    }
    catch(sql::SQLException& e)
    {
        error_log(e);
    }
}

void MySQL::queryDB(const std::string& query)
{
    try
    {
        /*Let's check if a previous instance of statement exists*/
        if(statement)
            delete statement;//Clean it in preparation for the new query!
        statement = NULL;
        /*Let's set up an instance of statement!*/
        statement = conn->createStatement();
        //Reset results
        CleanResults();
        //Execute query and get results!
        res = statement->executeQuery(query);
    }
    catch(sql::SQLException& e)
    {
        error_log(e);
    }
}

void MySQL::CleanResults()
{
    try
    {
        if(res)
            delete res;
    }
    catch(sql::SQLException& e)
    {
        error_log(e);
    }
}

bool MySQL::hasResults()
{
    return res->rowsCount();
}

size_t MySQL::rowCount()
{
    return res->rowsCount();
}

void MySQL::error_log(sql::SQLException& err)
{
    std::cerr << "# ERR: SQLException in " << __FILE__;
    std::cerr << "(" << __FUNCTION__ << ") on line "
    << __LINE__ << std::endl;
    std::cerr << "# ERR: " << err.what();
    std::cerr << " (MySQL error code: " << err.getErrorCode();
    std::cerr << ", SQLState: " << err.getSQLState() << " )" << std::endl;
    //Raise the status flag!
    status = true; //Things are bad!
}

bool MySQL::getStatus() const
{
    return status;
}

void MySQL::getResult(bool& response, const std::string& col_name)
{
    bool tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getBoolean(col_name);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }
    response = tmp;
}

void MySQL::getResult(int& response, const std::string& col_name)
{
    int tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getInt(col_name);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(std::string& response, const std::string& col_name)
{
    std::string tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getString(col_name);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(char& response, const std::string& col_name)
{
    char tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getString(col_name)[0];
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(double& response, const std::string& col_name)
{
    double tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getDouble(col_name);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(bool& response, size_t col_index)
{
    bool tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getBoolean(col_index);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }
    response = tmp;
}

void MySQL::getResult(int& response, size_t col_index)
{
    int tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getInt(col_index);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(std::string& response, size_t col_index)
{
    std::string tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getString(col_index);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(char& response, size_t col_index)
{
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
    char tmp;
    try
    {
        tmp = res->getString(col_index)[0];
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

void MySQL::getResult(double& response, size_t col_index)
{
    double tmp;
    try
    {
        if(!res->next())
        {
            std::cerr << "Error: Either the resultset is empty or something is wrong with the query string!"
            << " No results were obtained from the previous query!" << std::endl;
            return;
        }
        tmp = res->getDouble(col_index);
    }
    catch (sql::SQLException& e)
    {
        error_log(e);
    }

    response = tmp;
}

MySQL::~MySQL()
{
    disconnect();
    CleanResults();
    if(conn)
        delete conn;

}
