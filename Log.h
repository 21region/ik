///////////////////////////////////////////////////////////////////////////////
// Log.h
// =====
// It prints out any log messages to file or dialog box.
// Log class is a singleton class which is contructed by calling
// Log::getInstance() (lazy initialization), and is destructed automatically
// when the application is terminated.
//
// In order to log, use ST::log() function with appropriate formats.
// For example, ST::log(L"My number: %d\n", 123).
// It is similar to printf() function of C standard libirary.
//
// The template of the log dialog window is defined in log.rc and logResource.h
// You must include both resource file with this source codes.
// The dialog window cannot be closed by user once it is created. But it will be
// destroyed when the application terminated.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-14
// UPDATED: 2006-07-24
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_LOG_H
#define WIN_LOG_H

#include <string>
#include <vector>
#include <fstream>

namespace ST
{
    enum { LOG_MODE_FILE = 0, LOG_MODE_DIALOG, LOG_MODE_BOTH };
    enum { LOG_MAX_STRING = 1024 };

    // Clients are actually use this functions to send log messages.
    // USAGE: Win::log("I am the number %d.", 1);
    void log(const std::string& str);
    void log(const char *format, ...);

    // singleton class ////////////////////////////////////////////////////////
    class Log
    {
    public:
        ~Log();

        static Log& getInstance();

        void put(const std::string& str);

    private:
        Log();
        Log(const Log& rhs);

        const std::string getTime();
        const std::string getDate();
        const std::vector<std::string> getSubstr(
            const std::string& ) const;

        std::ofstream logFile;
    };
    ///////////////////////////////////////////////////////////////////////////
}

#endif
