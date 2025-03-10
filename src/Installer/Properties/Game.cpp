#include "Game.hpp"

namespace TaleOfTwoWastelandsProperties {
    // NOTE: originally internal
    static IDSS getBuildableBSAs() {
        return _buildableBsas.Value;
    }

    // NOTE: originally internal
    static IDSSA getCheckedBSAs() {
        return _checkedBsas.Value;
    }

    // NOTE: originally internal
    static std::string[] getCheckedESMs() {
        return _checkedEsms.Value;
    }

    // NOTE: originally internal
    static IDSS getVoicePaths() {
        return _voicePaths.Value;
    }
}
