#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

class telemetry {
public:
	int value;
	char date[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	bool finalValue = FALSE;
};

int main() {
	int port = 55555;

	//step 1: initialization of Winsock
	WSAData wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0) {
		std::cout << "Winsock DLL not found" << std::endl;
		return 0;
	}
	else {
		std::cout << "Winsock DLL found\n";
		std::cout << "Status: " << wsaData.szSystemStatus << "\n" << std::endl;
	}

	//step 2: creating socket
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error at socket() " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "socket() is ready\n" << std::endl;
	}

	//step 3: connecting to server
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cout << "Client: connect() - Failed to connect" << std::endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	else {
		std::cout << "Client: connect() is ready\n";
		std::cout << "Client: can start sending and receiving data..." << std::endl;
	}

	//chat to the server
	telemetry temp;
	char buffer[50];
	do {
		std::cout << "Please enter the temperature: ";
		std::cin >> temp.value;
		std::cout << "Please enter the date: ";
		std::cin >> temp.date;
		std::cout << "Please enter any number other than 0 if it is final value: ";
		std::cin >> temp.finalValue;

		int byteCount = send(clientSocket, (char*)&temp, sizeof(temp), 0);

		if (byteCount > 0) {
			std::cout << "Message successfully sent" << std::endl;
		}
		else {
			WSACleanup();
		}


		int byteCount2 = recv(clientSocket, buffer, 50, 0);
		if (byteCount > 0) {
			std::cout << "Responce from server: " << buffer << std::endl;
		}
		else {
			WSACleanup();
		}
	} while (temp.finalValue != TRUE);
	

	system("pause");
	WSACleanup();
	return 0;
}