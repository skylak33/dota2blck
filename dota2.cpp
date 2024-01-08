#include <windows.h>
#include <tlhelp32.h>

bool TerminateProcessByName(const char* processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, processName) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    CloseHandle(hSnapshot);
                    return true;
                }
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);
    return false;
}

int main() {
    const char* targetProcessName = "dota2.exe";

    // Скрытие окна приложения
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    // Выполнение в фоновом режиме
    while (true) {
        if (TerminateProcessByName(targetProcessName)) {
            // Действия при завершении процесса
        }
        Sleep(1000);  // Задержка 1000 миллисекунд (1 секунда) между итерациями
    }

    return 0;
}
