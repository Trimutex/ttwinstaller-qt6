#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
    class ILog {
    public:
        IProgress<string> m_displayMessage { get; set; }
        
        void file(string msg, params object[] args);
        void display(string msg, params object[] args);
        void dual(string msg, params object[] args);
    }
}
