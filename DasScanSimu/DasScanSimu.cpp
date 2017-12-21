#include "DasScanSimu.h"
#include <stdio.h>
#include <windows.h>

void CALLBACK ScanStop(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	printf("Scan Finished\n");
	scanFinished = true;

	printf("pd data saved\n");
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

	DWORD pathLength;
	TCHAR currPath[MAX_PATH_LONG + 1];

	SYSTEMTIME currentTime;
	TCHAR datName[MAX_NAME + 1], datDir1[MAX_NAME + 1], datDir2[MAX_NAME + 1], datFile[MAX_NAME + 1];

	HANDLE hFile = NULL;

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

	pathLength = GetCurrentDirectory(MAX_PATH_LONG, currPath);
	if (pathLength == 0 || pathLength >= MAX_PATH_LONG) {	
		fprintf(stderr, "DasScanSimu: GetCurrentDirectory failed.\n");
		return;
	}

	SetCurrentDirectory("d:\\pangu.dat\\raw\\");

	GetSystemTime(&currentTime);

	_snprintf_s(datName, MAX_NAME, "%04d%02d%02d%02d%02d%02d", currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
	_snprintf_s(datDir1, MAX_NAME, "1.%s", datName);
	_snprintf_s(datDir2, MAX_NAME, "2.%s", datName);
	_snprintf_s(datFile, MAX_NAME, "2.%s.pd", datName);

	if (CreateDirectory(datDir1, NULL)) {
		SetCurrentDirectory(datDir1);
		if (CreateDirectory(datDir2, NULL)) {
			SetCurrentDirectory(datDir2);
			hFile = CreateFile(datFile, GENERIC_READ | GENERIC_WRITE, 0, NULL,
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE) {
				fprintf(stderr, "DasScanSimu: create %s failed.\n", datFile);
			}
		}
	} else if (ERROR_ALREADY_EXISTS == GetLastError()) {
		fprintf(stderr, "DasScanSimu: directory %s already exists.\n", datDir1);
	} else {
		fprintf(stderr, "DasScanSimu: create directory %s failed.\n", datDir1);
	}

	if (hFile != NULL) {
		CloseHandle(hFile);
	}

	SetCurrentDirectory(currPath);	 /* Restore working directory. */
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
