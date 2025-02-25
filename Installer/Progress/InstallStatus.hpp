#pragma once

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    class InstallStatus : IInstallStatus {
    public:
        CancellationToken Token {
            get { return _token; }
        }

        int ItemsDone {
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

        int ItemsTotal {
            get
            {
                return _itemsTotal;
            }
            set
            {
                if (_itemsTotal != value)
                {
                    _itemsTotal = value;
                    Update();
                }
            }
        }

        string CurrentOperation {
            get
            {
                return _currentOperation;
            }
            set
            {
                if (_currentOperation != value)
                {
                    _currentOperation = value;
                    Update();
                }
            }
        }

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

    private:
        // NOTE: read-only block
        CancellationToken _token;
        IProgress<InstallStatus> _progress;

        int _itemsDone;
        int _itemsTotal;
        string _currentOperation;

        void Update() {
            _token.ThrowIfCancellationRequested();
            _progress.Report(this);
        }
    }
}
