#include "Save&load.h"
#include <vector>     
#include <conio.h>    
#include <limits>     
#include <iostream>
#include <string>
#include <windows.h>
#include <io.h> 

using namespace std;

string listAndGetFileName() {
    system("cls");
    cout << "\n--- DANH SACH MAN CHOI DA LUU ---" << endl;

    vector<string> savedFiles;
    int index = 1;

    string searchPath = SAVE_DIR + "*.txt";
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << " Chua co man choi nao duoc luu hoac loi truy cap vao thu muc." << endl;
        cout << "\nNhan phim bat ky de quay lai menu chinh";
        (void)_getch(); // Khac phuc C6031
        return "";
    }

    do {
        string filename(findFileData.cFileName);

        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (filename.length() > 4) {
                string baseName = filename.substr(0, filename.length() - 4);
                savedFiles.push_back(baseName);
                cout << index++ << ". " << baseName << endl;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);
    FindClose(hFind);

    if (savedFiles.empty()) {
        cout << "Chua co man choi nao duoc luu." << endl;
        cout << "\nNhan phim bat ky de quay lai menu chinh";
        (void)_getch(); // Khac phuc C6031
        return "";
    }

    // --- PHAN CHON FILE (DA TOI UU HOA XU LY LOI INPUT) ---
    int choice = -1;
    do {
        cout << "\nChon so thu tu man can tai (Nhap 0 de huy): ";

        // Su dung cu phap chuan de don bo dem
        //cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            choice = -1;
            cout << "Loi nhap, vui long nhap lai so thu tu." << endl;
        }
        else if (choice == 0) {
            return "";
        }
        else if (choice < 1 || choice >(int)savedFiles.size()) {
            cout << "So thu tu khong hop le, vui long nhap lai." << endl;
        }
    } while (choice < 1 || choice >(int)savedFiles.size());

    return savedFiles[choice - 1];
}
