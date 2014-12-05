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
#ifndef DEFAULT_H_INCLUDED
#define DEFAULT_H_INCLUDED

namespace h2fax
{
static cstr DEFAULT_ENGINE = "MySQL";
static cstr DEFAULT_USERNAME = "html2faxcover";
static cstr DEFAULT_PASSWORD = "pepito2014";
static cstr DEFAULT_DATABASE = "avantfax";
static cstr DEFAULT_HOST = "localhost";

const std::string MARKERS[] = {"to", "from", "to-company", "from-company", "to-location", "from-location",
                               "from-voice-number", "voice-number", "from-fax-number", "to-fax-number", "from-mail-address", "regarding",
                               "comments", "page-count", "todays-date",
                              };
const byte MARKERSNUM = 15;
}



#endif // DEFAULT_H_INCLUDED
