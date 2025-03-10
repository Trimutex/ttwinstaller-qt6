#pragma once

#include <optional>
#include <string>

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    class InstallStatus : IInstallStatus {
    public:
        CancellationToken m_token;
        int m_itemsDone;
        int m_itemsTotal;
        std::string m_currentOperation;
        InstallStatus(IProgress<InstallStatus> progress, std::optional<CancellationToken> token = null);

        CancellationToken getToken(void);
        int getItemsDone(void);
        void setItemsDone(int itemsDone);
        int getItemsTotal(void);
        int setItemsTotal(int itemsTotal);
        std::string getCurrentOperation(void);
        void setCurrentOperation(std::string currentOperation);

        int step();
        void finish();

    private:
        // NOTE: read-only block
        CancellationToken _m_token;
        IProgress<InstallStatus> _m_progress;

        int _m_itemsDone;
        int _m_itemsTotal;
        std::string _m_currentOperation;

        void update();
    }
}
