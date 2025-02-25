#pragma once

#include <optional>

namespace TaleOfTwoWastelandsInstall {
	interface IInstallStep {
        std::<optional> bool Run(IInstallStatusUpdate status, CancellationToken token);
	}
}
