#pragma once

#include <string>

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
	class IInstaller {
    public:
        std::string m_dirFO3Data { get; }
		std::string m_dirFNVData { get; }
		std::string m_dirTTWMain { get; }
		std::string m_dirTTWOptional { get; }

		/// <summary>
		/// Provides progress updates for minor operations
		/// </summary>
		IProgress<InstallStatus> m_progressMinorOperation { get; set; }

		/// <summary>
		/// Provides progress updates for major operations
		/// </summary>
		IProgress<InstallStatus> m_progressMajorOperation { get; set; }

		CancellationToken m_token { get; }

		void install(CancellationToken inToken);
	}
}
