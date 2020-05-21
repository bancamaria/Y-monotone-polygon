#include <iostream>
#include <fstream>
#include <cmath>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace std;

struct punct {
    int x, y;
};

bool isLess(punct V[], int i, int j) {
    /// pentru cazurile in care avem muchii orizontale
    return (V[i].y < V[j].y || (V[i].y == V[j].y && i < j));
}

bool isYMonotone(punct V[], int n) {
    int minim = 0;

    for(int i = 0; i < n; i++)
        /// if(V[i].y < V[(i+1)%n].y && V[i].y < V[(i-1)%n].y)
        /// in cazul primului varf vecinul din stanga va fi utimul varf, respectiv vecinul din dreapta al ultimului varf va fi primul varf
        if(isLess(V, i, (i + 1) % n) && isLess(V, i, (i - 1) % n))
            minim++;
    /// daca un singur varf al poligonului are coordonata y mai mica decat a vecinilor sai, acesta este y-monoton
    if(minim == 1)
        return 1;
    return 0; /// altfel, nu este
}

void printChains(punct V[], int n) {
    int idxMin = 0, idxMax = 0, minY = V[0].y, maxY = V[0].y, i;

    for(i = 1; i < n; i++) {
        if(V[i].y < minY) {
            idxMin = i;
            minY = V[i].y; }
        else if(V[i].y > maxY) {
            idxMax = i;
            maxY = V[i].y; } }
    cout << "index max: " << idxMax << "  maxY = " << maxY << endl;
    cout << "index min: " << idxMin << "  minY = " << minY << endl;
    int dif = abs(idxMax - idxMin);
    cout << "Lantul stang: ";
    for(i = idxMax; i <= idxMax + dif + 2; i++)
        cout << "(" << V[i % n].x << ", " << V[i % n].y << ") -> ";
    cout << endl << "Lantul drept: ";
    for(i = idxMax; i >= idxMax - dif; i--)
        cout << "(" << V[i % n].x << ", " << V[i % n].y << ") -> ";
    cout << endl;
}

void drawPolygon(punct V[], int n, int isPolygon) {
    int C[2 * n];
    punct cord[n];
    int fullheight = getmaxheight(), fullwidth = getmaxwidth();
    initwindow(fullwidth, fullheight, "Windows BGI", 0, 0, false);
    //std::cout << getmaxheight() << " " << getmaxwidth() << std::endl;
    int length = 8 * fullheight / 10;
    //int width = 6*fullwidth/10;
    setviewport((fullwidth - length) / 2, fullheight / 10, (fullwidth - length) / 2 + length, length + fullheight / 10, 1);
    //setfillstyle(1, 5);
    //floodfill(100, 100, 15);

    int idxMin, idxMax, minY = INT_MAX, maxY = INT_MIN, minX = INT_MAX, maxX = INT_MIN, i;
    for(i = 0; i < n; i++) {
        if(V[i].y < minY) {
            idxMin = i;
            minY = V[i].y;
            }
            else if(V[i].y > maxY) {
                idxMax = i;
                maxY = V[i].y;
                }
            if(V[i].x < minX)
                minX = V[i].x;
                else if(V[i].x > maxX)
                    maxX = V[i].x;
        }

    int scala = max( maxY - minY + 2, maxX - minX + 2);
    int unit = length / scala;

    int centreX = (minX - 1) * scala;
    int centreY = (minY - 1) * scala;
    /// calculam coordonatele varfurilor polinomului in sistemul ferestrei grafice
    for(i = 0; i < n; i++) {
        cord[i].x = (V[i].x - minX + 1) * unit;
        cord[i].y = (V[i].y - minY + 1) * unit;
        C[2 * i] = (V[i].x - minX + 1) * unit;
        C[2 * i + 1] = (V[i].y - minY + 1) * unit; }
    C[2 * i] = C[0];
    C[2 * i + 1] = C[1];

    setcolor(9);
    drawpoly(n + 1, C);

    /// Se asteapta oapasarea unei taste pentru a continua algoritmul
    getch();
    int dif = abs(idxMax - idxMin);

    if(isPolygon) {
        setcolor(4);
        int j = 0;
        for(i = idxMax; i < idxMax + dif + 2; i++) {
            line(cord[i % n].x, cord[i % n].y, cord[(i + 1) % n].x, cord[(i + 1) % n].y); }
        setcolor(6);
        for(i = idxMax; i > idxMax - dif; i--)
            line(cord[i % n].x, cord[i % n].y, cord[(i - 1) % n].x, cord[(i - 1) % n].y);
    }

    getch();
    closegraph();
}

int main()
{
    ifstream f("puncte.txt");
    int n, i;
    f >> n;
    punct V[n];
    for (i = 0; i < n; i++) {
        f >> V[i].x >> V[i].y;
        cout << V[i].x << " " << V[i].y << "\n"; }
    cout << endl;
    printChains(V, n);
    int result = isYMonotone(V, n);
    if(result)
        cout << "Poligonul este y-monoton" << endl;
    else
        cout << "Poligonul nu este y-monoton" << endl;

    drawPolygon(V, n, result);

    return 0;

}
