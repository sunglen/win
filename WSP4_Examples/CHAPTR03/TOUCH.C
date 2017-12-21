/* Chapter 3. touch command. */
/* Modified Sept 2, 2005 to fix the -a and -m logic 
     Modified again Jan 4, 2016 - the 2005 change was too complex.*/
/* touch[options] files
	Update the access and modification times of the files.
	Create non-existent files.
	Options:
		-a Only update the access time.
		-m Only update the modification (last write) time.
		         But, both are allowed, so the "only" is not accurate.
		-c Do not create a non-existent file. */

/* This program illustrates:
	1. Setting the file time attributes.
	2. Converting from system to file time. */

#include "Everything.h"

int _tmain (int argc, LPTSTR argv[])
{
	FILETIME newFileTime;
	LPFILETIME pAccessTime = NULL, pModifyTime = NULL;
	HANDLE hFile;
	BOOL setAccessTime, setModTime, notCreateNew;
	DWORD CreateFlag;
	int i, FileIndex;

	DWORD pathLength;
	TCHAR currPath[MAX_PATH_LONG + 1];

	SYSTEMTIME currentTime;
	TCHAR datName[MAX_NAME + 1], datDir1[MAX_NAME + 1], datDir2[MAX_NAME + 1], datFile[MAX_NAME + 1];


	/* Determine the options. */

	if (argc < 2) {
		_tprintf(_T("Usage: touch[options] files"));
		return 1;
	}

	pathLength = GetCurrentDirectory(MAX_PATH_LONG, currPath);
	if (pathLength == 0 || pathLength >= MAX_PATH_LONG) { /* pathLength >= MAX_PATH_LONG (32780) should be impossible */
		ReportError(_T("GetCurrentDirectory failed"), 1, TRUE);
	}

	SetCurrentDirectory("d:\\pangu.dat\\raw\\");	 /* Restore working directory. */

	GetSystemTime(&currentTime);

	_snprintf(datName, MAX_NAME, "%04d%02d%02d%02d%02d%02d",currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
	_snprintf(datDir1, MAX_NAME, "1.%s", datName);
	_snprintf(datDir2, MAX_NAME, "2.%s", datName);
	_snprintf(datFile, MAX_NAME, "2.%s.pd", datName);

	_tprintf("%s\n%s\n%s\n", datDir1, datDir2, datFile);

	if (CreateDirectory(datDir1, NULL)){
		// Directory created
		SetCurrentDirectory(datDir1);
		CreateDirectory(datDir2, NULL);
		SetCurrentDirectory(datDir2);
		hFile = CreateFile(datFile, GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		_tprintf(_T("directory already exists"));
		// Directory already exists
	}
	else
	{
		_tprintf(_T("directory create failed"));
		// Failed for some other reason
	}

	SetCurrentDirectory(currPath);	 /* Restore working directory. */

	FileIndex = Options(argc, argv, _T ("amc"),
			&setAccessTime, &setModTime, &notCreateNew, NULL);

	CreateFlag = notCreateNew ? OPEN_EXISTING : OPEN_ALWAYS;

	for (i = FileIndex; i < argc; i++) {
		hFile = CreateFile(argv[i], GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			if (!notCreateNew) ReportError (_T ("touch error: Cannot open file."), 0, TRUE);
			continue;
		}
		/* Get current system time and convert to a file time.
			Do not change the create time. */
		GetSystemTimeAsFileTime(&newFileTime);
		if (setAccessTime) pAccessTime = &newFileTime;
		if (setModTime) pModifyTime = &newFileTime;
		if (!SetFileTime(hFile, NULL, pAccessTime, pModifyTime))
			ReportError(_T ("Failure setting file times."), 2, TRUE);
		CloseHandle(hFile);
	}
	return 0;
}
