extern "C" __declspec(dllexport) void ScanInit();
extern "C" __declspec(dllexport) void LoadProtocol(const char* protocolName);
extern "C" __declspec(dllexport) void ScanGo();
extern "C" __declspec(dllexport) bool IsScanFinished();
extern "C" __declspec(dllexport) void ScanAbort();

bool scanFinished=true;

#define MAX_PATH_LONG 32767  /* Very long path names with \\?\ prefix. */
#define MAX_NAME 256    /* Name length - users and groups */