#pragma once

#include <string>

namespace TaleOfTwoWastelands {
    // NOTE: internal class
    class Log : ILog {
    public:
        IProgress<std::string> m_displayMessage { get; set; }
        
        void file(std::string msg, params object[] args);
        void display(std::string msg, params object[] args);
        void dual(std::string msg, params object[] args);

    private:
        std::string timestamp(void);
    }
}
