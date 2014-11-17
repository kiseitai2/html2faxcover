#include "marker_parser.h"
#include "functions.h"
char h2fax::CALLIDn_CIDNumber   = 1;
char h2fax::CALLIDn_CIDName		= 1;
char h2fax::CALLIDn_DIDNum		= 1;

void h2fax::logMsg(const std::string& msg)
{
    std::cerr << msg << std::endl;
}

h2fax::fax h2fax::faxinfo(const std::string& fileinfo_prog, const std::string& tiff_path)
{
    std::string tmpFile = "/tmp/faxrcvd_inf";
    std::string buff;//will hold file info buffer
    size_t index = 0;
    size_t end = 0;
    size_t count = 0;
    size_t length = 0;
    fax faxData;
    //ask fileinfo to print the header data for the tiff fax file
    exec_cmd(tiff_path.c_str(), fileinfo_prog, "-n > " + tmpFile);
    //open the temporary file that contains the file info!
    data_base file_inf(tmpFile);
    buff = file_inf.GetStrBuffer();
    //Time to dissect the data from the file info
    while(index != std::string::npos)
    {
        index = buff.find(':');
        index++;
        end = buff.find('\n', index);
        length = end - index;
        switch(count)
        {
        case 0:
            faxData.Sender = buff.substr(index, length);
            count++;
            break;
        case 1:
            faxData.Pages = buff.substr(index, length);
            count++;
            break;
        case 2:
            faxData.Quality = buff.substr(index, length);
            count++;
            break;
        case 3:
            faxData.Page = buff.substr(index, length);
            count++;
            break;
        case 4:
            faxData.Received = buff.substr(index, length);
            count++;
            break;
        case 5:
            faxData.TimeToRecv = buff.substr(index, length);
            count++;
            break;
        case 6:
            faxData.SignalRate = buff.substr(index, length);
            count++;
            break;
        case 7:
            faxData.DataFormat = buff.substr(index, length);
            count++;
            break;
        case 8:
            faxData.ErrCorrect = buff.substr(index, length);
            count++;
            break;
        case 9:
            faxData.CallID1 = buff.substr(index, length);//Fax number
            count++;
            break;
        case 10:
            faxData.CallID2 = buff.substr(index);//Name of fax sender
            count++;
            break;
        case 11:
            faxData.CallID3 = buff.substr(index);//DID number
            count++;
            break;
        default:
            std::cerr << "The file contained more fields than expected!" << std::endl;
        }
    }
    //Let's sepparate the time components so we can rearrange as we see fit!
    faxData.extractTime();
    return faxData;
}

void h2fax::faxheader::extractTime()
{
    size_t index = 0;
    size_t end = 0;
    size_t count = 0;
    size_t length = 0;
    //Below I tokenize the time string into its components!
    while(index != std::string::npos && (Received.size() > 0 && Received != "\0"))
    {
        end = Received.find(':');
        length = end - index;
        switch(count)
        {
        case 0:
            docTime.Year = Received.substr(index, length);
            count++;
            break;
        case 1:
            docTime.Month = Received.substr(index, length);
            count++;
            break;
        case 2:
            docTime.Day = Received.substr(index, length);
            count++;
            break;
        case 3:
            docTime.Hour = Received.substr(index, length);
            count++;
            break;
        case 4:
            docTime.Minute = Received.substr(index, length);
            count++;
            break;
        case 5:
            docTime.Second = Received.substr(index);
            count++;
            break;
        default:
            std::cerr << "The time string contained more items than expected!" << std::endl;
        }
        index = end;
    }

}





