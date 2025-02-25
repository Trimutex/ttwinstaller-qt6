#pragma once

namespace TaleOfTwoWastelandsProgress {
    // NOTE: public class
    // NOTE: interface class
    public interface IInstallStatusUpdate {
        string CurrentOperation { get; set; }
        int ItemsDone { get; set; }
        int ItemsTotal { get; set; }
        CancellationToken Token { get; }
        int Step();
    }
}
