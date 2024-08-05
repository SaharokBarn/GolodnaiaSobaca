#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <cstdlib>
using namespace std;

struct Tpoint
{
    int x, y;
};

const int MapSX = 32;
const int MapSY = 32;

char Map[MapSX][MapSY];

const char SimbolVoid = '.';
const char SimbolPlayer = 2;
const char SimbolBarier = '#';
const char SimbolApple = 23;
void INIT()
{
    for(int i = 0; i< MapSX; i++)
    {
        for(int j = 0; j< MapSY; j++)
        {
            Map[i][j] = SimbolVoid;
        }
    }
}
int score = -1;
void PRINT()
{
    for(int i = 0; i< MapSX; i++)
    {
        for(int j = 0; j< MapSY; j++)
        {
            cout << Map[i][j];
        }
        cout << endl;
    }
    cout << "score: " << score;
}

void CreateApple()
{
    score++;
    Map[rand() % MapSX][rand() % MapSY] = SimbolApple;
}

Tpoint Pos = {6,6};
Tpoint OPos = {6,6};


int main()
{
    srand(static_cast<unsigned int>(time(0)));
    INIT();
    CreateApple();

    Map[Pos.x][Pos.y] = SimbolPlayer;


    Map[2][6] = SimbolBarier;

    PRINT();

    //system("cls");
    while(true)
    {


    if(GetKeyState('W') < 0)
    {
        if(Pos.x != 0){
                if(Map[Pos.x - 1][Pos.y] != SimbolBarier){
                        if(Map[Pos.x - 1][Pos.y] == SimbolApple){CreateApple();}
                        Map[Pos.x][Pos.y] = SimbolVoid;
        Pos.x--;
       Map[Pos.x][Pos.y] = SimbolPlayer;
        system("cls");
        PRINT();
            }
        }
    }



    if(GetKeyState('S') < 0)
    {
        if(Pos.x != MapSX - 1){
                if(Map[Pos.x + 1][Pos.y] != SimbolBarier){
                        if(Map[Pos.x + 1][Pos.y] == SimbolApple){CreateApple();}
                        Map[Pos.x][Pos.y] = SimbolVoid;
        Pos.x++;
       Map[Pos.x][Pos.y] = SimbolPlayer;
        system("cls");
        PRINT();
            }
        }
    }
        if(GetKeyState('A') < 0)
    {
        if(Pos.x != MapSX - 1){
                if(Map[Pos.x][Pos.y - 1] != SimbolBarier){
                        if(Map[Pos.x ][Pos.y - 1] == SimbolApple){CreateApple();}
                        Map[Pos.x][Pos.y] = SimbolVoid;
        Pos.y--;
       Map[Pos.x][Pos.y] = SimbolPlayer;
        system("cls");
        PRINT();
            }
        }
    }
        if(GetKeyState('D') < 0)
    {
        if(Pos.x != MapSX - 1){
                if(Map[Pos.x][Pos.y + 1] != SimbolBarier){
                        if(Map[Pos.x ][Pos.y + 1] == SimbolApple){CreateApple();}
                        Map[Pos.x][Pos.y] = SimbolVoid;
        Pos.y++;
       Map[Pos.x][Pos.y] = SimbolPlayer;
        system("cls");
        PRINT();
            }
        }
    }

Sleep (50);
    }
    return 0;
}


