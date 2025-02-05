#include "stdafx.h"
#include <locale.h>
#include <tchar.h>
#include <Windows.h>
#include <stdlib.h>
//#include <stdio.h>

void reverseFile(TCHAR* inpath, TCHAR* outpath);
void reverse(FILE* file_input, FILE *file_output);
void reverseUnicode(FILE* file_input, FILE* file_output);
int compare(const void * x1, const void * x2);

int main()
{
	_tsetlocale(LC_ALL, _T("Russian"));

	/*
	* Определение размеров типов
	*/
	int size_char = sizeof(char);
	int size_wchar_t = sizeof(wchar_t);
	int size_tchar = sizeof(TCHAR);

	/*
	* Вывод на экран размеров типов
	* и определение кодировки
	*/
	_tprintf(_T("=====SYMBOLS WEIGHT===== \r\n"));
	_tprintf(_T(" [char]    bytes: %d \r\n"), size_char);
	_tprintf(_T(" [wchar_t] bytes: %d \r\n"), size_wchar_t);
	_tprintf(_T(" [TCHAR]   bytes: %d \r\n"), size_tchar);
	_tprintf(_T(" CURRENT CHARSET: %s \r\n"),
		size_char == size_tchar ? _T("ANSI") : _T("UNICODE"));
	_tprintf(_T("\r\n\r\n"));

	/*
	* Инциализация массива имен,
	* определение его размеров
	*/
	char names[3][30] = { "Темнохуд Михаил Игоревич", "Темнодух Игорил Михович", "Temnokhud Mykhailo Ihorovich" };
	size_t nameslen = sizeof(names) / sizeof(names[0]);

	/*
	* Вывод на экран массива <-names[]->
	*/
	_tprintf(_T("=====ARRAY: char NAMES===== \r\n\r\n"));
	for (int i = 0; i < nameslen; i++)
	{
		printf("[%2d]: %s \r\n", i, names[i]);
	}
	_tprintf(_T("\r\n\r\n"));

	/*
	* Конвертирование массива <-names[]-> в массив <-tnames[]->
	* из ANSI в UNICODE
	*/
	wchar_t tnames[3][30];
	size_t tnameslen = sizeof(tnames) / sizeof(tnames[0]);
	for (int i = 0; i < nameslen; i++)
	{
		MultiByteToWideChar(CP_ACP, 0, names[i], sizeof(names[i]), tnames[i], sizeof(names[i]) * 2);
	}

	/*
	* Вывод на экран массива <-tnames[]->
	*/
	_tprintf(_T("=====ARRAY: wchar_t TNAMES===== \r\n\r\n"));
	for (int i = 0; i < tnameslen; i++)
	{
		_tprintf(_T("[%2d]: %s \r\n"), i, tnames[i]);
		//ONLY IF UNICODE
		//MessageBox(0, tnames[i], L"tnames", MB_OK);
	}
	_tprintf(_T("\r\n\r\n"));

	/*
	* Вывод на экран отсортированного массива <-tnames[]->
	*/
	qsort(&tnames, tnameslen, sizeof(tnames[0]), compare);
	_tprintf(_T("=====ARRAY: wchar_t TNAMES SORTED===== \r\n\r\n"));
	for (int i = 0; i < tnameslen; i++)
	{
		_tprintf(_T("[%2d]: %s \r\n"), i, tnames[i]);
		//ONLY IF UNICODE
		//MessageBox(0, tnames[i], L"tnames", MB_OK);
	}
	_tprintf(_T("\r\n\r\n"));

	/*
	* Конвертирование массива <-tnames[]-> в массив <-cnames[]->
	* из UNICODE в ANSI
	*/
	char cnames[3][30];
	size_t cnameslen = sizeof(cnames) / sizeof(cnames[0]);
	for (int i = 0; i < tnameslen; i++)
	{
		WideCharToMultiByte(CP_ACP, 0, tnames[i], sizeof(tnames[i]), cnames[i], sizeof(tnames[i]) / 2, NULL, NULL);
	}

	/*
	* Вывод на экран массива <-cnames[]->
	*/
	_tprintf(_T("=====ARRAY: char CNAMES===== \r\n\r\n"));
	for (int i = 0; i < cnameslen; i++)
	{
		printf("[%2d]: %s \r\n", i, cnames[i]);
	}
	_tprintf(_T("\r\n\r\n"));

	//revfileUnicode(_T("reverse.txt"), _T("reverse_out.txt"));
	reverseFile((TCHAR*)_T("reverse.txt"), (TCHAR*)_T("reverse_out.txt"));

	getchar();
	return 0;
}

void reverse(FILE* file_input, FILE *file_output)
{
	TCHAR file_data[100][100];
	int counter = 0;
	TCHAR buffer[100];
	while ((_fgetts(buffer, 100, file_input)))
	{
		int c, i, j, remainder;
		j = _tcslen(buffer);
		for (i = 0, j = _tcslen(buffer) - 2; i < j; i++, j--) {
			c = buffer[i];
			buffer[i] = buffer[j];
			buffer[j] = c;
		}
		_tcscpy(file_data[counter++], buffer);
	}
	for (int i = 0; i < counter; ++i) {
		_fputts(file_data[i], file_output);
	}
}

void reverseUnicode(FILE* file_input, FILE* file_output)
{
	wchar_t file_data[100][100];
	int counter = 0;
	wchar_t buffer[100];
	while ((fgetws(buffer, 100, file_input)))
	{
		int c, i, j;
		for (i = 0, j = wcslen(buffer) - 2; i < j; i++, j--) {
			c = buffer[i];
			buffer[i] = buffer[j];
			buffer[j] = c;
		}
		wcscpy(file_data[counter++], buffer);
	}
	for (int i = 0; i < counter; ++i) {
		fputws(file_data[i], file_output);
	}
}

void reverseFile(TCHAR* inpath, TCHAR* outpath)
{
	FILE *file_input = _tfopen(inpath, _T("r"));
	FILE *pFile;
	if (file_input != NULL)
	{
		unsigned char b[3] = { 0 };
		fread(b, 1, 2, file_input);
		if ((b[0] == 0xEF && b[1] == 0xBB) ||
			(b[0] == 0xFE && b[1] == 0xFF) ||
			(b[0] == 0xFF && b[1] == 0xFE))
		{
			pFile = _tfopen(outpath, _T("w,ccs=UTF-8"));
			fclose(file_input);
			file_input = _tfopen(inpath, _T("r,ccs=UTF-8"));
			reverseUnicode(file_input, pFile);
		}
		else
		{
			pFile = _tfopen(outpath, _T("w"));
			rewind(file_input);
			reverse(file_input, pFile);
		}
	}

	fclose(file_input);
	fclose(pFile);
}

int compare(const void * x1, const void * x2)
{
	const wchar_t* first_string = (const wchar_t*)x1;
	const wchar_t* second_string = (const wchar_t*)x2;
	return wcscmp(first_string, second_string);
}