#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: internal class
    class Log : ILog {
    public:
        IProgress<string> m_displayMessage { get; set; }
        
        void file(string msg, params object[] args);
        void display(string msg, params object[] args);
        void dual(string msg, params object[] args);

    private:
        static StringBuilder m_timestamp;

    }
}
