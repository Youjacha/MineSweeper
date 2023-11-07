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

minefield MINEFIELD[4] = { //minefield�� �迭 
    {8, 8, 10}, {14, 14, 35}, {20, 20, 80}, {8, 8, 10} };

int diff = 1; //�⺻ ���̵�
bool is_game = false; 

int hiddenBoard[24][20]; 
char gameBoard[24][20]; 

int chosenXPos = 0;
int chosenYPos = 0; 

void set_mine(struct minefield m); //���� ��ġ
void check_mine(struct minefield m); //�ֺ� ���� ��ġ Ȯ��
void print_scr(struct minefield m); //ȭ�� ������Ʈ
void custom_field(struct minefield* m); //���̵� Ŀ����
int change_game_option(); //�ɼ� ����
void game_play(); //���� ����
void set_diff(int* diff); //���̵� ����
void get_key_move(minefield m); //Ű ������ ����
void raise_flag();
void open_tile();
void game_end(struct minefield m); //���� ��
void printby_color(int color, char text); //12 - ����, 15 - �Ͼ�
void clear_scr(int time); //ȭ�� �ʱ�ȭ 
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

    for (int i = 0; i < m.col; i++) { //���� �ʱ�ȭ
        for (int j = 0; j < m.row; j++) {
            hiddenBoard[i][j] = 0;
            gameBoard[i][j] = 63;
        }
    }
}

void set_mine(struct minefield m) {
    int x, y;

    for (int i = 0; i < m.frequency; i++) { //���� ���� ��ġ
        x = rand() % m.col;
        y = rand() % m.row;
        if (x == a && y == b) {
            i--;
            continue;
        }
        if (hiddenBoard[x][y] == 0) { //�ش� ĭ�� ���ڰ� ������
            hiddenBoard[x][y] = MINE; //���� ��ġ
        } 
        else //�̹� ���ڰ� ������
            i--; //�� �� �ٽ� ��ġ ����
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

void print_scr(struct minefield m) { //ȭ�� ǥ��
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
    printf("���ڸ� ��ҽ��ϴ�..!\n������ �����մϴ�. 'X'�� ������ ��ġ�Դϴ�\n");
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            hiddenBoard[i][j] != MINE ? printf("%d  ", hiddenBoard[i][j]) : printby_color(12, 'X');
        }
        printf("\n");
    }
    printf("\n10�� �� ���� ȭ������ ���ư��ϴ�");

    clear_scr(10000);
}

void custom_field(struct minefield* m) {
    int x, y, frequency;
    printf("�������� X���� �Է��ϼ��� ( �ִ� 24 ĭ )\n");
    scanf("%d", &x);
    printf("�������� Y���� �Է��ϼ��� ( �ִ� 20 ĭ )\n");
    scanf("%d", &y);
    printf("������ ������ �Է��ϼ��� ( �ִ� %d�� )\n", x * y / 4);
    scanf("%d", &frequency);

    if (x > 24 || y > 20 || frequency > x * y / 4) {
        printf("�Է°��� �߸��Ǿ����ϴ�");
        clear_scr(1000);
        return 0;
    }

    m->col = x;
    m->row = y;
    m->frequency = frequency;

    printf("���� %dĭ\n���� %dĭ\n���� %dĭ", m->col, m->row, m->frequency);

    clear_scr(1000);
}

int change_game_option() {
    int a;
    printf("������ �����Ϸ��� 1��, ���̵��� �ٲٷ��� 2��, ���̵� Ŀ������ �Ϸ��� 3��, ������ �����Ϸ��� 4�� �Է����ּ���\n");
    scanf("%d", &a);

    return a;
}

void set_diff(int* diff) {
    int temp;
    printf("���̵��� �����ϼ���\n����: 1\n�߰�: 2\n�����: 3\nĿ����: 4\n");
    scanf("%d", &temp);

    printf("���� ���̵�: %d\n", temp);
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