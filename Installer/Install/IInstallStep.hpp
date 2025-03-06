#pragma once

#include <optional>

namespace TaleOfTwoWastelandsInstall {
	interface IInstallStep {
        std::<optional> bool run(IInstallStatusUpdate status, CancellationToken token);
	}
}
