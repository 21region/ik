///////////////////////////////////////////////////////////////////////////////
// Log.cpp
// =======
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
// AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-14
// UPDATED: 2006-07-24
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <iomanip>
#include <sstream>
#include "Log.h"
using namespace ST;
using namespace std;

const char* LOG_FILE = "log.txt";


///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
Log::Log()
{
    // open log file
    logFile.open(LOG_FILE, ios::out);
    if(logFile.fail())
        return;

    // first put starting date and time
    logFile << "===== Log started at "
            << getDate() << ", "
            << getTime() << ". =====\n\n"
            << flush;
}

///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
Log::~Log()
{
    // close opened file
    logFile << "\n===== END OF LOG =====\n";
    logFile.close();
}

///////////////////////////////////////////////////////////////////////////////
// instantiate a singleton instance if not exist
///////////////////////////////////////////////////////////////////////////////
Log& Log::getInstance()
{
    static Log self;
    return self;
}

///////////////////////////////////////////////////////////////////////////////
// add message to log
///////////////////////////////////////////////////////////////////////////////
void Log::put(const string& message)
{
    string time = getTime() + "  ";
    // substrings without '\n'
    vector<string> substrings = getSubstr(message);

    // if there is no '\n' or only '\n'
    if (substrings.size() <= 1)
    {
        logFile << time
                << message
                << '\n'
                << flush;
    }
    else
    {
        logFile << time << substrings[0] << '\n';

        // used instead of time
        string empty = " ";
        for (size_t i = 1; i < time.size(); i++)
            empty += " ";

        for (size_t i = 1; i < substrings.size(); i++)
            logFile << empty << substrings[i] << '\n';

        logFile << flush;
    }
}

///////////////////////////////////////////////////////////////////////////////
// get strings without \n
///////////////////////////////////////////////////////////////////////////////
const vector<string> Log::getSubstr(const string& str) const
{
    vector<string> substrings;

    size_t from = 0, // search from
             to = 0; // '\n' location

    while ((to = str.find("\n", from))
            != string::npos)
    {
        substrings.push_back(str.substr(from, to - from));
        from = to + 1;                              // next char after '\n'
    }

    // store string on the right from '\n'
    if (from != string::npos)
        substrings.push_back(str.substr(from));

    return substrings;
}

///////////////////////////////////////////////////////////////////////////////
// get system date as a string
///////////////////////////////////////////////////////////////////////////////
const string Log::getDate()
{
    stringstream ss;

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    ss << setfill('0');
    ss << sysTime.wYear << "-" << setw(2)
       << sysTime.wMonth << "-" << setw(2)
       << sysTime.wDay;

    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////
// get system time as a string
///////////////////////////////////////////////////////////////////////////////
const string Log::getTime()
{
    stringstream ss;

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    ss << setfill('0');
    ss << sysTime.wHour << ":" << setw(2)
       << sysTime.wMinute << ":" << setw(2)
       << sysTime.wSecond;

    return ss.str();
}

void ST::log(const char *format, ...)
{
    char buffer[LOG_MAX_STRING];

    // do the formating
    va_list valist;
    va_start(valist, format);
    _vsnprintf(buffer, LOG_MAX_STRING, format, valist);
    va_end(valist);

    Log::getInstance().put(buffer);
}

void ST::log(const string& str)
{
    Log::getInstance().put(str);
}
