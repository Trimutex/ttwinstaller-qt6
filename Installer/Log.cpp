﻿#include "Log.hpp"

namespace TaleOfTwoWastelands {

    IProgress<string> DisplayMessage { get; set; }

    void File(string msg, params object[] args) {
        Trace.Write(Timestamp);
        Trace.WriteLine(string.Format(msg, args));
    }

    void Display(string msg, params object[] args) {
        Debug.Assert(DisplayMessage != null, "shouldn't call Display before setting DisplayMessage");

        var displayProg = DisplayMessage;
        if (displayProg != null) {
            var sb = Timestamp.AppendFormat(msg, args);
            displayProg.Report(sb.ToString());
        }
    }

    public void Dual(string msg, params object[] args) {
        File(msg, args);
        Display(msg, args);
    }

    static StringBuilder Timestamp {
        get
        {
            return new StringBuilder()
                .Clear()
                .Append('[')
                .Append(DateTime.Now)
                .Append(']')
                .Append('\t');
        }
    }
}
