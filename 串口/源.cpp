#include <windows.h> 
#include <stdio.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <tchar.h>
#include <afx.h>
#include <afxwin.h>


int main()
{
#define MAX_KEY_LENGTH   255
#define MAX_VALUE_NAME  16383
	HKEY hTestKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey)){
		TCHAR   achClass[MAX_PATH] = TEXT(""); // buffer for class name
		DWORD   cchClassName = MAX_PATH; // size of class string
		DWORD   cSubKeys = 0;                  // number of subkeys
		DWORD   cbMaxSubKey;             // longest subkey size
		DWORD   cchMaxClass;             // longest class string
		DWORD   cValues;             // number of values for key
		DWORD   cchMaxValue;         // longest value name
		DWORD   cbMaxValueData;      // longest value data
		DWORD   cbSecurityDescriptor; // size of security descriptor
		FILETIME ftLastWriteTime;     // last write time

		DWORD i, retCode;
		TCHAR achValue[MAX_VALUE_NAME];
		DWORD cchValue = MAX_VALUE_NAME;

		// Get the class name and the value count.
		retCode = RegQueryInfoKey(
			hTestKey,                   // key handle
			achClass,               // buffer for class name
			&cchClassName,          // size of class string
			NULL,                   // reserved
			&cSubKeys,              // number of subkeys
			&cbMaxSubKey,           // longest subkey size
			&cchMaxClass,           // longest class string
			&cValues,               // number of values for this key
			&cchMaxValue,           // longest value name
			&cbMaxValueData,        // longest value data
			&cbSecurityDescriptor,  // security descriptor
			&ftLastWriteTime);      // last write time

		if (cValues > 0) {
			for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++) {
				cchValue = MAX_VALUE_NAME; achValue[0] = '\0';
				if (ERROR_SUCCESS == RegEnumValue(hTestKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL)) {
					CString szName(achValue);
					if (-1 != szName.Find(_T("Serial")) || -1 != szName.Find(_T("VCom"))){
						BYTE strDSName[10]; memset(strDSName, 0, 10);
						DWORD nValueType = 0, nBuffLen = 10;
						if (ERROR_SUCCESS == RegQueryValueEx(hTestKey, (LPCTSTR)achValue, NULL,
							&nValueType, strDSName, &nBuffLen)){
							int nIndex = -1;
							while (++nIndex < 20){
								if (-1 == m_nComArray[nIndex]) {
									m_nComArray[nIndex] = atoi((char*)(strDSName + 3));
									break;
								}
							}
						}
					}
				}
			}
		}
		else{
			AfxMessageBox(_T("机PC机没有COM口....."));
		}
	}
	RegCloseKey(hTestKey);

}