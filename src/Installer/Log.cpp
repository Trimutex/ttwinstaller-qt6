#include "Log.hpp"
#include <chrono>

namespace TaleOfTwoWastelands {
    void Log::file(std::string msg, params object[] args) {
        Trace.Write(timestamp());
        Trace.WriteLine(string.Format(msg, args));
    }

    void Log::display(std::string msg, params object[] args) {
        Debug.Assert(m_displayMessage != null, "shouldn't call Display before setting DisplayMessage");

        var displayProg = m_displayMessage;
        if (displayProg != null) {
            var sb = timestamp() + msg + args;
            displayProg.Report(sb.ToString());
        }
    }

    void Log::dual(std::string msg, params object[] args) {
        file(msg, args);
        display(msg, args);
    }

    std::string Log::timestamp(void) {
        // TODO: use actual current time
        return "[" + std::chrono::system_clock::now() + "]\t";
    }
}
