#include "DasScanSimu.h"
#include <stdio.h>
#include <windows.h>


void CALLBACK ScanStop(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	printf("Scan Finished\n");
	scanFinished = true;

	//todo: generate data files in d:
}

void ScanInit()
{
	printf("ScanInit\n");
	Sleep(500);
}

void LoadProtocol(const char* protocolName)
{
	printf("Load protocol file %s\n", protocolName);
}

void ScanGo()
{
	HANDLE timer;
	LARGE_INTEGER t;

	printf("Scan start\n");
	scanFinished = false;

	//timer = CreateWaitableTimer(NULL, FALSE, NULL);
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (timer == NULL){
		fprintf(stderr, "DasScanSimu: Create timer failed.\n");
		return;
	}

	t.QuadPart = -10 * 10000000;
	SetWaitableTimer(timer, &t, 0, ScanStop, NULL, FALSE);

	//SleepEx(INFINITE, TRUE);
}

bool IsScanFinished()
{
	//SleepEx(100, TRUE);
	return scanFinished;
}

void ScanAbort()
{
	printf("Abort scan\n");
	scanFinished = true;
}
