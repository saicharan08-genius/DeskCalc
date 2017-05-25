#pragma once

#include <functional>
#include <stdexcept>
#include <sstream>

class ErrorReporter {
public:
    template<class T, class ...Args>
    void operator()(const T& t, Args&&... args)
    {
        error(t, std::forward<Args>(args)...);
    }

    void set_cleanup_func(std::function<void()> func) { cleanup = func; }

private:
    template<class T, class ...Args>
    void error(const T& t, Args&&... args)
    {
        msgStream << t;
        error(std::forward<Args>(args)...);
    }

    void error()
    {
        auto msg = msgStream.str();
        msgStream.str("");
        msgStream.clear();
        cleanup();
        throw std::runtime_error{ msg };
    }

    std::ostringstream msgStream;
    std::function<void()> cleanup;
};