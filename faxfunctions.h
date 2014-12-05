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

<<<<<<< HEAD
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

=======
>>>>>>> origin/v1.1_debug

//Functions
fax faxinfo(const std::string& fileinfo_prog, const std::string& tiff_path);
std::string strip_sipinfo(std::string dirtyStr);
void logMsg(const std::string& msg);

}

#endif // FAXFUNCTIONS_H_INCLUDED
