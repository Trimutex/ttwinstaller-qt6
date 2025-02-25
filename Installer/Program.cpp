﻿#include "Program.hpp"

namespace TaleOfTwoWastelands {

    static bool VerifyElevation() {
        var identity = WindowsIdentity.GetCurrent();
        Debug.Assert(identity != null, "identity != null");

        var principal = new WindowsPrincipal(identity);
        return principal.IsInRole(WindowsBuiltInRole.Administrator);
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
        // NOTE: MAIN IS HERE
        static void Main() {
            SetupTraceListeners();

            Application.ThreadException += Application_ThreadException;
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new UI.frm_Main());
        }

    static void SetupTraceListeners() {
        Directory.CreateDirectory(LogDirectory);

        var LogFilename = "Install Log " + DateTime.Now.ToString("MM_dd_yyyy - HH_mm_ss") + ".txt";
        var LogFilepath = Path.Combine(LogDirectory, LogFilename);

        Trace.AutoFlush = true;
        Trace.Listeners.Add(new TextWriterTraceListener(new FinalizedTextWriter(LogFilepath)));
    }

    static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e) {
        HandleCrashException(e.Exception);
    }

    static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e) {
        HandleCrashException((Exception)e.ExceptionObject);
    }

    static void HandleCrashException(Exception e) {
        Trace.WriteLine("An uncaught exception occurred: " + e);
        MessageBox.Show(Localization.UncaughtExceptionMessage);
        Environment.Exit(1);
    }
}
