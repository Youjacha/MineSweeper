#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>

#define MINE -1

#define FLAG 70
#define ENTER 13
#define isArrowKeys -32
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

typedef struct minefield {
    int col;
    int row;
    int frequency;
} minefield;

minefield MINEFIELD[4] = { //minefield형 배열 
    {8, 8, 10}, {14, 14, 35}, {20, 20, 80}, {8, 8, 10} };

int diff = 1; //기본 난이도
bool is_game = false; 

int hiddenBoard[24][20]; 
char gameBoard[24][20]; 

int chosenXPos = 0;
int chosenYPos = 0; 

void set_mine(struct minefield m); //지뢰 배치
void check_mine(struct minefield m); //주변 지뢰 위치 확인
void print_scr(struct minefield m); //화면 업데이트
void custom_field(struct minefield* m); //난이도 커스텀
int change_game_option(); //옵션 변경
void game_play(); //게임 시작
void set_diff(int* diff); //난이도 변경
void get_key_move(minefield m); //키 움직임 감지
void raise_flag();
void open_tile();
void game_end(struct minefield m); //게임 끝
void printby_color(int color, char text); //12 - 빨강, 15 - 하양
void clear_scr(int time); //화면 초기화 
void init(minefield m);

int main() {
    srand((unsigned)time(NULL)); 
    int game;
    while (1) {
        game = change_game_option();
        switch (game) {
        case 1:
            game_play();
            continue;
        case 2:
            set_diff(&diff);
            continue;
        case 3:
            custom_field(&MINEFIELD[3]);
            continue;
        case 4:
            exit(1);
        }
    }
}

void game_play() {
    init(MINEFIELD[diff]);
    print_scr(MINEFIELD[diff]);
    set_mine(MINEFIELD[diff]);
    check_mine(MINEFIELD[diff]);
    get_key_move(MINEFIELD[diff]);
}

void init(minefield m) {
    is_game = true;
    chosenXPos = 0;
    chosenYPos = 0;

    for (int i = 0; i < m.col; i++) { //보드 초기화
        for (int j = 0; j < m.row; j++) {
            hiddenBoard[i][j] = 0;
            gameBoard[i][j] = 63;
        }
    }
}

void set_mine(struct minefield m) {
    int x, y;

    for (int i = 0; i < m.frequency; i++) { //지뢰 랜덤 배치
        x = rand() % m.col;
        y = rand() % m.row;
        if (x == a && y == b) {
            i--;
            continue;
        }
        if (hiddenBoard[x][y] == 0) { //해당 칸에 지뢰가 없으면
            hiddenBoard[x][y] = MINE; //지뢰 배치
        } 
        else //이미 지뢰가 있으면
            i--; //한 번 다시 위치 선정
    }
}

//[i - 1][j - 1] [i][j - 1] [i + 1][j - 1]
//[i - 1][j]         [i][j]    [i + 1][j]
//[i - 1][j + 1] [i][j + 1] [i + 1][j + 1]
void check_mine(struct minefield m) {
    int mine_count = 0;
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            mine_count = 0;

            if (hiddenBoard[i][j] == MINE) continue;
            if (i != 0 && j != 0 && hiddenBoard[i - 1][j - 1] == MINE) mine_count++;
            if (j != 0 && hiddenBoard[i][j - 1] == MINE) mine_count++;
            if (i != m.col - 1 && j != 0 && hiddenBoard[i + 1][j - 1] == MINE) mine_count++;
            if (i != 0 && hiddenBoard[i - 1][j] == MINE) mine_count++;
            if (i != m.col - 1 && hiddenBoard[i + 1][j] == MINE) mine_count++;
            if (i != 0 && j != m.row - 1 && hiddenBoard[i - 1][j + 1] == MINE) mine_count++;
            if (j != m.row - 1 && hiddenBoard[i][j + 1] == MINE) mine_count++;
            if (i != m.col - 1 && j != m.row - 1 && hiddenBoard[i + 1][j + 1] == MINE) mine_count++;

            hiddenBoard[i][j] = mine_count;
        }
    }
}

void print_scr(struct minefield m) { //화면 표기
    clear_scr(0);
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            if (i == chosenXPos && j == chosenYPos) {
                printby_color(12, gameBoard[i][j]);
            }
            else {
                printf("%c  ", gameBoard[i][j]);
            }
        }
        printf("\n");
    }
}

void get_key_move(minefield m) {
    while (is_game) {
        char key;
        if (_kbhit) {
            key = _getch();
            if (key > 0) {
                switch (key) {
                case ENTER: 
                    open_tile();
                    break;
                case FLAG: 
                    raise_flag();
                    break;
                default:
                    break;
                }
            }
            else {
                key = _getch();
                switch (key) {
                case UP:
                    chosenXPos == 0 ? chosenXPos : chosenXPos--;
                    print_scr(MINEFIELD[diff]);
                    break;
                case DOWN:
                    chosenXPos == m.row ? chosenXPos : chosenXPos++;
                    print_scr(MINEFIELD[diff]);
                    break;
                case RIGHT:
                    chosenYPos == m.col ? chosenYPos : chosenYPos++;
                    print_scr(MINEFIELD[diff]);
                    break;
                case LEFT:
                    chosenYPos == 0 ? chosenXPos : chosenYPos--;
                    print_scr(MINEFIELD[diff]);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void raise_flag() {
    int tile = hiddenBoard[chosenXPos][chosenYPos];
    if (gameBoard != 70) {
        gameBoard[chosenXPos][chosenYPos] = FLAG;
    } else {
        gameBoard[chosenXPos][chosenYPos] = tile + 48;
    }
}

void open_tile() {
    int tile = hiddenBoard[chosenXPos][chosenYPos];
    if (tile < 0) {
        game_end(MINEFIELD[diff]);
    }
    else {
        gameBoard[chosenXPos][chosenYPos] = tile + 48;
        print_scr(MINEFIELD[diff]);
    }
}

void game_end(struct minefield m) {
    clear_scr(0);
    is_game = false;
    printf("지뢰를 밟았습니다..!\n정답을 공개합니다. 'X'가 지뢰의 위치입니다\n");
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            hiddenBoard[i][j] != MINE ? printf("%d  ", hiddenBoard[i][j]) : printby_color(12, 'X');
        }
        printf("\n");
    }
    printf("\n10초 후 메인 화면으로 돌아갑니다");

    clear_scr(10000);
}

void custom_field(struct minefield* m) {
    int x, y, frequency;
    printf("지뢰판의 X값을 입력하세요 ( 최대 24 칸 )\n");
    scanf("%d", &x);
    printf("지뢰판의 Y값을 입력하세요 ( 최대 20 칸 )\n");
    scanf("%d", &y);
    printf("지뢰의 개수를 입력하세요 ( 최대 %d개 )\n", x * y / 4);
    scanf("%d", &frequency);

    if (x > 24 || y > 20 || frequency > x * y / 4) {
        printf("입력값이 잘못되었습니다");
        clear_scr(1000);
        return 0;
    }

    m->col = x;
    m->row = y;
    m->frequency = frequency;

    printf("가로 %d칸\n세로 %d칸\n지뢰 %d칸", m->col, m->row, m->frequency);

    clear_scr(1000);
}

int change_game_option() {
    int a;
    printf("게임을 시작하려면 1을, 난이도를 바꾸려면 2를, 난이도 커스텀을 하려면 3을, 게임을 종료하려면 4를 입력해주세요\n");
    scanf("%d", &a);

    return a;
}

void set_diff(int* diff) {
    int temp;
    printf("난이도를 선택하세요\n쉬움: 1\n중간: 2\n어려움: 3\n커스텀: 4\n");
    scanf("%d", &temp);

    printf("현재 난이도: %d\n", temp);
    *diff = temp - 1;

    clear_scr(1000);
}

void printby_color(int color, char text) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    printf("%c  ", text);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void clear_scr(int time) {
    Sleep(time);
    system("cls");
}