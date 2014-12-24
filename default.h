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
