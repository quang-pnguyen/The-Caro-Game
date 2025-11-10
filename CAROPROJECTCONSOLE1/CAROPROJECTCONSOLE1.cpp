#include "View.h"
#include "Control.h"
#include "Model.h"
#include "GameState.h"
#include "HandleInput.h"
#include <conio.h>
#include <ctype.h>
#include <iostream>
#include <windows.h>
#include <limits> 
#include "Save&load.h"

using namespace std;

#ifdef max
#undef max
#endif 
// -------------------------------------------------------------

GameState currentState = MENU;

int main() {
    FixConsoleWindow();
    InitConsole();

    while (true) {
        switch (currentState) {

            // --- MENU CHINH ---
        case MENU: {
            int choice = HandleMainMenuInput();

            switch (choice) {
            case 0: // NEW GAME
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

                 // --- TRANG THAI CHOI GAME ---
        case PLAY: {
            if (_kbhit()) {
                bool validEnter = true;
                _COMMAND = toupper(_getch());

                if (_COMMAND == 27) { // ESC → Pause Menu
                    currentState = PAUSE;
                    SetCursorVisible(false);
                    break;
                }

                // Di chuyen con tro
                else if (_COMMAND == 'A') MoveLeft();
                else if (_COMMAND == 'D') MoveRight();
                else if (_COMMAND == 'W') MoveUp();
                else if (_COMMAND == 'S') MoveDown();

                else if (_COMMAND == 13) { // ENTER danh quan
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
                // SUA LOI RESUME: Dat lai con tro ve vi tri cu
                system("cls");
                DrawBoard(BOARD_SIZE);
                RedrawBoardState();

                SetCursorVisible(true);
                GotoXY(_X, _Y);
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
              
                std::cout << "Nhap ten file de luu (vd: mygame1, sau do nhan Enter): ";
                std::string saveName;
                std::cin >> saveName;

           
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            

                saveGame(saveName);
                currentState = PAUSE;
                break;
            }

            case 4: // Quit (Ve Menu chinh)
                system("cls");
                currentState = MENU;
                SetCursorVisible(false);
                break;
            }
            break;
        }

                  // --- TRANG THAI LOAD GAME ---
        case LOAD: {
            SetCursorVisible(false);

            string fileNameToLoad = listAndGetFileName();

            if (!fileNameToLoad.empty()) {
                bool loadSuccess = loadGame(fileNameToLoad);

                if (loadSuccess) {
                    // Neu tai thanh cong, thiet lap lai giao dien
                    system("cls");
                    DrawBoard(BOARD_SIZE);
                    RedrawBoardState();

                    // Dat con tro ve vi tri da duoc cap nhat trong loadGame
                    SetCursorVisible(true);
                    GotoXY(_X, _Y);
                    currentState = PLAY;
                }
                else {
                    // Tai that bai (loadGame in thong bao loi va cho Enter)

                    currentState = MENU;
                }
            }
            else {
                // Huy tai
                currentState = MENU;
            }
            break;
        }

                 // --- CAC TRANG THAI KHAC (Cho phim ESC de thoat) ---
        case SETTINGS:
        case GUIDE:
        case ABOUT:
            if (_kbhit() && toupper(_getch()) == 27) {
                currentState = MENU;
                SetCursorVisible(false);
            }
            break;

            // --- THOAT ---
        case EXIT:
            ExitGame();
            return 0;
        }
    }
}
