#include "Log.hpp"

namespace TaleOfTwoWastelands {

    IProgress<std::string> DisplayMessage { get; set; }

    void file(std::string msg, params object[] args) {
        Trace.Write(timestamp());
        Trace.WriteLine(string.Format(msg, args));
    }

    void display(std::string msg, params object[] args) {
        Debug.Assert(DisplayMessage != null, "shouldn't call Display before setting DisplayMessage");

        var displayProg = DisplayMessage;
        if (displayProg != null) {
            var sb = timestamp() + msg + args;
            displayProg.Report(sb.ToString());
        }
    }

    void dual(std::string msg, params object[] args) {
        File(msg, args);
        Display(msg, args);
    }

    std::string timestamp(void) {
        // TODO: use actual current time
        return "[" + DateTime.Now + "]\t";
    }
}
