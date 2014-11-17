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
#define ERROR -1
#include "conversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>


//Prototypes
void shiftLeadingExponentZeroes(char* source, size_t index);

//Functions

int charToInt(const char buffer)
{
    /*Base function that converts a single character into a number. This function is the heart of the string conversion functions.*/
    int answer = buffer - '0';
    return answer;
}

std::string intToStr (const int num)
{
    char buffer[33];
    snprintf(buffer, 33, "%d", num);//convert and store in buffer. this function used to be done with itoa but it wasn't standard so I changed it (MinGW vs. Cygwin capabilities).
    return std::string(buffer);
}

int cStrToInt(const char buffer[])
{
    /*Same as cStrToNum but this one assumes the string yields an integer. The code is somewhat shorter and should execute much faster.*/
    return atoi(buffer);
}

double cStrToNum(const char buffer[])
{
    /*Converts a string buffer into its number equivalent. A.k.a. "123" = 123 or "12.3" = 12.3. This function returns a number in floating point
    because its purpose is general.*/
    return atof(buffer);
}

int numToInt(double value)
{
    int extra = 0;
    if(value - (int)value > 0.5)
    {
        extra++;
    }
    return int(value + extra);
}

const int searchCharIndex(char character, char buffer[]){
  /*This function will search for a character and tell me where it exists in the buffer.*/
for(unsigned int i =0; buffer[i] != '\0'; i++)
{
    if(character == buffer[i])
    {
        return i;
    }
}
return ERROR;
}

const int searchCharIndex(const char character, std::string buffer, int start)
{
      /*This function will search for a character and tell me where it exists in the buffer.*/
    for(unsigned int i =start; i < buffer.length(); i++)
    {
        if(character == buffer[i])
        {
            return i;
        }
    }
    return ERROR;
}

bool searchChar(const char character, std::string buffer){
/*This function will search for a character and tell me if it exists.*/
  for(unsigned int i =0; i < buffer.size() -1; i++)
{
    if(character == buffer[i])
    {
        return true;
    }
}
return false;
}

int findString(const char target[], const char stringSource[], int pos)
{
    /*This function searches for a string and returns the index of the first character in the target string.*/
    std::string StringSource = stringSource;
    return StringSource.find(target, pos);
}

void transferStr(pChar* const input, char* const array)
{
    /*This function extracts the characters in the pBuffer field of input and save them in the array from the caller. */
    for(unsigned int i = 0; i < input->size; i++)
    {
        if(i < sizeof(array))
        {
            array[i] = input->pBuffer[i];
        }
    }
}

std::string removeCharFromStr(char target, const char cString[])
{
    std::string piece = cString;
    int index = 0;
    index = piece.find(target);
    if(index >= 0)
    {
        piece.erase(index,1);
    }
    return piece;
}
std::string removeMultipleCharFromStr(char target, const char cString[])
{
    std::string piece = cString;
    int index = 0;
    index = piece.find(target);
    while(index >= 0)
    {
        piece.erase(index,1);
        index = piece.find(target);
    }
    return piece;

}
pChar* createPCharFromBuffer(char buffer[], int size)
{
    /*This function can create a simple pChar structure on the heap from a given C string and size.
    The function checks the heap pointers to make sure they are not null.*/
    pChar* holder = new pChar;
    holder->size = size;
    char* content = new char[holder->size + 1];
    content[holder->size] = '\0';
    if(!content)//Chacks for validity of pointer
    {
        holder->pBuffer = 0;//makes sure the pointer goes to the special address if the validity check fails.
    }
    else if(!holder)
    {
        return 0;
    }
    else
    {
        for(unsigned int i = 0; i<holder->size; i++)
        {
            content[i] = buffer[i];
        }
    }
    holder->pBuffer = content;
    return holder;
}
pChar* slice(char* const input, int start, int end)
{
    /*Slices a c-string and returns the slice. This emulates the behavior of the python slice.
    Pretty handy for string manipulation and extraction. */
    char* answer = new char[end - start + 1];
    answer[end - start] = '0';
    pChar* holder = new pChar;
    holder->size = end - start;
    for(int i = 0; i < (end - start); i++)
    {
        answer[i] = input[i + start];
    }
    holder->pBuffer = answer;
    return holder;
}

std::string sliceStr(std::string input, int start, int end)
{
    if(input == "")
    {
      return "";
    }
    /*This function takes a string and returns a slice from the start position to the desired end position*/
    char answer[end - start + 1];
    answer[end-start] = '\0'; //Append end of string character so the string is not misdisplayed or misused later on.
    if(end < 0)
    {
        return input;
    }
    for(int i = 0; i<end-start; i++)
    {
        answer[i] = input[i+start];
    }
    return std::string(answer);
}
pChar* shiftArrayLeft(char* input, int size)
{
    /*Default version of this function. It returns a slice of the original c-string that does
    not contain the first element of the array.*/
    pChar* holder = slice(input, 1, size);
    if(!holder)
    {
        return NULL;
    }
    return holder;
}

pChar* shiftArrayLeft(char* input, int size, int elements)
{
    /*Overload version of this function. It returns a slice of the original c-string that does
    not contain the number of elements (elements) in the array. For example, "ABCDE" => "DE" if elements = 3*/
    pChar* holder = slice(input, elements, size);
    if(!holder)
    {
        return NULL;
    }
    return holder;
}

std::string fuseStrs(std::string Str1, std::string Str2)
{
    return Str1 + Str2;
}

double round(double value, int precision)
{
    int precisionFactor = 10 ^ precision;
    double tmpValue = value * precisionFactor;
    double decimals = tmpValue - (int)(tmpValue);
    if(decimals > 0.5)
    {
        return ceil(tmpValue) / precisionFactor;
    }
    return floor(tmpValue) / precisionFactor;
}

int iround(double value, int precision)
{
    return (int)(round(value, precision));
}

std::string numToStr(const double num)
{
    std::stringstream s;
    s << num;
    return s.str();
}

bool changeProgramWorkingDirectory(char* newPath)
{
    //This function allows the calling program to change the current working directory
    if(chdir(newPath) == ERROR)
    {
        std::cout <<"The program tried to change to this directory, but chdir returned an error!" << std::endl;
        return false;
    }
    return true;
}

char capitalizeChar(const char character)
{
  /*This function takes a character and returns the upper case version. This
   *conversion is based on the ASCII table. Because it uses the symmetry of the
   *table, you can actually input other characters and receive not the upper case,
   *but another character (example, [ -> ;). Warning: it shouldn't work with
   *other character standards unless they share the same symmetry as the ASCII
   *table.
   **/
  return character - ('a' - 'A');
}

char lowerCaseChar(const char character)
{
  /*This function takes a character and returns the lower case version. This
   *conversion is based on the ASCII table.
   **/
  return character + ('a' - 'A');
}

std::string capitalizeStr(const std::string& source)
{
    std::string newStr= "";
    if(source[0] >= 'a' && source[0] <= 'z')
    {
        newStr = capitalizeChar(source[0]);
    }
    return newStr + source.substr(1, source.length());
}

char* getCharArrayFromConstArray(const std::string& s)
{
    char* tmp = new char(s.length());
    for(size_t i = 0; i < s.length(); i++)
        tmp[i] = s[i];
    return tmp;
}

std::string scientificFormat(const std::string& num, size_t precision)
{
    /* This function takes a string type of a floating point number and creates the scientific notation version of it!
    This was my own function and contains a bug I don't seem to be able to fix. Check out the overloaded version until
    the bug is fixed. */
    size_t dotLoc = num.find('.');
    size_t start = 0;
    size_t offset =  0;
    int exponent = 0;
    std::string tmp = "";
    //If the number is too small to begin with, return the unedited string!
    if(num.length() <= precision)
        return num;
    //Check wether index 0 is the negative sign or the actual start.
    if(num[start] == '-')
            start++;
    //If the number is too small to begin with, return the unedited string!
    if(cStrToNum((num.c_str())) < 0 && dotLoc != std::string::npos)
        if(num.length() - 2 <= precision && num[0] != '0')
            return num;
    //If the number is already in a scientific notation.
    if(num.find('e') != std::string::npos)
    {
        size_t end = 0;
        size_t exponentLoc = 0;
        size_t exponentSign = 0;

        //Obtain the exponent location, which is the end of the number part.
        exponentLoc = num.find('e');
        end = exponentLoc;
        exponentSign = exponentLoc + 1;
        //Identify the index of the sign of the exponent, if any.
        if(num[exponentSign] != '-' && num[exponentSign] != '+')
        {
            exponentSign = std::string::npos;
            exponentLoc++;
        }
        else
            exponentLoc += 2;
        //Move our exponent index to the part where the exponent starts with a non-zero value.
        while(num[exponentLoc] == '0')
        {
            exponentLoc++;
        }
        //If the number part is smaller than the requested precision, do not modify the number string
        if(end - start <= precision)
            return num;
        //If start is not 0, append - sign
        if(start)
            tmp += '-';
        //Add the number part until we hit the pecision level. Discard the rest!
        for(size_t i = start; i <= start + precision; i++)
            tmp += num[i];
        //Append the exponent char
        tmp += 'e';
        //Append sign if available.
        if(exponentSign != std::string::npos)
            tmp += num[exponentSign];
        //Append the exponent!
        for(size_t i = exponentLoc; i < num.length(); i++)
            tmp += num[i];
        //Return the new scientific notation value.
        return tmp;
    }
    //I we have a fully raw number.
    if(cStrToNum((num.c_str())) < 1 && cStrToNum((num.c_str())) > -1)//Determine if it's a 0.001 kind of decimal
    {
        //Obtain a negative exponent
        for(size_t i = dotLoc + 1; i < num.length(); i++)
        {
            exponent--;
            if(num[i] != '0')
                break;
        }
        //Build a string fragment that has the x.xx value of our scientific notation
        for(size_t i = 0; i < precision && i < num.length(); i++)
        {
            if(i == 1 && tmp.find('.') >= num.length())
            {
                tmp += '.';
                i--;
                continue;
            }
            tmp += num[dotLoc + abs(exponent) + i];
        }
    }
    else
    {
        //Obtain a positive exponent
        for(size_t i = dotLoc; i > 1; i--)
            exponent++;
        //Build a string fragment that has the x.xx value of our scientific notation
        for(size_t i = start; i < precision + offset && i < num.length(); i++)
        {
            if(i == 1 && tmp.find('.') == std::string::npos)
            {
                tmp += '.';
                i--;
                continue;
            }
            if(num[i] == '.')
            {
                offset++;
                continue;
            }
            tmp += num[i];
        }
    }

    //Adjust the sign
    if(cStrToNum((num.c_str())) < 0)
        tmp = '-' + tmp;
    //For some bizzarre reason, the final string may randomly contain spaces so let's make sure the string is clean!
    //These spaces and nul characters disrupt the proper addition of the exponent component of a properly formatted scientific notation string!
    tmp = removeMultipleCharFromStr(' ', tmp.c_str());
    for(size_t i = 0; i < tmp.length(); i++ )
    {
        if(tmp[i] == 'e' || tmp[i] == '-' || tmp[i] == '.')
            continue;
        if(tmp[i] < '0' || tmp[i] > '9')
            tmp = removeMultipleCharFromStr(tmp[i], tmp.c_str());
    }
    //Add the exponent char
    if(exponent != 0)
    {
        tmp += 'e';
        tmp += intToStr(exponent);
    }
    //Return the fully formed string
    return tmp;
}

std::string scientificFormat(float num, size_t precision)
{
    /* Overloaded version of my scientific notation function. This one uses the standard c library!*/
    size_t size = 10;
    size_t exponentStart = 0;
    char buff[size];
    std::string formatPrecision = "%." + numToStr(precision) + "e";//Build the string format for the number
    /*if(numToStr(num).length() > precision)
    {
        //Create the scientific notation string*/
        sprintf(buff, formatPrecision.c_str(), num);
       /* //Grab the index of either - or + after e (in e+-00x)
        if(searchCharIndex('-', buff, searchCharIndex('e', buff)) != ERROR)
            exponentStart = searchCharIndex('-', buff, searchCharIndex('e', buff));
        else if(searchCharIndex('+', buff, searchCharIndex('e', buff)) != ERROR)
            exponentStart = searchCharIndex('+', buff, searchCharIndex('e', buff));
        //Get rid of all leading zeroes
        while(buff[exponentStart + 1] == '0')
            shiftLeadingExponentZeroes(buff, exponentStart + 1);
        return buff;
    }
    return numToStr(num);*/
    return buff;
}

void shiftLeadingExponentZeroes(char* source, size_t index)
{
    /* This function shifts a c string with leading 0's such that you get rid of the leading 0's. Example,
    "0001" -> "1".
    */
    if(source[index] == '\0')//Protection from misuse!
        return;

    if(source[index + 1] == '\0')//Base case
    {
        source[index] = source[index + 1];
        return;
    }

    source[index] = source[index + 1];//Keep shifting until we hit the base case
    shiftLeadingExponentZeroes(source, index + 1);
}

std::string replaceCharInStr(std::string source, char target, char replacement, bool allInstances)
{
    size_t i = 0;
    if(allInstances)
    {
        while(i < source.length())
        {
            i = source.find(target, i);
            if(i < source.length())
                //Macs are extremely picky so this second check prevents a loophole in my logic and thus the crash. Windows and Linux are more flexible in this regard. Linux optimizes out std::string::npos, so you can't use that for cross-platform checks!
                source[i] = replacement;
        }
    }
    else
    {
        i = source.find(target, i);
        source[i] = replacement;
    }
    return source;
}

bool isNum(std::string& strNum)
{
    std::string nums = "0123456789e-+.^";
    char c;
    size_t correctCount = 0;
    for(size_t i = 0; i < strNum.size(); i++)
    {
        c = strNum[i];
        for(size_t j = 0; j < nums.size(); j++)
        {
            if(strNum[i] == nums[j])
            {
                correctCount++;
                break;
            }
        }
    }

    return correctCount == strNum.size();
}

#ifdef UTF8_NEEDED
bool checkUTF8String(const std::string& buffer)
{
    return utf8::is_valid(buffer.begin(), buffer.end());
}

std::string convertASCII2UTF8(const std::string& ascii)
{
    //create temporary buffer
    std::string tmp;
    //Ask utfcpp to look for invalid utf8 portions and replace them with utf8!
    utf8::replace_invalid(ascii.begin(), ascii.end(), back_inserter(tmp));
    //Return the newly formed string
    return tmp;
}

std::string convertUTF82UTF16(const std::string& utf8)
{
    //create temporary buffer
    std::string tmp;
    //Ask utfcpp to convert the utf8 string into a utf16 string!
    utf8::utf8to16(utf8.begin(), utf8.end(), back_inserter(tmp));
    //Return the newly formed string
    return tmp;
}

std::string convertUTF162UTF8(const std::string& utf16)
{
    //create temporary buffer
    std::string tmp;
    //Ask utfcpp to convert the utf16 string into a utf8 string!
    utf8::utf16to8(utf16.begin(), utf16.end(), back_inserter(tmp));
    //Return the newly formed string
    return tmp;
}

std::string convertUTF82UTF32(const std::string& utf8)
{
    //create temporary buffer
    std::string tmp;
    //Ask utfcpp to convert the utf8 string into a utf16 string!
    utf8::utf8to32(utf8.begin(), utf8.end(), back_inserter(tmp));
    //Return the newly formed string
    return tmp;
}

std::string convertUTF322UTF8(const std::string& utf32)
{
    //create temporary buffer
    std::string tmp;
    //Ask utfcpp to convert the utf16 string into a utf8 string!
    utf8::utf32to8(utf32.begin(), utf32.end(), back_inserter(tmp));
    //Return the newly formed string
    return tmp;
}
#endif // UTF8_NEEDED
