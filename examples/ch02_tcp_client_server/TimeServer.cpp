#include <iostream>
#include <WinSock2.h>
#include <time.h>

using namespace std;

#pragma comment(lib, "WS2_32.LIB")

int main (int argc, char* argv[])
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
    
    SOCKET lstnsock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    try
    {
        if (lstnsock == INVALID_SOCKET)
            throw "Failed to instantiate listen socket";
        
        struct sockaddr_in addr;
        addr.sin_family     = AF_INET;
        addr.sin_port       = htons (42001);
        addr.sin_addr.s_addr= htonl (INADDR_ANY);
        if (bind(lstnsock, (struct sockaddr*) &addr, sizeof(addr))
             == SOCKET_ERROR)
            throw "bind Error";
        
        if (listen(lstnsock, SOMAXCONN) == SOCKET_ERROR)
            throw "listen Error";
        
        while (1)
        {
            struct sockaddr_in cliaddr;
            int addrlen = sizeof(cliaddr);

            SOCKET commsock = accept (lstnsock,
                (struct sockaddr*) &cliaddr, &addrlen);
            
            cout << "[" << inet_ntoa(cliaddr.sin_addr) << "]";
            cout << "from Access" << endl;

            time_t long_time;
            struct tm *newtime;
            char sztime[100];

            time(&long_time);
            newtime = localtime (&long_time);

            sprintf (sztime, "%.19s\n", asctime(newtime));
            send(commsock, sztime, (int) strlen (sztime), 0);
            closesocket(commsock);
        }
    }
    catch (char* errmsg)
    {
        cout << errmsg << endl;
        LPVOID lpOSMsg;
        FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpOSMsg, 0, NULL);
    
        cout << "[" << (char*) lpOSMsg<< "]" << endl;
        LocalFree (lpOSMsg);
    }

    if (lstnsock != INVALID_SOCKET)
            closesocket (lstnsock);

    WSACleanup();

    return 0;
}