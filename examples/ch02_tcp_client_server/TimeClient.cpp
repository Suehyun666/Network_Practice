#include <iostream>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main (int argc, char* argv[]) {
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

    char szip[30];
    cout << "Enter the server address to connect to: ";
    cin >> szip;

    SOCKET sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    try {
        if (sock == INVALID_SOCKET)
            throw "Failed to instantiate socket";

        struct sockaddr_in serveraddr;
        serveraddr.sin_family      = AF_INET;
        serveraddr.sin_port        = htons (42001);
        serveraddr.sin_addr.s_addr = inet_addr (szip);
        if (connect (sock, (struct sockaddr*) &serveraddr,
            sizeof (serveraddr)) == SOCKET_ERROR)
            throw "connect Error";

        char sztime[100] = {0};
        recv (sock, sztime, sizeof(sztime), 0);
        cout << "Current time from server: " << sztime << endl;
    }
    catch (const char* msg)
    {
        cout << msg << endl;
        LPVOID lpOSMsg;
        FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            WSAGetLastError (),
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpOSMsg,
            0,
            NULL);
        cout << "[" << (char*) lpOSMsg << "]" <<endl;
        LocalFree (lpOSMsg);
    }

    if (sock != INVALID_SOCKET)
        closesocket(sock);

    WSACleanup();

    return 0;
}