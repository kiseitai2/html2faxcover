#include "marker_parser.h"
#include "conversion.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>


void h2fax::removeAvantFaxPrefix(cstr filepath, const std::string& prefix)
{
    FILE* in, *out;
    std::string tmp;
    char c;
    //Open and read file contents
    in = fopen(filepath, "r");
    if (in == NULL) std::cerr << "Error opening file.1" << std::endl;
    else
    {
        c = fgetc(in);
        while(c != EOF)
        {
            tmp += c;
            c = fgetc(in);
        }
        fclose(in);
    }
    //Modify contents
    tmp = replaceStrInStr(tmp, prefix, "");
    //Output the new contents to the same file
    out = fopen(filepath, "w");
    if (out == NULL) std::cerr << "Error opening file.2" << std::endl;
    else
    {
        for(size_t i = 0; i < tmp.size(); i++)
        {
            if(fputc(tmp[i], out) == EOF)
            {
                std::cerr << "Error writing file!" << std::endl;
                break;
            }
        }
        fclose(out);
    }
}

h2fax::cstr h2fax::copyFile(cstr source, cstr destination)
{
    std::string cmd = "cp -f ";
    cmd += source;
    cmd += " ";
    cmd += destination;
    if(std::system(cmd.c_str()))
        return destination;
    return NULL;
}


h2fax::cstr h2fax::copyDir(cstr source, cstr destination)
{
    std::string cmd = "cp -r ";
    cmd += source;
    cmd += " ";
    cmd += destination;
    if(std::system(cmd.c_str()))
        return destination;
    return NULL;
}

h2fax::cstr h2fax::removeDir(cstr target)
{
    std::string cmd = "rm -r ";
    cmd += target;
    if(std::system(cmd.c_str()))
        return target;
    return NULL;
}

void h2fax::convertToPS(cstr filepath, const std::string& html2ps, const std::string& options)
{
    std::string command = html2ps + " ";
    command += filepath;
    command += " " + options;
    std::system(command.c_str());
}

void h2fax::substituteMarkers(cstr filepath, faxcover_args& data)
{
    FILE* in, *out;
    std::string tmp;
    char c;
    //Open and read file contents
    in = fopen(filepath, "r");
    if (in == NULL) std::cerr << "Error opening file.3" << std::endl;
    else
    {
        c = fgetc(in);
        while(c != EOF)
        {
            tmp += c;
            c = fgetc(in);
        }
        fclose(in);
    }
    //Modify contents
    for(byte i = 0; i < MARKERSNUM; i++)
    {
        tmp = replaceStrInStr(tmp, MARKERS[i], data[MARKERS[i].c_str()], false);
    }
    //Output the new contents to the same file
    out = fopen(filepath, "w");
    if (out == NULL) std::cerr << "Error opening file.4" << std::endl;
    else
    {
        for(size_t i = 0; i < tmp.size(); i++)
        {
            if(fputc(tmp[i], out) == EOF)
            {
                std::cerr << "Error writing file!" << std::endl;
                break;
            }
        }
        fclose(out);
    }
}

std::string h2fax::getDate()
{
    time_t     now = time(0);
    struct tm*  tstruct;
    std::string buf;
    tstruct = localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    buf = intToStr(tstruct->tm_mday) + "/" + intToStr(tstruct->tm_mon + 1) + "/" + intToStr(tstruct->tm_year + 1900);
    //buf = asctime(tstruct);
    return buf;
}

void h2fax::exec_cmd(cstr filepath, const std::string& cmd, const std::string& options)
{
    convertToPS(filepath, cmd, options);
}
