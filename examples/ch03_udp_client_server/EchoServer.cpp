#include <iostream>
#include <WinSock2.h>
#include <time.h>

using namespace std;

#pragma comment(lib, "WS2_32.LIB")

#define BUF_LEN 256
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

    SOCKET sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    try
    {
        if (sock == INVALID_SOCKET)
            throw "Failed to instantiate listen socket";

        struct sockaddr_in addr;
        addr.sin_family     = AF_INET;
        addr.sin_port       = htons (42002);
        addr.sin_addr.s_addr= htonl (INADDR_ANY);
        if (bind(sock, (struct sockaddr*) &addr, sizeof(addr))
             == SOCKET_ERROR)
            throw "bind Error";

        while (1)
        {
            struct sockaddr_in cliaddr;
            int addrlen = sizeof(cliaddr);
            char buf[BUF_LEN+1] = {0};
            int n = recvfrom(sock, buf, BUF_LEN, 0,
                (struct sockaddr*) &cliaddr, &addrlen);
            if (n == SOCKET_ERROR)
            {
                cout << "recvfrom error" << endl;
                continue;
            }

            cout << "MSG: (" << inet_ntoa(cliaddr.sin_addr) << ":"
                << ntohs(cliaddr.sin_port) << ") --> " << buf <<endl;

            sendto (sock, buf, n, 0,
                (struct sockaddr*) &cliaddr, addrlen);

            if (strcmp (buf, "QUIT") == 0)
                break;
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

    if (sock != INVALID_SOCKET)
            closesocket (sock);

    WSACleanup();

    return 0;
}