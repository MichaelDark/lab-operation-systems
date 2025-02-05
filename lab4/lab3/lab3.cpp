#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "MailBox.h"
#include <tchar.h>
#include <iostream>

#define TRUE_STR _T("TRUE")
#define FALSE_STR _T("FALSE")
#define SUCCESS_STR _T("SUCCESS")
#define ERROR_STR _T("ERROR")


void WriteState(DWORD state);

int _tmain()
{
	_tsetlocale(LC_ALL, _T("Russian"));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	TCHAR* buffer = new TCHAR[MAX_PATH];
	Mailbox MyBox(_T("mailbox.txt"), 1000);
	_tprintf(_T("Integrity: %s\r\n"), MyBox.CheckIntegrity() ? TRUE_STR : FALSE_STR);
	Mailbox MyBox1(_T("mailbox_template.txt"), 1000);
	_tprintf(_T("Integrity: %s\r\n"), MyBox1.CheckIntegrity() ? TRUE_STR : FALSE_STR);
	Mailbox MyBox2(_T("mailbox_wrong_checksum.txt"), 1000);
	_tprintf(_T("Integrity: %s\r\n"), MyBox2.CheckIntegrity() ? TRUE_STR : FALSE_STR);
	Mailbox MyBox3(_T("mailbox_wrong_size.txt"), 1000);
	_tprintf(_T("Integrity: %s\r\n"), MyBox3.CheckIntegrity() ? TRUE_STR : FALSE_STR);

	_tprintf(_T("\r\n\r\n===WRITING\r\n\r\n"));
	WriteState( MyBox.WriteEnd(_T("111")) );
	WriteState( MyBox.WriteEnd(_T("222")) );
	WriteState( MyBox.WriteEnd(_T("333")) );
	WriteState( MyBox.WriteBegin(_T("444")) );
	WriteState( MyBox.WriteBegin(_T("555")) );
	WriteState( MyBox.Write(_T("666"), 5) );
	WriteState( MyBox.Write(_T("777"), 10) );
	WriteState( MyBox.Write(_T("888"), -1) );
	WriteState( MyBox.Write(_T("999"), 1) );

	_tprintf(_T("\r\n\r\n===DELITING\r\n\r\n"));
	WriteState( MyBox.Delete(2, buffer) );
	_tprintf(_T("Deleted #2: %s\r\n"), buffer);
	WriteState(MyBox.Delete(2, buffer));
	_tprintf(_T("Deleted #2: %s\r\n"), buffer);
	WriteState(MyBox.Delete(2, 0));

	for (int i = 0; i < MyBox.MessageCount(); i++)
	{
		_tprintf(_T("Mailbox[%d]: %s\r\n"), i, MyBox[i]);
	}

	_tprintf(_T("%d"), MyBox.Checksum());

	//MyBox.Clear();

	system("pause");
}

void WriteState(DWORD state)
{
	switch (state)
	{
	case TRUE:
	{
		_tprintf(_T("State: %s\t\n"), SUCCESS_STR);
		return;
	}
	case FALSE:
	{
		_tprintf(_T("State: %s\t\n"), ERROR_STR);
		return;
	}
	}
	return;
}


//MAKE FIRST BYTE 0x5F
/*HANDLE fileHandle = CreateFile(_T("mailbox_wrong_checksum.txt"), GENERIC_READ | GENERIC_WRITE,
FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
BYTE a = 0x5f;
WriteFile(fileHandle, &a, 1, 0, 0);
CloseHandle(fileHandle);*/

//MAKE EOF 4 BYTES EARLIER
/*HANDLE fileHandle = CreateFile(_T("mailbox_wrong_size.txt"), GENERIC_READ | GENERIC_WRITE,
FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
SetFilePointer(fileHandle, -4, 0, FILE_END);
SetEndOfFile(fileHandle);
CloseHandle(fileHandle);*/