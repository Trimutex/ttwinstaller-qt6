#include "InstallStatus.hpp"

namespace TaleOfTwoWastelandsProgress {
    CancellationToken m_token {
        get { return _token; }
    }

    int m_itemsDone {
        get
        {
            return _itemsDone;
        }
        set
        {
            if (_itemsDone != value)
            {
                _itemsDone = value;
                Update();
            }
        }
    }

    int m_itemsTotal {
        get
        {
            return _itemsTotal;
        }
        set
        {
            if (_itemsTotal != value) {
                _itemsTotal = value;
                Update();
            }
        }
    }

    string m_currentOperation {
        get
        {
            return _currentOperation;
        }
        set
        {
            if (_currentOperation != value) {
                _currentOperation = value;
                update();
            }
        }
    }

    InstallStatus(IProgress<InstallStatus> progress, CancellationToken? token = null) {
        _token = token ?? CancellationToken.None;
        _progress = progress;
    }

    int step() {
        var itemsDone = Interlocked.Increment(ref _itemsDone);
        if (itemsDone > ItemsTotal)
            throw new ArgumentOutOfRangeException();

        update();
        return itemsDone;
    }

    void finish() {
        _itemsDone = 0;
        _itemsTotal = 0;
        _currentOperation = "";
        update();
    }

    void update() {
        _token.ThrowIfCancellationRequested();
        _progress.report(this);
    }
}
