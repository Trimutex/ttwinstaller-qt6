#include "InstallStatus.hpp"

namespace TaleOfTwoWastelandsProgress {
    InstallStatus(IProgress<InstallStatus> progress, CancellationToken? token = null) {
        _token = token ?? CancellationToken.None;
        _progress = progress;
    }

    int Step() {
        var itemsDone = Interlocked.Increment(ref _itemsDone);
        if (itemsDone > ItemsTotal)
            throw new ArgumentOutOfRangeException();

        Update();
        return itemsDone;
    }

    void Finish() {
        _itemsDone = 0;
        _itemsTotal = 0;
        _currentOperation = "";
        Update();
    }

    void Update() {
        _token.ThrowIfCancellationRequested();
        _progress.Report(this);
    }
}
