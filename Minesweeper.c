#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

// 초급 => 8 / 8, 중급 => 18 * 14, 고급 => 24 * 20
// 초급 => 10개 / 중급 => 40개 / 고급 => 99개

typedef struct minefield {
    int col;
    int row;
    int frequency;
} minefield;

struct minefield MINEFIELD[4] = {
    {8, 8, 10}, {18, 14, 40}, {24, 20, 99}, {8, 8, 10} };

int diff = 1;
bool is_game = false;

int b_arr[24][20];
char f_arr[24][20];

int set_mine(struct minefield m);
int check_mine(struct minefield m);
int print_scr(struct minefield m);
int custom_field(struct minefield* m);
int game_option();
int game_play();
int set_diff(int* diff);
int tile_open();
int game_end(struct minefield m);
void clear_scr(int time);

int main() {
    srand((unsigned)time(NULL));
    int game;
    while (1) {
        game = game_option();
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

int set_mine(struct minefield m) {
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            b_arr[i][j] = 0;
            f_arr[i][j] = "?";
        }
    }

    int x, y;

    for (int i = 0; i < m.frequency; i++) {
        x = rand() % m.col;
        y = rand() % m.row;

        if (b_arr[x][y] == 0) {
            b_arr[x][y] = -1;
        }
        else
            i--;
    }

    return 0;
}

// [i - 1][j - 1], [i - 1][j], [i - 1][j + 1], [i][ j - 1], [i][j + 1], [i +1][j - 1], [i + 1][j], [i + 1][j + 1]

int check_mine(struct minefield m) {
    int mine_num = 0;
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            mine_num = 0;

            if (b_arr[i][j] == -1) continue;

            if ((i - 1 > 0) && (j - 1 > 0) && b_arr[i - 1][j - 1] == -1)
                mine_num++;
            if ((i - 1 > 0) && b_arr[i - 1][j] == -1)
                mine_num++;
            if ((i - 1 > 0) && (j + 1 < m.row) && b_arr[i - 1][j - 1] == -1)
                mine_num++;
            if ((j - 1 > 0) && b_arr[i][j - 1] == -1)
                mine_num++;
            if ((j + 1 > m.row) && b_arr[i][j + 1] == -1)
                mine_num++;
            if ((i + 1 < m.col) && b_arr[i + 1][j - 1] == -1)
                mine_num++;
            if ((i + 1 < m.col) && b_arr[i + 1][j] == -1)
                mine_num++;
            if ((i + 1 < m.col) && (j + 1 < m.row) && b_arr[i + 1][j + 1] == -1) {
                mine_num++;
            }
            b_arr[i][j] = mine_num;
        }
    }

    return 0;
}

int print_scr(struct minefield m) {
    clear_scr(0);
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            printf("%c  ", f_arr[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int custom_field(struct minefield* m) {
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
    return 0;
}

int game_option() {
    int a;
    printf("게임을 시작하려면 1을, 난이도를 바꾸려면 2를, 난이도 커스텀을 하려면 3을, 게임을 종료하려면 4를 입력해주세요\n");
    scanf("%d", &a);

    return a;
}

int game_play() {
    is_game = true;

    set_mine(MINEFIELD[diff]);
    check_mine(MINEFIELD[diff]);
    print_scr(MINEFIELD[diff]);

    while (is_game) {
        tile_open();
    }
}

int tile_open() {
    int a, b;
    printf("열 자리의 좌표값을 입력하세요\n");
    scanf("%d %d", &a, &b);
    int c = b_arr[a - 1][b - 1];
    if (c < 0) {
        game_end(MINEFIELD[diff]);
        return -1;
    } else {
        f_arr[a - 1][b - 1] = c + 48;
        print_scr(MINEFIELD[diff]);
        return 1;
    }
}

int game_end(struct minefield m) {
    clear_scr(0);
    is_game = false;
    printf("지뢰를 밟았습니다..!\n정답을 공개합니다. -1이 지뢰의 위치입니다\n");
    for (int i = 0; i < m.col; i++) {
        for (int j = 0; j < m.row; j++) {
            printf("%d ", b_arr[i][j]);
        }
        printf("\n");
    }
    printf("\n10초 후 메인 화면으로 돌아갑니다");

    clear_scr(10000);
    return 0;
}

int set_diff(int* diff) {
    int temp;
    printf("난이도를 선택하세요\n쉬움: 1\n중간: 2\n어려움: 3\n커스텀: 4\n");
    scanf("%d", &temp);

    printf("현재 난이도: %d\n", temp);
    *diff = temp - 1;
    
    clear_scr(1000);

    return 0;
}

void clear_scr(int time) {
    Sleep(time);
    system("cls");
}