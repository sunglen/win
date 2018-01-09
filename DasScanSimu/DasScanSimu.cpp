#include "DasScanSimu.h"
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

void copyPdFile(const char* testName)
{
	DWORD pathLength;
	TCHAR currPath[MAX_PATH_LONG + 1];

	SYSTEMTIME currentTime;
	TCHAR datName[MAX_NAME + 1], datDir1[MAX_NAME + 1], datDir2[MAX_NAME + 1], datFile[MAX_NAME + 1];

	HANDLE hFile = NULL;
	HANDLE hSrc = NULL;

	DWORD nIn, nOut;
	CHAR buffer[BUF_SIZE];

	TCHAR srcName[MAX_NAME + 1];

	pathLength = GetCurrentDirectory(MAX_PATH_LONG, currPath);
	if (pathLength == 0 || pathLength >= MAX_PATH_LONG) {
		fprintf(stderr, "DasScanSimu: GetCurrentDirectory failed.\n");
		return;
	}

	SetCurrentDirectory("d:\\pangu.dat\\raw\\");

	_snprintf_s(srcName, MAX_NAME, "C:\\Insitum.App\\%s.pd", testName);

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

			//copy srcName to datFile
			hSrc = CreateFile(srcName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hSrc == INVALID_HANDLE_VALUE) {
				fprintf(stderr, "DasScanSimu: Cannot open %s.\n", srcName);
			}

			while (ReadFile(hSrc, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
				WriteFile(hFile, buffer, nIn, &nOut, NULL);
				if (nIn != nOut) {
					fprintf(stderr, "DasScanSimu: Fatal write error.\n");
				}
			}
		}
	}
	else if (ERROR_ALREADY_EXISTS == GetLastError()) {
		fprintf(stderr, "DasScanSimu: directory %s already exists.\n", datDir1);
	}
	else {
		fprintf(stderr, "DasScanSimu: create directory %s failed.\n", datDir1);
	}

	if (hFile != NULL) {
		CloseHandle(hFile);
	}

	if (hFile != NULL) {
		CloseHandle(hSrc);
	}

	SetCurrentDirectory(currPath);	 /* Restore working directory. */
}

void CALLBACK ScanStop(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	printf("Scan Finished\n");

	if (StrCmp(protocol, "Drift.xml") == 0){
		copyPdFile("Drift1");
		SleepEx(1000, TRUE);
		copyPdFile("Drift2");
	}
	else if (StrCmp(protocol, "Leakage.xml") == 0){
		copyPdFile("Leakage1");
		SleepEx(1000, TRUE);
		copyPdFile("Leakage2");
	}
	else if (StrCmp(protocol, "mARatio.xml") == 0){
		copyPdFile("mARatio1");
		SleepEx(1000, TRUE);
		copyPdFile("mARatio2");
	}
	else if (StrCmp(protocol, "RD.xml") == 0){
		copyPdFile("RD1");
		SleepEx(1000, TRUE);
		copyPdFile("RD2");
		SleepEx(1000, TRUE);
		copyPdFile("RD3");
	}
	else if (StrCmp(protocol, "Spectral.xml") == 0){
		copyPdFile("Spectral1");
		SleepEx(1000, TRUE);
		copyPdFile("Spectral2");
	}
	else if (StrCmp(protocol, "Z_Align.xml") == 0){
		copyPdFile("Z_Align1");
		SleepEx(1000, TRUE);
		copyPdFile("Z_Align2");
	}
	else{
		copyPdFile(protocol);
	}

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

	_snprintf_s(protocol, MAX_NAME, "%s", protocolName);
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

	//t.QuadPart = -10 * 10000000;
	t.QuadPart = -2 * 10000000;
	SetWaitableTimer(timer, &t, 0, ScanStop, NULL, FALSE);
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
