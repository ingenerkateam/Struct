#include "TXLib.h"

struct sharVraga
{
    int x;
    int y;
    int vx;
    int vy;
    COLORREF color;
};

void drawHelp();
void drowchar (int x, int y);
void drawhero (int x, int y, COLORREF color);
void dvizhenieVnutriKorobki ();
void acceleration (sharVraga *shar);
void ikran (int x, int y, int* vx, int* vy);
void stolknovenie (int *schet, int heroX, int heroY, int x, int y, COLORREF color);

int main()
{
   // HDC  background_FromTXLibHelp = txLoadImage ("Resources\\Images\\i.bmp");
    txCreateWindow (500, 596);
    dvizhenieVnutriKorobki();

    return 0;
}

void drawhero (int x, int y, COLORREF color)
{
    txSetFillColor (color);
    txCircle (x, y, 50);
}

void drowchar (int x, int y, COLORREF color)
{
    txSetFillColor (color);
    txCircle (x     ,y     , 50);
}

//ускорение
void acceleration (sharVraga *shar)
{
    shar->x = shar->x + shar->vx;
    shar->y = shar->y + shar->vy;
    ikran    (shar->x, shar->y,
              &shar->vx,  &shar->vy);
    drowchar (shar->x, shar->y, shar->color);
}

//Проверяем, что находимся в пределах экрана
void ikran (int x, int y, int* vx, int* vy)
{
    if ( x > 500) {
        *vx = - *vx;
    }
    if ( y > 596) {
        *vy = - *vy;
    }
    if ( x < 0) {
        *vx = - *vx;
    }
    if ( y < 0) {
        *vy = - *vy;
    }
}
void stolknovenie (int* schet, int heroX, int heroY, int x, int y, COLORREF color)
{
    if (abs(heroX - x) +abs(heroY - y) < 100) {
        drawhero (heroX, heroY, color);
        txSleep(30);
        *schet = *schet + 1;
    } else {
        //drawhero (heroX, heroY, RGB(126, 126, 126));
    }
}

void dvizhenieVnutriKorobki ()
{
    int pauseLength = 50;
    //                      x   y   vx vy   color
    sharVraga firstShar = {200, 50, 3, 8, TX_WHITE};
    sharVraga secondShar ={100, 50, 8, 5, TX_YELLOW};
    sharVraga thirdShar = {150, 50, -8, 6, TX_BLUE};

    int count_collisions1 = 0;
    int count_collisions2 = 0;
    int count_collisions3 = 0;

    int oldheroX = 300;
    int oldheroY = 100;
    int heroX = 300;
    int heroY = 100;
    HDC  fon = txLoadImage ("fon.bmp");

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        //txSetFillColor(TX_BLACK);
        //txClear();
        txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), fon, 0, 0);

        oldheroX = heroX;
        oldheroY = heroY;


        heroX = txMouseX();
        heroY = txMouseY();

        COLORREF color = txGetPixel(heroX, heroY);

        if (color == RGB(0, 0, 255))
        {
            txMessageBox ("Дурак что ли?", "Будем продолжать?", MB_YESNO);

            heroX = oldheroX;
            heroY = oldheroY;
        }
        if (color == TX_WHITE)
        {
            txMessageBox ("Будем продолжать?", "Будем продолжать?", MB_YESNO);
        }

        acceleration (&firstShar);
        acceleration (&secondShar);
        acceleration (&thirdShar);

        stolknovenie (&count_collisions1,  heroX,  heroY,  firstShar.x,   firstShar.y,  TX_RED);
        stolknovenie (&count_collisions2,  heroX,  heroY,  secondShar.x,  secondShar.y, TX_RED);
        stolknovenie (&count_collisions3,  heroX,  heroY,  thirdShar.x,  thirdShar.y, TX_RED);

        drawhero (heroX, heroY, RGB(126, 126, 126));

        if (GetAsyncKeyState('P')) {
            txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), fon, 0, 0);
            drawHelp();
            txSleep (50);

            while (!GetAsyncKeyState('P')) {
                txSleep (10);
            }
            txSleep (50);
        }

        txSleep (pauseLength);
    }

    printf ("count_collisions1=%d\n", count_collisions1);
    printf ("count_collisions2=%d\n", count_collisions2);
    printf ("count_collisions3=%d\n", count_collisions3);
    printf ("kol-vo_ochkov=%d\n", count_collisions1 * 3 - count_collisions2 * 2 + count_collisions3 * 5);

    txDeleteDC (fon);
}


void drawHelp()
{
    txSetColor(TX_YELLOW);
    txSelectFont("Arial", 36, 0);

    char help[4][100];
    sprintf (help[0], "Не сталкивайся с шарами!!!");
    txTextOut (100, 100, help[0]);
    sprintf (help[1], "Двигайся по стрелочкам!!!");
    txTextOut (100, 130, help[1]);
    sprintf (help[2], "Иначе я найду тебя!!!");
    txTextOut (100, 160, help[2]);
    sprintf (help[3], "И всё, игре конец!!!");
    txTextOut (100, 190, help[3]);
}
