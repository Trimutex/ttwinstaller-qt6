#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
	IInstaller {
		string DirFO3Data { get; }
		string DirFNVData { get; }
		string DirTTWMain { get; }
		string DirTTWOptional { get; }

		/// <summary>
		/// Provides progress updates for minor operations
		/// </summary>
		IProgress<InstallStatus> ProgressMinorOperation { get; set; }

		/// <summary>
		/// Provides progress updates for major operations
		/// </summary>
		IProgress<InstallStatus> ProgressMajorOperation { get; set; }

		CancellationToken Token { get; }

		void Install(CancellationToken inToken);
	}
}
