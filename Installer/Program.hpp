#pragma once

namespace TaleOfTwoWastelands {
    static class Program {
    public:
        // NOTE: read-only block
        static string LogDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "My Games", "TaleOfTwoWastelands");
        static bool IsElevated = VerifyElevation();

        static void Main();
        static void SetupTraceListeners();
        static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e);
        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e);
        static void HandleCrashException(Exception e);
        
    private:
        static bool VerifyElevation();
    }
}
