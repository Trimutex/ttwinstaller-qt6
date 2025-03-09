#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
    class ILog {
    public:
        IProgress<std::string> m_displayMessage { get; set; }
        
        // TODO: replace `params object[] args`
        void file(std::string msg, params object[] args);
        void display(std::string msg, params object[] args);
        void dual(std::string msg, params object[] args);
    }
}
