#include "Save&load.h"
#include "model.h"  
#include "control.h" 
#include <iostream>
#include <conio.h>   // Cho _getch()

using namespace std;

// HÀM HỖ TRỢ ĐỂ CHUYỂN ĐỔI INT SANG BOOL
void setTurnFromInt(int turnInt) {
    _TURN = (turnInt == -1);
}

bool loadGame(const string& filename) {
    string fullPath = SAVE_DIR + filename + ".txt";

    // --- DEBUG LOAD GAME ---
   
    ifstream file(fullPath);

    if (!file.is_open()) {
        cout << "LOI: Khong the mo file: " << fullPath << endl;
        cout << "Nhan phim bat ky de ve Menu chinh..." << endl;
        (void)_getch(); // Dung man hinh
        return false;
    }

    int temp_size;
    int temp_turn;

    // 1. Doc kich thuoc ban co
    if (!(file >> temp_size)) {
        file.close();
        cout << "LOI: Khong doc duoc kich thuoc board (board size) tu file." << endl;
        (void)_getch();
        return false;
    }
    // --- DEBUG 2: KIEM TRA KÍCH THƯỚC BOARD ---
    //cout << "Kich thuoc doc duoc: " << temp_size << endl;
    if (temp_size != BOARD_SIZE) {
        file.close();
        cout << "LOI: Kich thuoc board (" << temp_size << ") khong khop voi cau hinh hien tai (" << BOARD_SIZE << ")." << endl;
        (void)_getch();
        return false;
    }
    // ---------------------------------------------

    // 2. Doc nguoi choi hien tai
    if (!(file >> temp_turn)) {
        file.close();
        cout << "LOI: Khong doc duoc luot choi (turn) tu file." << endl;
        (void)_getch();
        return false;
    }
    // --- DEBUG 3: IN RA LƯỢT CHƠI ĐÃ ĐỌC ---
    //cout << "Luot choi doc duoc: " << temp_turn << endl;
    // --------------------------------------
    setTurnFromInt(temp_turn);

    // 3. Doc trang thai ban co
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!(file >> _A[i][j].c)) {
                file.close();
                cout << "LOI: Khong doc duoc ma tran ban co tai vi tri (" << i << "," << j << "). File bi thieu du lieu." << endl;
                (void)_getch();
                return false;
            }
        }
    }

    // CẬP NHẬT TỌA ĐỘ CON TRỎ MÀN HÌNH SAU KHI TẢI
    _X = _A[0][0].x;
    _Y = _A[0][0].y;

    file.close();
    //cout << "Game da duoc tai thanh cong! Nhan phim bat ky de tiep tuc." << endl;
    //(void)_getch();
    return true;
}
