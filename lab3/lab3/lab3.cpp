#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "MailBox.h"
#include <tchar.h>
#include <iostream>

using namespace std;
//int MAILBOX::count = 0;
int _tmain()
{
	Mailbox MailBox(_T(" mb.txt"), 1000);

	MailBox += _T("One");
	MailBox += _T("Two");
	MailBox.WriteEnd(_T("hiii"));
	MailBox.WriteBegin(_T("hi"));
	
	/*
	TCHAR Msg[80];
	for (int i = 0; i < MailBox.ReadCounts(); ++i)
	{
		MailBox.RdMsg(Msg, i);
		_tprintf(_T(" %s\n"), Msg);
	}
	_tprintf(_T(" \n"));
	_tprintf(_T(" \n"));
	MailBox -= 2;
	for (int i = 0; i < MailBox.ReadCounts(); ++i)
	{
		MailBox.RdMsg(Msg, i);
		_tprintf(_T(" %s\n"), Msg);
	}
	*/
	system("pause");
}