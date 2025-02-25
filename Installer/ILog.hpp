#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
    class ILog {
        IProgress<string> DisplayMessage { get; set; }
        
        void File(string msg, params object[] args);
        void Display(string msg, params object[] args);
        void Dual(string msg, params object[] args);
    }
}
