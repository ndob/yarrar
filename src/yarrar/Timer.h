#pragma once

#include "Util.h"

#include <chrono>
#include <iostream>
#include <sstream>

namespace yarrar
{

class Timer
{
public:
    Timer(bool startImmediately, const std::string& name = "n/a")
        : m_name(name)
        , m_started(false)
    {
        if(startImmediately)
        {
            start();
        }
    }

    ~Timer()
    {
        end();
    }

    void start()
    {
        m_start = std::chrono::system_clock::now();
        m_started = true;
        printCurrentState("START");
    }

    void end()
    {
        if(m_started)
        {
            m_started = false;
            printCurrentState("END");
        }
    }

    void printElapsed(const std::string& tag = "")
    {
        printCurrentState(tag);
    }

private:
    void printCurrentState(const std::string& tag = "")
    {
        std::chrono::duration<double> seconds = std::chrono::system_clock::now() - m_start;
        std::ostringstream oss;
        oss << "[" << m_name << "]"
            << " tag: " << tag << " ";
        if(m_started)
        {
            oss << "time elapsed: " << seconds.count() << '\n';
        }
        else
        {
            oss << "not running." << '\n';
        }

        util::log(oss.str());
    }

    std::string m_name;
    bool m_started;
    std::chrono::time_point<std::chrono::system_clock> m_start;
};
}
