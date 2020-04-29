#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<string>
#include <fstream>
#include <cstring> // for std::strlen
#include <cstddef> // for std::size_t -> is a typedef on an unsinged int


using namespace std;

bool WriteComPort(HANDLE hPort, const char* data)
{
    DCB dcb;
    DWORD byteswritten;

    if (!GetCommState(hPort, &dcb))
        return false;
    dcb.BaudRate = CBR_115200; //9600 Baud
    dcb.ByteSize = 8; //8 data bits
    dcb.Parity = NOPARITY; //no parity
    dcb.StopBits = ONESTOPBIT; //1 stop
    if (!SetCommState(hPort, &dcb))
        return false;
    bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
    CloseHandle(hPort); //close the handle
    return retVal;
}

int ReadByte()
{
    DCB dcb;
    int retVal;
    BYTE Byte;
    DWORD dwBytesTransferred;
    DWORD dwCommModemStatus;

    HANDLE hPort;

    hPort = CreateFile(L"COM3",                //port name
        GENERIC_READ | GENERIC_WRITE, //Read/Write
        0,                            // No Sharing
        NULL,                         // No Security
        OPEN_EXISTING,// Open existing port only
        0,            // Non Overlapped I/O
        NULL);        // Null for Comm Devices


    if (!GetCommState(hPort, &dcb))
        return 0x100;
    dcb.BaudRate = CBR_115200; //9600 Baud
    dcb.ByteSize = 8; //8 data bits
    dcb.Parity = NOPARITY; //no parity
    dcb.StopBits = ONESTOPBIT; //1 stop
    if (!SetCommState(hPort, &dcb))
        return 0x100;
    SetCommMask(hPort, EV_RXCHAR | EV_ERR); //receive character event
    WaitCommEvent(hPort, &dwCommModemStatus, 0); //wait for character
    if (dwCommModemStatus & EV_RXCHAR)
        ReadFile(hPort, &Byte, 1, &dwBytesTransferred, 0); //read 1
    else if (dwCommModemStatus & EV_ERR)
        retVal = 0x101;
    retVal = Byte;
    CloseHandle(hPort);
    return retVal;
}

int main()
{


    ifstream inFile;
    size_t size = 0; // here

    inFile.open("test.txt", ios::in | ios::binary | ios::ate);
    char* oData = 0;

    inFile.seekg(0, ios::end); // set the pointer to the end
    size = inFile.tellg(); // get the length of the file
    cout << "Size of file: " << size;
    inFile.seekg(0, ios::beg); // set the pointer to the beginning

    oData = new char[size + 1]; //  for the '\0'
    inFile.read(oData, size);
    oData[size] = '\0'; // set '\0' 
    cout << " oData size: " << strlen(oData) << "\n";

    //print data
    for (size_t i = 0; i < strlen(oData); i++)
    {
        cout << "oData[" << i << "] " << oData[i];
        cout << "\n";
        cout << oData[i] << " + 'a' = " << (oData[i] + 'a');
        cout << "\n\n";

    }
    getchar();

    cout << "\n" << ReadByte();

    return 0;
}
