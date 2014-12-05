#ifndef MARKER_PARSER_H_INCLUDED
#define MARKER_PARSER_H_INCLUDED

#include <string>
#include "typedefs.h"
#include "data_base.h"
#include "cmd_processor.h"
#include "default.h"


namespace h2fax
{
    void removeAvantFaxPrefix(cstr filepath, const std::string& prefix);
    cstr copyFile(cstr source, cstr destination);
    cstr copyDir(cstr source, cstr destination);
    cstr removeDir(cstr target);
    void convertToPS(cstr filepath, const std::string& html2ps, const std::string& options);
    void substituteMarkers(cstr filepath, faxcover_args& data);
    std::string getDate();
}



#endif // MARKER_PARSER_H_INCLUDED
