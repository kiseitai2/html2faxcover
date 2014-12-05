#include "marker_parser.h"
#include "faxfunctions.h"
<<<<<<< HEAD
#include "version.h"

static const int NUM_CORRECTION = 2557215978;
=======
>>>>>>> origin/v1.1_debug

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
    data_base file_inf(tmpFile.c_str());
    buff = file_inf.GetStrBuffer();
    //Time to dissect the data from the file info
    index = buff.find("Sender");
    while(index != std::string::npos || (index) < buff.size())
    {
        index = buff.find(':', index + 1);
        end = buff.find('\n', index);
        if(index > buff.size())
            break;
        length = end - index;
        switch(count)
        {
        case 0:
            faxData.Sender = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 1:
            faxData.Pages = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 2:
            faxData.Quality = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 3:
            faxData.Page = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 4:
            faxData.Received = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' ', false));
            index += length;
            count++;
            break;
        case 5:
            faxData.TimeToRecv = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' ', false));
            index += length;
            count++;
            break;
        case 6:
            faxData.SignalRate = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 7:
            faxData.DataFormat = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 8:
            faxData.ErrCorrect = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));
            count++;
            break;
        case 9:
            faxData.CallID1 = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index, length), ':', ' '));//Fax number
            count++;
            break;
        case 10:
            faxData.CallID2 = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index + 1, length), ':', ' '));//Name of fax sender
            count++;
            break;
        case 11:
            faxData.CallID3 = removeLeadingWhiteSpace(replaceCharInStr(buff.substr(index + 1, length), ':', ' '));//DID number
            count++;
            break;
        default:
            std::cerr << "The file contained more fields than expected!" << index << std::endl;
        }
    }
    //Let's sepparate the time components so we can rearrange as we see fit!
    faxData.extractTime();
    //Let's do a final check and clean up
    if(faxData.CallID1 == "")
        faxData.CallID1 = faxData.Sender;
    if(faxData.CallID2 == "")
        faxData.CallID2 = "NO_NAME_" + faxData.Sender;
    if(faxData.CallID3 == "")
        faxData.CallID3 = faxData.Sender;

    //clean the fax numbers from characters like - and (
    faxData.CallID1 = removeMultipleCharFromStr('-', faxData.CallID1.c_str());
    faxData.CallID1 = removeMultipleCharFromStr('(', faxData.CallID1.c_str());
    faxData.CallID1 = removeMultipleCharFromStr(')', faxData.CallID1.c_str());
    faxData.CallID3 = removeMultipleCharFromStr('-', faxData.CallID3.c_str());
    faxData.CallID3 = removeMultipleCharFromStr(')', faxData.CallID3.c_str());
    faxData.CallID3 = removeMultipleCharFromStr('(', faxData.CallID3.c_str());

    //Let's do a phone number correction
    //faxData.Sender = intToStr(cStrToInt(faxData.Sender) - NUM_CORRECTION);
    //Clean tmp file!
    exec_cmd(tmpFile.c_str(), "rm -rf", "");
    return faxData;
}

void h2fax::faxheader::extractTime()
{
    size_t index = 0;
    size_t end = 0;
    size_t count = 0;
    size_t length = 0;
    //Below I tokenize the time string into its components!
    while(index != std::string::npos && (Received.size() > 0 && Received != "\0") || index < Received.size())
    {
        end = Received.find(':', end + 1);
        length = end - index;
        switch(count)
        {
        case 0:
            docTime.Year = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length), ':', ' '));
            count++;
            break;
        case 1:
            docTime.Month = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length), ':', ' '));
            count++;
            break;
        case 2:
            docTime.Day = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length - 3), ':', ' '));
            end -= 3;
            count++;
            break;
        case 3:
            docTime.Hour = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length), ':', ' '));
            count++;
            break;
        case 4:
            docTime.Minute = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length), ':', ' '));
            count++;
            break;
        case 5:
            docTime.Second = removeLeadingWhiteSpace(replaceCharInStr(Received.substr(index, length), ':', ' '));
            count++;
            break;
        default:
            std::cerr << "The time string contained more items than expected!" << std::endl;
        }
        index = end;
    }

}

static void h2fax::print_h2fax_info()
{
    std::cout << "Program name: " << PROG_NAME << std::endl;
    std::cout << "Program version: " << PROG_VERSION << std::endl;
    std::cout << "Program status: " << PROG_RELEASE << std::endl;
}



