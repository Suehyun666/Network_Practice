#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    BYTE nMajor = 2, nMinor = 2;
    WORD wVersionRequested = MAKEWORD(nMinor, nMajor);

    int nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet == SOCKET_ERROR)
    {
        cout << "can't initialize socket" << endl;
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != nMinor ||
        HIBYTE(wsaData.wVersion) != nMajor)
    {
        cout << "window socket " << nMajor << "." << nMinor << "cant use" << endl;
        WSACleanup();
        return -1;
    }
    cout << "Initialize socket!" << endl;
    WSACleanup();
    return 0;
}