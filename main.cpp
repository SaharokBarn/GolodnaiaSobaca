#include <iostream>
#include <windows.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

const int SCREEN_W = 120;
const int SCREEN_H = 30;
const int MAP_W = 32;
const int MAP_H = 32;

char map_data[MAP_H][MAP_W];
float player_x = 3.5f;
float player_y = 3.5f;
float player_angle = 0.0f;
int score = 0;
int apples_eaten = 0;
float player_speed = 0.5f;

char screen[SCREEN_H][SCREEN_W];
int color[SCREEN_H][SCREEN_W];

void init_map() {
    memset(map_data, '.', sizeof(map_data));

    for(int i = 0; i < MAP_W; i++) {
        map_data[0][i] = '#';
        map_data[MAP_H-1][i] = '#';
    }
    for(int i = 0; i < MAP_H; i++) {
        map_data[i][0] = '#';
        map_data[i][MAP_W-1] = '#';
    }

    for(int i = 3; i < 20; i++) map_data[5][i] = '#';
    for(int i = 8; i < 25; i++) map_data[12][i] = '#';
    for(int i = 3; i < 15; i++) map_data[18][i] = '#';
    for(int i = 5; i < MAP_H-3; i++) map_data[i][10] = '#';
    for(int i = 3; i < MAP_H-5; i++) map_data[i][20] = '#';

    map_data[5][5] = '.';
    map_data[5][15] = '.';
    map_data[12][15] = '.';
    map_data[18][8] = '.';

    for(int i = 0; i < 15; i++) {
        int x = 2 + rand() % (MAP_W-4);
        int y = 2 + rand() % (MAP_H-4);
        if(map_data[y][x] == '.') map_data[y][x] = 'A';
    }
}

void clear_screen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);
}

void render() {
    memset(screen, ' ', sizeof(screen));
    memset(color, 0, sizeof(color));

    const float FOV = 60.0f * 3.14159f / 180.0f;
    const float DEPTH = 30.0f;

    for(int col = 0; col < SCREEN_W; col++) {
        float angle = player_angle - FOV/2.0f + (float)col / SCREEN_W * FOV;

        float dx = cos(angle);
        float dy = sin(angle);

        float dist = 0.0f;
        char hit_char = '.';
        int hit_color = 7;

        for(float step = 0.0f; step < DEPTH; step += 0.25f) {
            float x = player_x + dx * step;
            float y = player_y + dy * step;

            int ix = (int)x;
            int iy = (int)y;

            if(ix < 0 || ix >= MAP_W || iy < 0 || iy >= MAP_H) {
                dist = step;
                hit_char = '#';
                hit_color = 8;
                break;
            }

            if(map_data[iy][ix] != '.') {
                dist = step;
                hit_char = map_data[iy][ix];
                if(hit_char == 'A') hit_color = 12;
                else hit_color = 8;
                break;
            }
        }

        if(dist < 0.1f) dist = DEPTH;

        int wall_height = (int)(SCREEN_H / (dist + 0.5f) * 8.0f);

        int start_row = (SCREEN_H - wall_height) / 2;
        int end_row = start_row + wall_height;

        if(start_row < 0) start_row = 0;
        if(end_row > SCREEN_H) end_row = SCREEN_H;

        char wall_char = ' ';
        int wall_color = 7;

        if(hit_char == 'A') {
            wall_color = 12;
            if(dist < 3.0f) wall_char = '#';
            else if(dist < 8.0f) wall_char = '=';
            else if(dist < 15.0f) wall_char = '-';
            else wall_char = '.';
        }
        else if(hit_char == '#') {
            if(dist < 5.0f) {
                wall_color = 10;
                wall_char = '#';
            }
            else if(dist < 10.0f) {
                wall_color = 2;
                wall_char = '=';
            }
            else if(dist < 15.0f) {
                wall_color = 8;
                wall_char = '-';
            }
            else {
                wall_color = 8;
                wall_char = '.';
            }
        }

        for(int row = start_row; row < end_row; row++) {
            screen[row][col] = wall_char;
            color[row][col] = wall_color;
        }

        for(int row = 0; row < start_row; row++) {
            screen[row][col] = ' ';
            color[row][col] = 0;
        }
        for(int row = end_row; row < SCREEN_H; row++) {
            if(row < SCREEN_H - 2) {
                screen[row][col] = ' ';
                color[row][col] = 1;
            }
        }
    }
}

void draw() {
    clear_screen();

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i = 0; i < SCREEN_H; i++) {
        for(int j = 0; j < SCREEN_W; j++) {
            SetConsoleTextAttribute(hOut, color[i][j]);
            cout << screen[i][j];
        }
        cout << "\n";
    }

    SetConsoleTextAttribute(hOut, 7);
    cout << "\n=== HUNGRY DOG 3D ===\n";
    cout << "Score: " << score << " | Apples: " << apples_eaten << "\n";
    cout << "Pos: (" << (int)player_x << "," << (int)player_y << ") Angle: " << (int)(player_angle * 180 / 3.14159) << "d\n";
    cout << "WASD - Move | Q/E - Rotate | Z/X - Speed | ESC - Exit\n";
}

void input() {
    float move_x = 0, move_y = 0;
    float rot_speed = 0.05f;

    if(GetKeyState('W') < 0) {
        move_x = cos(player_angle) * player_speed;
        move_y = sin(player_angle) * player_speed;
    }
    if(GetKeyState('S') < 0) {
        move_x = -cos(player_angle) * player_speed;
        move_y = -sin(player_angle) * player_speed;
    }
    if(GetKeyState('A') < 0) {
        move_x = cos(player_angle - 1.57f) * player_speed;
        move_y = sin(player_angle - 1.57f) * player_speed;
    }
    if(GetKeyState('D') < 0) {
        move_x = cos(player_angle + 1.57f) * player_speed;
        move_y = sin(player_angle + 1.57f) * player_speed;
    }

    if(GetKeyState('Z') < 0 && player_speed > 0.05f) player_speed -= 0.005f;
    if(GetKeyState('X') < 0 && player_speed < 0.5f) player_speed += 0.005f;

    if(GetKeyState('Q') < 0) player_angle -= rot_speed;
    if(GetKeyState('E') < 0) player_angle += rot_speed;

    float new_x = player_x + move_x;
    float new_y = player_y + move_y;

    int ix = (int)new_x;
    int iy = (int)new_y;

    if(ix >= 1 && ix < MAP_W-1 && iy >= 1 && iy < MAP_H-1) {
        if(map_data[iy][ix] == '.' || map_data[iy][ix] == 'A') {
            player_x = new_x;
            player_y = new_y;

            if(map_data[iy][ix] == 'A') {
                score += 10;
                apples_eaten++;
                map_data[iy][ix] = '.';

                int x = 2 + rand() % (MAP_W-4);
                int y = 2 + rand() % (MAP_H-4);
                if(map_data[y][x] == '.') map_data[y][x] = 'A';
            }
        }
    }
}

int main() {
    srand((unsigned int)time(0));

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    init_map();

    while(true) {
        if(GetKeyState(VK_ESCAPE) < 0) break;

        input();
        render();
        draw();

        Sleep(16);
    }

    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    return 0;
}
