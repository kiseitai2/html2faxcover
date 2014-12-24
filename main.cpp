#include <iostream>
#include "html2faxcover.h"
#include "data_base.h"

enum Modes
{
    STAND_ALONE, CALL_HYLAFAX_FAXCOVER, CALL_HYLAFAX_FAXCOVER_AS_IS, HASH_TEST
};

#define TMP_HTML "tmp.html"
#define PATH_SLASH "/"

//Prototypes
void getNameAndEmail(MySQL& db, std::string criteria, std::string& name, std::string& email);
std::string generateCommand(const h2fax::faxcover_args& args);

int main(int argc, char* argv[])
{
    //Grab command line arguments in a nicely packaged structure!
    h2fax::faxcover_args args = h2fax::getParameters(argc, argv);
    h2fax::uint execMode;
    data_base settings("settings");
    std::string name, email;
    std::string prefix = settings.GetStrFromData("prefix");
    std::string html2ps_path = settings.GetStrFromData("html2ps");
    std::string tmp = settings.GetStrFromData("tmp");
    std::string hylafax = settings.GetStrFromData("hylafax");
    //Let's only execute the rest of the program if we have all of the settings
    if(args.argNum > 1)
    {
     execMode = cStrToInt(args.execMode);

    //Let's prep the file path for later usage
    std::string path = args.template_path;
    path = replaceCharInStr(path, '\\', '/');
    std::string root_path = path.substr(0, path.rfind("/")) + PATH_SLASH;
    std::string file = path.substr(root_path.size());

    //Setup date
    args.date = h2fax::getDate();

    //Let's check we have the information for connecting to a database
    if(!args.database || !args.host || !args.username || !args.password)
   {
        args.Database = settings.GetStrFromData("database").c_str();
        args.Host = settings.GetStrFromData("host").c_str();
        args.Username = settings.GetStrFromData("username").c_str();
        args.Password = settings.GetStrFromData("password").c_str();
   }

   if(settings.GetIntFromData("ignore_db_args"))
   {
	args.Database = settings.GetStrFromData("database").c_str();
        args.Host = settings.GetStrFromData("host").c_str();
        args.Username = settings.GetStrFromData("username").c_str();
        args.Password = settings.GetStrFromData("password").c_str();
   }

    //Let's make sure we have an html template
    if(path.find(".ps") != std::string::npos)
        return EXIT_FAILURE;

    //Now, let's create a database connection!
    MySQL db(args.Database.c_str(), args.Username.c_str(), args.Password.c_str(), args.Host.c_str());

    if(db.getStatus())
        return EXIT_FAILURE;

    switch(execMode)
    {
    case STAND_ALONE:
        {
            //Get name and address from the database
            getNameAndEmail(db, args.from, name, email);
            //Update args
            args.from = name.c_str();
            args.email = email.c_str();
            /*Copy folder containing coversheet and supplemental materials like 
            images to a temporary location*/
            h2fax::copyDir(root_path.c_str(), tmp.c_str());
            //Take away the AvantFax prefix and substitute the markers like to-company for their actual value
            h2fax::removeAvantFaxPrefix((tmp + file).c_str(), prefix);
            h2fax::substituteMarkers((tmp + file).c_str(), args);
            //Convert file to PS for HylaFax!
            h2fax::convertToPS((tmp + file).c_str(), html2ps_path, "");
            /*By now, HylaFax should be receiving the processed html template via
            stdin or the pipe. I am not sure how it reads the output since I have
            not looked at the code for the program.*/
            //Let's clean our temporary location!
            h2fax::removeDir(tmp.c_str());
            
            break;
        }
    case CALL_HYLAFAX_FAXCOVER:
        {
            //Get name and address from the database
            getNameAndEmail(db, args.from, name, email);
            //Update args
            args.from = name.c_str();
            args.email = email.c_str();
            /*Unlike the previous scenario, Hylafax or whatever calling application requested that
            the program convert the html template into the PostScript template that Hylafax's faxcover
            accepts. No processing of the markers is required beyond stripping them of the AvantFax prefix.
            */
            //Copy folder where template is to a temporary local folder
            h2fax::copyFile((root_path + PATH_SLASH).c_str(), tmp.c_str());
            //Take away the AvantFax prefix and substitute the markers like to-company for their actual value
            h2fax::removeAvantFaxPrefix((tmp + file).c_str() , prefix);
//This step ensures that the final PostScript file contains the local images!
            h2fax::copyFile(tmp.c_str(), (root_path + PATH_SLASH + TMP_HTML).c_str());
            //Convert file to PS for HylaFax!
            h2fax::convertToPS((root_path + PATH_SLASH + TMP_HTML).c_str(), html2ps_path, " > " + tmp + ".ps");
            //Now, we execute faxcover with all of the switches.
            args.template_path = (tmp + ".ps").c_str();
            h2fax::convertToPS(generateCommand(args).c_str(), hylafax, "");
            //By now, HylaFax should be receiving the processed html template via stdin.
            break;
        }
    case CALL_HYLAFAX_FAXCOVER_AS_IS:
        {
            //In this case, the program is asked to strip the template of the AvantFax prefix and send it to faxcover.
            //Copy template file to a temporary local file
	    std::cout << "Hi" << std::endl;
            h2fax::copyFile(args.template_path, (tmp + ".ps").c_str());
            //Take away the AvantFax prefix and substitute the markers like to-company for their actual value
            h2fax::removeAvantFaxPrefix(tmp.c_str(), prefix);
            //Now, we execute faxcover with all of the switches.
            args.template_path = (tmp + ".ps").c_str();
            h2fax::convertToPS(generateCommand(args).c_str(), hylafax, "");
            //By now, HylaFax should be receiving the processed html template via stdin.
            break;
        }
    case HASH_TEST:
        {
          for(int i = 0; i < h2fax::MARKERSNUM; i++)
          {
            std::cout << std::hex << args.hashFunction(h2fax::MARKERS[i].c_str()) << std::endl;
          }
          break;
        }
    default:
        std::cout << "The execution mode flag that was provided does not match any case in the program. The program will "
            << "close! Thanks for using me? << Sounds a bit echi even for a machine! :3" << std::endl;
    }
    }

    return EXIT_SUCCESS;
}

void getNameAndEmail(MySQL& db, std::string criteria, std::string& name, std::string& email)
{
    //Let's do a small check!
    if(criteria[0] != '\'' || criteria[criteria.size() - 1] != '\'')
        criteria = "'" + criteria + "'";
    std::string tmp = db.prepareStatement("UserAccount", "name", "username = " + criteria);
    //Query Database for username
    db.queryDB(tmp);
    db.getResult(name);
    //Generate a new query for the email
    tmp = db.prepareStatement("UserAccount", "email", "username = " + criteria);
    db.queryDB(tmp);
    db.getResult(email);
}

std::string generateCommand(const h2fax::faxcover_args& args)
{
    return "-c " + std::string(args.comments) + " -C " + std::string(args.template_path) + " -l "
        + std::string(args.to_location) + " -L " + std::string(args.from_location) + " -M " + std::string(args.email)
        + " -n " + std::string(args.to_fax) + " -N " + std::string(args.from_fax) + " -p " + std::string(args.pageCount)
        + " -r " + std::string(args.regarding) + " -f " + std::string(args.from) + " -t " + std::string(args.to)
        + " -v " + std::string(args.to_voice) + " -V " + std::string(args.from_voice) + " -x "
        + std::string(args.to_company) + " -X " + std::string(args.from_company) + "-s" + std::string(args.pageSize) + " -u ";
}
