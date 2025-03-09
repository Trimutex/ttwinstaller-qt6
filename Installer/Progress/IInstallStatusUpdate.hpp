#pragma once

#include <string>

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    // NOTE: interface class
    class IInstallStatusUpdate {
    public:
        std::string m_currentOperation;
        int m_itemsDone;
        int m_itemsTotal;
        CancellationToken m_token;
        int step();
    }
}
