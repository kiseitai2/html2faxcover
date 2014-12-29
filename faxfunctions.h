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
#ifndef FAXFUNCTIONS_H_INCLUDED
#define FAXFUNCTIONS_H_INCLUDED
#include <string>
#include "mysql.h"
namespace h2fax
{
//Structs
typedef struct time_info
{
    std::string Year;
    std::string Month;
    std::string Day;
    std::string Hour;
    std::string Minute;
    std::string Second;
} faxtime;

typedef struct faxheader
{
    std::string Sender;
    std::string Pages;
    std::string Quality;
    std::string Page;
    std::string Received;
    std::string TimeToRecv;
    std::string SignalRate;
    std::string DataFormat;
    std::string ErrCorrect;
    std::string CallID1;
    std::string CallID2;
    std::string CallID3;
    faxtime docTime;
    void extractTime();
} fax;


//Functions
fax faxinfo(const std::string& fileinfo_prog, const std::string& tiff_path);
std::string strip_sipinfo(std::string dirtyStr);
void logMsg(const std::string& msg);
std::string create_preview(const std::string& dir, const std::string& pdf_name, std::string& thumb_name, const std::string& convert_cmd, size_t pages = 1);

}

#endif // FAXFUNCTIONS_H_INCLUDED
