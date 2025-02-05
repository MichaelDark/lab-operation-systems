// RSA_using_dll.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "resource.h"
#define ulong unsigned long long

typedef void (*GenerateKeys) (ulong * e, ulong * d, ulong * n);
typedef void (*CryptMessage) (wchar_t * msg, int length, ulong e, ulong n, ulong * crypto);
typedef void (*DecryptMessage) (wchar_t * msg, int length, ulong d, ulong n, ulong * crypto);

HMODULE hdl;
HMODULE hlang;

GenerateKeys _GenerateKeys;
CryptMessage _CryptMsg;
DecryptMessage _DecryptMsg;

TCHAR input[100];
TCHAR encrypted[100];
TCHAR decrypted[100];
TCHAR finished[100];

int DLLInit()
{
	hdl = LoadLibrary(_T("RSA_dll.dll"));

	if (hdl == 0)
	{
		printf("Download failure\n");
		return 1;
	}

	_GenerateKeys = (GenerateKeys)GetProcAddress(hdl, "GenerateKeys");

	if (_GenerateKeys == 0)
	{
		printf("Download failure\n");
		FreeLibrary(hdl);
		return 1;
	}

	_CryptMsg = (CryptMessage)GetProcAddress(hdl, "CryptMessage");

	if (_CryptMsg == 0)
	{
		printf("Download failure\n");
		FreeLibrary(hdl);
		return 1;
	}

	_DecryptMsg = (DecryptMessage)GetProcAddress(hdl, "DecryptMessage");

	if (_GenerateKeys == 0)
	{
		printf("Download failure\n");
		FreeLibrary(hdl);
		return 1;
	}

	return 0;
}
int InitLang(const wchar_t * lang)
{

	wchar_t dll[100];

	wsprintf(dll, _T("%s.dll"), lang);

	hlang = LoadLibrary(dll);

	if (hlang)
	{
		LoadString(hlang, IDS_INPUT, input, 100);
		LoadString(hlang, IDS_ENCRYPTED, encrypted, 100);
		LoadString(hlang, IDS_DECRYPTED, decrypted, 100);
		LoadString(hlang, IDS_FINISHED, finished, 100);
	}
	else
	{
		return 1;
	}

	return 0;
}

int main()
{
	_wsetlocale(LC_ALL, _T("Russian"));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (InitLang(_T("Russian")))
	{
		return 1;
	}

	if (DLLInit() != 0)
	{
		return 1;
	}

	ulong e, d, n;

	_GenerateKeys(&e, &d, &n);

	printf("E: %llu\n", e);
	printf("D: %llu\n", d);
	printf("N: %llu\n", n);
	
	wchar_t msg[200];

	_tprintf(_T("%s: "), input);
	std::wcin.getline(msg, 200);

	ulong *res = new ulong[wcslen(msg)];
	wchar_t *dec = new wchar_t[wcslen(msg) + 1];

	_CryptMsg(msg, wcslen(msg), e, n, res);
	_DecryptMsg(dec, wcslen(msg), d, n, res);

	_tprintf(_T("%s: "), encrypted);

	for (int i = 0; i < wcslen(msg); i++)
	{
		printf("%llu ", res[i]);
	}

	_tprintf(_T("\n%s: "), decrypted);
	_tprintf(_T("%s\n"), dec);

	delete[] res;
	delete[] dec;

	_tprintf(_T("%s\n"), finished);

	getchar();

	FreeLibrary(hlang);
	FreeLibrary(hdl);

    return 0;
}

