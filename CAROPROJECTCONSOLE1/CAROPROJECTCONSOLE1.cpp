#include "View.h"
#include "Control.h"
#include "Model.h"
#include "GameState.h"
#include "HandleInput.h"
#include <conio.h>
#include <ctype.h>
#include <iostream>
#include <windows.h>
#include "Save&load.h" 

using namespace std;

GameState currentState = MENU;

int main() {
    FixConsoleWindow();
    InitConsole();

    while (true) {
        switch (currentState) {

            // --- MENU CHÍNH ---
        case MENU: {
            int choice = HandleMainMenuInput();

            switch (choice) {
            case 0: // PLAY
                SetCursorVisible(true);
                StartGame();
                currentState = PLAY;
                break;
            case 1: currentState = LOAD; break;
            case 2: currentState = SETTINGS; break;
            case 3: currentState = GUIDE; break;
            case 4: currentState = ABOUT; break;
            case 5: currentState = EXIT; break;
            }
            break;
        }

                 // --- TRẠNG THÁI CHƠI GAME ---
        case PLAY: {
            if (_kbhit()) {
                bool validEnter = true;
                _COMMAND = toupper(_getch());

                if (_COMMAND == 27) { // ESC → Pause Menu
                    currentState = PAUSE;
                    SetCursorVisible(false);
                    break;
                }

                // Di chuyển con trỏ
                else if (_COMMAND == 'A') MoveLeft();
                else if (_COMMAND == 'D') MoveRight();
                else if (_COMMAND == 'W') MoveUp();
                else if (_COMMAND == 'S') MoveDown();

                else if (_COMMAND == 13) { // ENTER đánh quân
                    GotoXY(_X, _Y);
                    int checkResult = CheckBoard(_X, _Y);
                    switch (checkResult) {
                    case -1: std::cout << "X"; break;
                    case 1:  std::cout << "O"; break;
                    case 0:  validEnter = false; break;
                    }

                    if (validEnter) {
                        int status = ProcessFinish(TestBoard());
                        if (status == -1 || status == 1 || status == 0) {
                            if (AskContinue() != 'Y') {
                                currentState = MENU;
                                SetCursorVisible(false);
                            }
                            else {
                                StartGame();
                                SetCursorVisible(true);
                            }
                        }
                    }
                    GotoXY(_X, _Y);
                    validEnter = true;
                }
            }
            Sleep(15);
            break;
        }

                 /* --- PAUSE MENU ---*/
        case PAUSE: {
            SetCursorVisible(false);

            int pauseChoice = HandlePauseMenuInput();

            switch (pauseChoice) {

            case 0: // Resume
                // --- SỬA LỖI RESUME: Đặt lại con trỏ về vị trí cũ ---
                system("cls");
                DrawBoard(BOARD_SIZE);
                RedrawBoardState();

                SetCursorVisible(true);
                GotoXY(_X, _Y);         // <-- ĐƯA CON TRỎ VỀ VỊ TRÍ CŨ
                currentState = PLAY;
                break;

            case 1: // Restart
                SetCursorVisible(true);
                StartGame();
                currentState = PLAY;
                break;

            case 2: { // Save
                system("cls");
                GotoXY(10, 5);
                std::cout << "Nhập tên file để lưu (vd: mygame1, sau đó nhán Enter): ";
                std::string saveName;
                std::cin >> saveName;

                // Dọn bộ đệm input sau khi nhập tên file
                std::cin.ignore(200, '\n');

                // THỰC HIỆN SAVE
                saveGame(saveName);

                // Sau khi Save xong, hiển thị lại menu PAUSE
                currentState = PAUSE;
                break;
            }

                  // case 3: Settings (Chức năng này được chuyển xuống khối dưới)

            case 4: // Quit (Về Menu chính)
                system("cls");
                currentState = MENU;
                SetCursorVisible(false);
                break;
            }
            break;
        }

                  // --- TRẠNG THÁI LOAD GAME (ĐÃ TÁCH KHỎI SETTINGS) ---
        case LOAD: {
            SetCursorVisible(false);

            // 1. Gọi hàm để liệt kê file và chọn tên file
            string fileNameToLoad = listAndGetFileName();

            if (!fileNameToLoad.empty()) {
                // 2. Nếu chọn được file, thực hiện tải
                bool loadSuccess = loadGame(fileNameToLoad);

                if (loadSuccess) {
                    // 3. Nếu tải thành công, thiết lập lại giao diện
                    system("cls");
                    DrawBoard(BOARD_SIZE);
                    RedrawBoardState();

                    // Thiết lập con trỏ về ô [0][0] của bàn cờ
                    _X = _A[0][0].x;
                    _Y = _A[0][0].y;

                    SetCursorVisible(true);
                    GotoXY(_X, _Y);
                    currentState = PLAY;
                }
                else {
                    // Tải thất bại
                    currentState = MENU;
                }
            }
            else {
                // Hủy tải
                currentState = MENU;
            }
            break;
        }

                 // --- CÁC TRẠNG THÁI KHÁC (Chờ phím ESC để thoát) ---
        case SETTINGS:
        case GUIDE:
        case ABOUT:
            // Mỗi trạng thái này cần có hàm Draw riêng để hiển thị nội dung.
            // Hiện tại chỉ xử lý thoát bằng ESC.
            if (_kbhit() && toupper(_getch()) == 27) { // 27 = ESC
                currentState = MENU;
                SetCursorVisible(false);
            }
            break;

            // --- THOÁT ---
        case EXIT:
            ExitGame();
            return 0;
        }
    }
}