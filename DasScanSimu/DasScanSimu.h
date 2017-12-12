extern "C" __declspec(dllexport) void ScanInit();
extern "C" __declspec(dllexport) void LoadProtocol(const char* protocolName);
extern "C" __declspec(dllexport) void ScanGo();
extern "C" __declspec(dllexport) bool IsScanFinished();
extern "C" __declspec(dllexport) void ScanAbort();

bool scanFinished=true;