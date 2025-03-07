#include "IInstallStatusUpdate.hpp"

string CurrentOperation { get; set; }
int ItemsDone { get; set; }
int ItemsTotal { get; set; }
CancellationToken Token { get; }
int Step();
