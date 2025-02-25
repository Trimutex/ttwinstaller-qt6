#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: internal class
    class Log : ILog {
    public:
        IProgress<string> DisplayMessage { get; set; }
        
        void File(string msg, params object[] args);
        void Display(string msg, params object[] args);
        void Dual(string msg, params object[] args);

    private:
        static StringBuilder Timestamp;

    }
}
