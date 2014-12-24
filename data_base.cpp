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
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include "conversion.h"
#include "data_base.h"

//Reading section
    bool data_base::LoadData(const char* location, bool readMode)
    {
        /*Opens the file handle*/
        if(readMode)
        {
            file.open(location, std::fstream::in);
            lastLocation = location;
        }
        else
        {
            output.open(location, std::ios::out | std::ios::trunc);
            lastLocation = location;
        }
        return file.is_open();
    }
void data_base::LoadStringBuffer(bool closeFile)
{
    /*Loads file into internal string buffer line by line*/
    /*std::string temp = "";
    if (file.is_open())
    {
        while ( file.good() )
        {
            std::getline (file,temp);
            temp += "\n\r";
            buffer += temp;
            lines++;
        }
    }*/
    /*Above solution is depracated because of a bug. I'm currently using the
     *steps below!
     */
    file.seekg(0, std::ios::end);
    s = file.tellg();//The +1 to the size is to add the nul terminating character.
    char* buff = new char[s];
    file.seekg(0, std::ios::beg);
    file.read(buff, s);
    buffer = buff;//Load the string to the internal buffer
    delete[] buff;// free the temporary buffer
    for(size_t i = 0; i <buffer.size(); i++)
    {
        /*Obtain line number for the functions that need it!*/
        if(buffer[i] == '\n')
        {
            lines++;
        }
    }
    lines++;
    if(closeFile)
    {
        file.close();
    }
}
    std::string data_base::GetStrBuffer() const
    {
        return buffer;
    }
    double data_base::GetValueFromData(const std::string& search) const
    {
        /*This function takes a search string (aka. keyword) and looks for the index of the beginning of the keyword.
        Then, it slices the internal buffer and cleans it of non-numeric characters that are known to be in the internal string buffer.
        Finally, it sends the number string into a conversion function that returns the number equivalent of the string.*/
        int start = findString((char*)search.c_str(), buffer.c_str())+ search.length();
        int end = searchCharIndex(';', buffer, start);
        if(end < 0)
        {
            end = searchCharIndex('\n', buffer.c_str(), start);
        }
        std::string numberStr = sliceStr(buffer,start, end);//Get slice
        numberStr = removeCharFromStr('=', numberStr.c_str());
        return cStrToNum(numberStr.c_str());
    }
    std::string data_base::GetStrFromData(const std::string& search) const
    {
        /* Extracts the string in much the same way the GetValueFromData does.*/
        int start = findString((char*)search.c_str(), buffer.c_str())+ search.length();
        int end = searchCharIndex(';', buffer, start);//Get's end of line
        if(end < 0)
        {
            end = searchCharIndex('\n', buffer.c_str(), start);
        }
        std::string Str = sliceStr(buffer,start, end);
        Str = removeCharFromStr('=', Str.c_str());//Cleans value string from the = sign
        while(Str[0] == ' ')//remover initial spaces left after removing '='.
        {
            Str = removeCharFromStr(' ', Str.c_str());
        }
        return Str;
    }
    int data_base::GetIntFromData(const std::string& search) const
    {
        return numToInt(GetValueFromData(search));
    }
    void data_base::OpenFile(const char* location, bool readMode)
    {
        switch(readMode)
        {
            case true:
            if(file.is_open())
            {
                file.close();
            }
            if(LoadData(location))//Checks if internal file buffer was properly started.
            {
                LoadStringBuffer();//reads the file buffer into the internal string buffer
                isBufferLoaded = true;//sets this flag true so other processes know the task was successful!
            }
            break;
            default:
            if(LoadData(location))//Checks if internal file buffer was properly started.
            {
                writeMode = true;
                LoadStringBuffer();//Get an internal copy of the contents in the file and close the file
                LoadData(location, readMode);//Reopen the same file but in output mode, which erases all previous data.
            }
        }

    }

    void data_base::OpenFileForQuickWrite(const char *location)
    {
        /*Will open the output stream in append mode. It assumes you just want to ad some new stuff to the file immediately.*/
        if(output.is_open())//we don't want to leak handles or anything
        {
            output.close();
        }
        output.open(location, std::ios::out | std::ios::app);
        buffer = "";
    }
  void data_base::OpenBinFileForQuickWrite(const char* location)
    {
        /*Will open the output stream in bin mode. It assumes you just want to ad some new stuff to the file immediately.*/
        if(output.is_open())//we don't want to leak handles or anything
        {
            output.close();
        }
        output.open(location, std::ios::out | std::ios::binary);
        buffer = "";
    }

     std::string data_base::OpenFileAndGetBinBuffer(char* location)
{
    if(file.is_open())
    {
        file.close();
    }
    file.open(location, std::ios::in | std::ios::binary);
    file.seekg(0);
    std::cout << file.tellg();
    std::string line, buff;
    while(std::getline(file, line))
    {
        buff+=line + "\n";
    }
    file.close();
    return buff;
}
   double data_base::GetValueFromDataWithLine(const std::string& search, int instanceIndex) const
    {
         /*Note: instanceIndex is the number of times a search word can be found in the target file. i.e. value = 1; value = 3;
        has 2 instances, so if inctanceIndex is 1 the result will be 1 and if instanceIndex is 2 the result will be 3.
        This method gets the value from an specific instance of a keyword.*/
        int start = GetLineIndex(search, instanceIndex);
        int end = searchCharIndex(';', buffer, start);
        if(end == ENDOFFILE)
        {
            end = searchCharIndex('\n', buffer, start);
        }
        std::string numberStr = sliceStr(buffer,start, end);//Get slice
        numberStr = removeCharFromStr('=', numberStr.c_str());
        return cStrToNum(numberStr.c_str());
    }

    std::string data_base::GetStrFromDataWithLine(const std::string& search, int instanceIndex) const
    {
        /*Note: instanceIndex is the number of times a search word can be found in the target file. i.e. value = 1; value = 3;
        has 2 instances, so if inctanceIndex is 1 the result will be 1 and if instanceIndex is 2 the result will be 3.
        This method gets the value from an specific instance of a keyword.*/
        int start = GetLineIndex(search, instanceIndex);
        int end = searchCharIndex(';', buffer, start);
        if(end == ENDOFFILE)
        {
            end = searchCharIndex('\n', buffer, start);
        }
        std::string Str = sliceStr(buffer,start, end);
        Str = removeCharFromStr('=', Str.c_str());//Cleans value string from the = sign
        while(Str[0] == ' ')//remover initial spaces left after removing '='.
        {
            Str = removeCharFromStr(' ', Str.c_str());
        }
        return Str;
    }

bool data_base::SearchTermExists(const std::string& search) const
{
    if(static_cast<size_t>(findString(search.c_str(), GetStrBuffer().c_str())) == std::string::npos)
    {
        return false;
    }
    return true;
}
    //Writing
    void data_base::FlushData()
    {
        FileClear();
        output << buffer;//write buffer into file stream
        output.flush();//flush the stream into the storage device so that the file is updated
    }

    void data_base::WriteValue(const std::string& value, const std::string& search)
    {
        /*Writes value into an specific 'tag' in file. i.e. value = 3; [search = value, value = 5] -> value = 5;*/
        if(buffer != "" && search != "")
        {
            int start = findString(search.c_str(), buffer.c_str()) + search.length() + 3;//3 is the number of spaces in the line that form the string " = ". Check the syntax of files
            int end = searchCharIndex(';', buffer, start);
            int size = end - start;
            buffer.replace(start, size, value);
            FlushData();
        }
        else if(buffer == "" && search != "")
        {
            std::cout<<"Error: File is empty!\n\r";
        }
        else
        {
            buffer = value;
            FlushData();
        }
    }

    void data_base::WriteValueWithLineIndex(const std::string& value, const std::string& search, int instanceIndex)
    {
        /*This methods find the instance of a keyword and modifies its value. i.e. value = 3;\nvalue = 4 [mod line 2 with 3] -> value = 3;\nvalue = 3.*/
        int start = GetLineIndex(search, instanceIndex) + 3;
        int end = searchCharIndex(';', buffer, start);
        int size = end - start;
        if(end == ENDOFFILE)
        {
            end = searchCharIndex('\n', buffer, start);
        }
        if(buffer != "")
        {
            buffer.replace(start, size, value);
            FlushData();
        }
    }

    void data_base::WriteValueAndFlush(const std::string& value)
    {
        /*Meant to be used with OpenFileForQuickWrite(). Unlike in the regular output mode, value will contain a manually
        created string. In other words, this function will not edit search terms within the file.*/
        output << value;
        output.flush();
    }

    //General methods
    int data_base::GetLineCount() const
    {
        return lines;
    }
    int data_base::GetNumInstances(const std::string& search) const
    {
        /*Searches for the search keyword until and increments the instance count (line count) each time it finds the search word.
        Then, the method returns this number.*/
        int index = 0;
        int line = 0;
        int start = 0;
        while(true)
        {
            start = findString(search.c_str(), buffer.c_str(), index) + search.length();
            line++;
            if(index < start)
            {
                index = start;
            }
            else
            {
                return line - 1;//Unfortunately, this function also includes the end of string as an instance of the keyword. I adjust for this false positive by substracting 1.
            }
        }
    }
    bool data_base::GetStateOfInternalBuffer() const
    {
        return isBufferLoaded;
    }
    int data_base::GetLineIndex(const std::string& search, int lineNum) const
    {
        int index = 0;
        int line = 0;
        int start = 0;
        while(true)
        {
            start = findString(search.c_str(), buffer.c_str(), index) + search.length();
            line++;
            index = start;
            if(line == lineNum)
            {
                return index;
            }
        }

    }

    void data_base::FileClear()
    {
        /*Clears the file. WARNING: do not invoke this method if you are reading a file and the internal buffer does not have a copy of the file.
        This will surely empty the file!!!!!!!!!!!!!!!!!!!*/
        if(GetMode())//Despite warnings, I still check the class is in writing mode. I know someone may misuse this class.
        {
            if(output.is_open())
            {
                output.close();
            }
            output.open(lastLocation.c_str(), std::ios::out | std::ios::trunc);
        }
    }

    bool data_base::GetMode() const
    {
        return writeMode;
    }

    std::string data_base::GetLastOutput() const
    {
        return lastLocation;
    }

    bool data_base::isOutputOpen() const
    {
        if(output)
        {
            return output.is_open();
        }
        return false;
    }

    void data_base::CloseFile(const std::string& streamsToClose)
    {
        /*streamsToClose flags are all for all stream, in for input stream, and out for output stream. the flag is set to all by default.
        This method makes some checks*/
        writeMode = false;
        isBufferLoaded = false;
        lines = 0;
        if(streamsToClose == "all" && (file.is_open() && output.is_open()))
        {
            file.close();
            output.close();
        }
        else if(streamsToClose == "all" && (file.is_open()&& !output.is_open()))
        {
            file.close();
        }
        else if(streamsToClose == "all" && (!file.is_open()&& output.is_open()))
        {
            output.close();
        }
        else if(streamsToClose == "in" && file.is_open())
        {
            file.close();
        }
        else if(streamsToClose == "out" && output.is_open())
        {
            output.close();
        }
    }

    void data_base::RestoreFileContents()
    {
      /*This method should be called immediately after opening a file in write mode
       *so the contents are kept in the file for other processes that may be using it.
       */
      CleanFileContentsOfArtifacts();
      TrimEndOfFile();
      FlushData();
    }

    void data_base::RefreshFile()
    {
      /*This method should be called periodically after opening a file in read mode.
       *The purpose of this method is to reload the file contents to the internal
       *buffer when the file is expected to change.
       **/
      CloseFile();
      OpenFile(lastLocation.c_str());
    }

    void data_base::CleanFileContentsOfArtifacts()
    {
      /*This method will erase all excess of newline and return characters!
       *I noticed that the way I have this class set up introduces excess
       *of these characters when the file is repeatedly opened in write
       *mode. I term this excess artifacts.
       **/
      unsigned short newLineInstances = 0;
      unsigned short returnInstances = 0;
      for(size_t i = 0; i < buffer.size(); i++)
      {
         if(buffer[i] == '\n')
          {
            newLineInstances++;
           }
         if(buffer[i] == '\r')
          {
            returnInstances++;
           }
         if(buffer[i] != '\n' && newLineInstances > 0)
           {
             newLineInstances--;
            }
          if(buffer[i] != '\r' && returnInstances > 0)
            {
              returnInstances--;
             }
          if(newLineInstances > 1)
           {
             buffer.replace(i, 1, "");
             newLineInstances--;
            }
          if(returnInstances > 1)
           {
             buffer.replace(i, 1, "");
              returnInstances--;
            }
      }
    }

    void data_base::TrimEndOfFile()
    {
      /*The generated files receive an excess chunk after it is opened in write mode
       *multiple times. This makes the file progressively larger with crap.
       */
      size_t lastMarker = 0;
      for(size_t i = 0; i < buffer.size(); i++)
        {
          if(buffer[i] == ';' || buffer[i] == '\n')
            {
              lastMarker = i;
            }
          if(buffer[i] == 26)
            {
              lastMarker = i;
              break;//Break a leg! We just reached EOF
            }
        }
      if(findString(":EOF:", buffer.c_str()) > 0)
        {
          //Optional way to end the file. Takes priority over the other markers.
          lastMarker = (size_t)findString(":EOF:", buffer.c_str()) - 1;
        }
      buffer = sliceStr(buffer, 0, lastMarker + 1);
    }

    void data_base::CreateNewFile(const char* location)
    {
      /*This method empties the file if location = "" (you are assuming the file was already opened; see FileClear for
       *more information), or creates an empty file in the designated location.
       **/
      if(location == "")
        {
          FileClear();
        }
      else
        {
          lastLocation = location;
          output.open(location, std::ios::out | std::ios::trunc);
          FileClear();//Make sure I make a new file.
        }
      //Let's close the file since we only want to create a new file.
      if(output.is_open())
        {
            output.close();
        }
    }

    void data_base::CreateNewFile_static(const char* location)
    {
      /*Static version of CreateNewFile
       **/
      std::ofstream out;
      out.open(location, std::ios::out | std::ios::trunc);
      out.close();
    }

    data_base::data_base(const char location[], bool read)
    {
        /*Constructor. This class will be used for reading files by default. However, changing the read flag to false will
        make the constructor prep the object for writing purposes instead*/
        isBufferLoaded = false;
        writeMode = false;
        buffer = "";
        lines = 0;
        s = 0;
        switch(read)
        {
            case false:
            if(LoadData(location))
            {
                writeMode = true;
                LoadStringBuffer();//Get an internal copy of the contents in the file and close the file
                LoadData(location, read);//Reopen the same file but in output mode, which erases all previous data.
                isBufferLoaded = true;//sets this flag true so other processes know the task was successful!
                }
            break;
            default:
            if(LoadData(location))//Checks if internal file buffer was properly started.
            {
                LoadStringBuffer(false);//reads the file buffer into the internal string buffer
                isBufferLoaded = true;//sets this flag true so other processes know the task was successful!
            }
        }
    }
    data_base::~data_base()
    {
        /*Makes sure the file handle is properly closed from the constructor event. It helps prevent file handle leaks.*/
        if(file.is_open())
        {
            file.close();
        }
        if(output.is_open())
        {
            FlushData();
            output.close();
        }
    }

bool copyfile(const std::string& source, const std::string& destination, bool binary)
{
    /* This is my file copy function. It is a supporting function for the data_base class, since this class has the purpose
    of handling file operations!*/
    std::ifstream src;
    std::ofstream out;
    bool result = false;
    if(binary)
    {
        /*If the user wants this operation to be strictly binary, specify the binary modifier!*/
        src.open(source.c_str(), std::ios::binary);
        out.open(destination.c_str(), std::ios::binary);
        if(out << src.rdbuf())
            result = true;
        src.close();
        out.close();
    }
    else
    {
        src.open(source.c_str());
        out.open(destination.c_str());
        if(out << src.rdbuf())
            result = true;
        src.close();
        out.close();
    }
    return result;
}
