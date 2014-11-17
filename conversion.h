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
#ifndef CONVERSION_H_INCLUDED
#define CONVERSION_H_INCLUDED
#include <string>
#include "config.h"

#ifdef UTF8_NEEDED
#warning "Notice, if you want to access the utf8 conversion functions, you are required to add the utfcpp library/headers"
#warning "to your project and define the macro UTF8_NEEDED in the included config.h header! You may find the library at"
#warning "http://utfcpp.sourceforge.net/"

#include "utf8.h"

#define convertASCIItoUTF8 convertASCII2UTF8
#define convertUTF8toUTF16 convertUTF82UTF16
#define convertUTF16toUTF8 convertUTF162UTF8
#define convertUTF8toUTF32 convertUTF82UTF32
#define convertUTF32toUTF8 convertUTF322UTF8
#endif

struct pChar{
char* pBuffer;//Contains the pointer to the character array
unsigned int size;//Contains the size of the array
};
int charToInt(const char buffer);//Get numbers out of chars
int cStrToInt(const char buffer[]);//Get full numbers out of character strings
std::string intToStr (const int num); //Converts a single number into a string equivalent
double cStrToNum(const char buffer[]);
std::string numToStr(const double num);
const int searchCharIndex(const char character, std::string buffer, int start = 0);
bool searchChar(const char character, std::string buffer);
int findString(const char target[], const char stringSource[], int pos = 0);
pChar* slice(char* const input, int start, int end);
pChar* shiftArrayLeft(char* const input, int size);
pChar* shiftArrayLeft(char* const input, int size, int elements);
void transferStr(pChar* const input, char* const array);
pChar* createPCharFromBuffer(char buffer[], int size);
std::string removeCharFromStr(char target, const char cString[]);
std::string removeMultipleCharFromStr(char target, const char cString[]);
const int searchCharIndex(char character, char buffer[]);
std::string sliceStr(std::string input, int start, int end);
int numToInt(double value);
std::string fuseStrs(std::string Str1, std::string Str2);
std::string capitalizeStr(const std::string& source);
std::string replaceCharInStr(std::string source, char target, char replacement, bool allInstances = true);
char* getCharArrayFromConstArray(const std::string& s);
double round(double value, int precision);
int iround(double value, int precision);
bool changeProgramWorkingDirectory(char* newPath);
char capitalizeChar(const char character);
char lowerCaseChar(const char character);
std::string scientificFormat(const std::string& num, size_t precision = 3);
std::string scientificFormat(float num, size_t precision = 3);
bool isNum(std::string& strNum);

#ifdef UTF8_NEEDED
//UTF based conversion functions. They rely on the library utfcpp!
bool checkUTF8String(const std::string& buffer);
std::string convertASCII2UTF8(const std::string& ascii);
std::string convertUTF82UTF16(const std::string& utf8);
std::string convertUTF162UTF8(const std::string& utf16);
std::string convertUTF82UTF32(const std::string& utf8);
std::string convertUTF322UTF8(const std::string& utf32);
#endif

//Templated function
template <typename N> N compareValues(N numA, N numB)
{
    if(numA > numB)
    {
        return numA;
    }
    return numB;
}

template <typename T> T reverseStackOrder(const T& in)
{
    /*This function will reverse the order of an stack. It assumes you are not feeding a pointer to an stack!
    */
    T out;
    for(unsigned int i = 0; i < in.size(); i++)
    {
        out.push(in.top());
    }
    return out;
}
#endif // CONVERSION_H_INCLUDED
