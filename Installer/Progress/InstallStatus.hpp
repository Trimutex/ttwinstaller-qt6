#pragma once

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    class InstallStatus : IInstallStatus {
    public:
        CancellationToken m_token;
        int m_itemsDone;
        int m_itemsTotal;
        string m_currentOperation;
        InstallStatus(IProgress<InstallStatus> progress, CancellationToken? token = null);
        int step();
        void finish();

    private:
        // NOTE: read-only block
        CancellationToken _m_token;
        IProgress<InstallStatus> _m_progress;

        int _m_itemsDone;
        int _m_itemsTotal;
        string _m_currentOperation;

        void update();
    }
}
