#pragma once

namespace TaleOfTwoWastelands {
    static class Program {
    public:
        // NOTE: read-only block
        static std::string m_logDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "My Games", "TaleOfTwoWastelands");
        static bool m_isElevated = verifyElevation();

        static void main();
        static void setupTraceListeners();
        static void application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e);
        static void currentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e);
        static void handleCrashException(Exception e);
        
    private:
        static bool verifyElevation();
    }
}
