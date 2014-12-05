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
#ifndef MARKER_PARSER_H_INCLUDED
#define MARKER_PARSER_H_INCLUDED

#include <string>
#include "typedefs.h"
#include "data_base.h"
#include "cmd_processor.h"
#include "default.h"


namespace h2fax
{
<<<<<<< HEAD
void removeAvantFaxPrefix(cstr filepath, const std::string& prefix);
cstr copyFile(cstr source, cstr destination);
cstr copyDir(cstr source, cstr destination);
cstr removeDir(cstr target);
void convertToPS(cstr filepath, const std::string& html2ps, const std::string& options);
void exec_cmd(cstr filepath, const std::string& cmd, const std::string& options);
void substituteMarkers(cstr filepath, faxcover_args& data);
std::string getDate();
=======
    void removeAvantFaxPrefix(cstr filepath, const std::string& prefix);
    cstr copyFile(cstr source, cstr destination);
    cstr copyDir(cstr source, cstr destination);
    cstr removeDir(cstr target);
    void convertToPS(cstr filepath, const std::string& html2ps, const std::string& options);
    void exec_cmd(cstr filepath, const std::string& cmd, const std::string& options);
    void substituteMarkers(cstr filepath, faxcover_args& data);
    std::string getDate();
>>>>>>> origin/v1.1_debug
}



#endif // MARKER_PARSER_H_INCLUDED
