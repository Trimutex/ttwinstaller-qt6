#include "IInstallStatusUpdate.hpp"

string m_currentOperation { get; set; }
int m_itemsDone { get; set; }
int m_itemsTotal { get; set; }
CancellationToken m_token { get; }
int step();
