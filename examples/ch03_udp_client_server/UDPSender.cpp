#include <iostream>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define BUF_LEN 256

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

    char szip[30] = "224.0.0.1";
    // cout << "Enter the echo server address to connect to: ";
    // cin >> szip;
    // cin.ignore();

    SOCKET sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    try {
        if (sock == INVALID_SOCKET)
            throw "Failed to instantiate socket";

        struct sockaddr_in serveraddr;
        serveraddr.sin_family      = AF_INET;
        serveraddr.sin_port        = htons (42002);
        serveraddr.sin_addr.s_addr = inet_addr (szip);

        while (1)
        {
            cout << "Out Data : ";
            char buf[BUF_LEN+1] = {0};
            cin.getline(buf, BUF_LEN);

            int n = sendto (sock, buf, strlen(buf), 0,
                (struct sockaddr*) &serveraddr, sizeof(serveraddr));
            if (n == SOCKET_ERROR)
            {
                cout << "sendto error" <<endl;
                continue;
            }

            memset(buf, 0x00, BUF_LEN+1);
            struct sockaddr_in addr2 = {0};
            int addrlen = sizeof(addr2);
            n = recvfrom(sock, buf, BUF_LEN, 0,
                (struct sockaddr*) &addr2, &addrlen);
            if (n == SOCKET_ERROR)
            {
                cout << "recvfrom error" <<endl;
                continue;
            }

            cout << "In Data: " << buf << endl;

            if (strcmp (buf, "QUIT") == 0)
                break;
        }
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