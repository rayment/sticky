%{
#include "log/log.h"
%}

%nspace log;

%inline %{
class log
{
public:
    static void trace(const std::string &msg)
    {
        st_log_trace("%s", msg.data());
    }

    static void debug(const std::string &msg)
    {
        st_log_debug("%s", msg.data());
    }

    static void info(const std::string &msg)
    {
        st_log_info("%s", msg.data());
    }

    static void warn(const std::string &msg)
    {
        st_log_warn("%s", msg.data());
    }

    static void error(const std::string &msg)
    {
        st_log_error("%s", msg.data());
    }

private:
    log() {}
    ~log() {}
};
%}
