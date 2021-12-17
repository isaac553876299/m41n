#include <iostream>

void Swapr(int r[6][9], int A, int a, int B, int b, int C, int c, int D, int d)
{
    int aux = r[A][a];
    r[A][a] = r[B][b];
    r[B][b] = r[C][c];
    r[C][c] = r[D][d];
    r[D][d] = aux;
}

int main()
{
    int reset = 1, r[6][9], m[6][16] =
    {
        1,0,1,2,2,0,1,2,3,0,1,2,4,0,1,2,
        5,0,1,2,2,6,3,0,0,8,7,6,4,2,5,8,
        5,2,5,8,3,6,3,0,0,2,5,8,1,2,5,8,
        5,8,7,6,4,6,3,0,0,0,1,2,2,2,5,8,
        5,6,3,0,1,6,3,0,0,6,3,0,3,2,5,8,
        3,8,7,6,2,8,7,6,1,8,7,6,4,8,7,6
    };
    while (1)
    {
        system("cls");
        for (int i = 0, s = 1; i < 54; i++)
        {
            if (reset == 1) r[i / 9][i % 9] = i / 9;
            printf("%d", r[i / 9][i % 9]);
            if (r[i / 9][i % 9] != i / 9) s = 0;
            if (i % 9 == 8) printf("\n");
            if (i == 53) printf(s == 1 ? "[solved!]\n" : "[scrambled]\n");
        }
        int f = getchar() - '0';
        if (f >= 0 && f <= 5)
        {
            Swapr(r, f, 0, f, 6, f, 8, f, 2);
            Swapr(r, f, 1, f, 3, f, 7, f, 5);
            for (int i = 1; i < 4; i++) Swapr(r, m[f][0], m[f][i], m[f][4], m[f][4 + i], m[f][8], m[f][8 + i], m[f][12], m[f][12 + i]);
        }
        reset = (f == 6 ? 1 : 0);
    }
    return 0;
}
