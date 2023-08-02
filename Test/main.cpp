#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
//#include "mingw.thread.h"

std::atomic<bool> fileFound(false);

void searchFileByName(const std::wstring& directory, const std::wstring& filename) {
    WIN32_FIND_DATAW wfd;
    HANDLE hFind = FindFirstFileW((directory + L"\\*").c_str(), &wfd);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        std::wstring currentFile = wfd.cFileName;
        if (currentFile != L"." && currentFile != L"..") {
            if (currentFile == filename) {
                std::wcout << "Found file: " << directory << L"\\" << currentFile << std::endl;
                fileFound = true;
                break; 
            }

            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                searchFileByName(directory + L"\\" + currentFile, filename);
            }
        }
    } while (!fileFound && FindNextFileW(hFind, &wfd) != 0);

    FindClose(hFind);
}

int main() {
    std::wstring rootDirectory = L"G:\\"; 
    std::wstring targetFilename = L"H_matrix.m";

    const int numThreads = 8; 
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(searchFileByName, rootDirectory, targetFilename);
    }

    for (std::thread& t : threads) {
        t.join();
    }

    system("pause");
    return 0;
}