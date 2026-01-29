#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Use Format : > Hostent Domain name" << endl;
        return 0;
    }

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


    struct hostent *host;
    host = gethostbyname(argv[1]);
    if(host == NULL)
    {
        cout << "ERROR : 'gethostbyname'" << endl;
        WSACleanup();
        return 0;
    }
    
    cout << "Initialize socket!" << endl;
    
    int i = 0;
    while (host->h_aliases[i] != NULL)
    {
        cout << "aliases name: \t\t" << host->h_aliases <<endl;
        i++;
    }

    cout << "host address type : \t\t" << host->h_addrtype << endl;
    cout << "length of host address : \t" << host->h_length << endl;
    
    i = 0;
    while (host->h_addr_list[i] != NULL)
    {
        struct in_addr inaddr; /* IP 주소를 저장할 구조체 */
        inaddr.s_addr = *((u_long *)(host->h_addr_list[i]));
        cout << "IP Adress : \t\t" << inet_ntoa(inaddr) << endl;
        i++;
    }

    WSACleanup();
    
    return 0;
}