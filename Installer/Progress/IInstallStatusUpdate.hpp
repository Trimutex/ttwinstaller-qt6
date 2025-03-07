#pragma once

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    // NOTE: interface class
    class IInstallStatusUpdate {
    public:
        string m_currentOperation;
        int m_itemsDone;
        int m_itemsTotal;
        CancellationToken m_token;
        int step();
    }
}
