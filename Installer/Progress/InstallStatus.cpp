#include "InstallStatus.hpp"

namespace TaleOfTwoWastelandsProgress {
    CancellationToken getToken(void) {
        return _m_token;
    }

    int getItemsDone(void) {
        return _m_itemsDone;
    }

    void setItemsDone(int itemsDone) {
        if (_m_itemsDone == itemsDone)
            return;
        _m_itemsDone = itemsDone;
        update();
    }

    int getItemsTotal(void) {
        return _m_itemsTotal;
    }

    int setItemsTotal(int itemsTotal) {
        if (_m_itemsTotal == itemsTotal)
            return;
        _m_itemsTotal = itemsTotal;
        update();
    }

    std::string getCurrentOperation(void) {
        return _m_currentOperation;
    }

    void setCurrentOperation(std::string currentOperation) {
        if (_m_currentOperation == currentOperation)
            return;
        _m_currentOperation = currentOperation;
        update();
    }

    InstallStatus(IProgress<InstallStatus> progress, std::optional<CancellationToken> token = null) {
        if (token)
            _m_token = token;
        else
            _m_token = CancellationToken.None;
        _m_progress = progress;
    }

    int step() {
        var itemsDone = Interlocked.Increment(ref _m_itemsDone);
        if (itemsDone > ItemsTotal)
            throw new ArgumentOutOfRangeException();

        update();
        return itemsDone;
    }

    void finish() {
        _m_itemsDone = 0;
        _m_itemsTotal = 0;
        _m_currentOperation = "";
        update();
    }

    void update() {
        _token.ThrowIfCancellationRequested();
        _progress.report(this);
    }
}
