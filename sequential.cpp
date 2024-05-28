#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <deque>
#include <chrono>

float MAXIMO = 3.40283 * pow(10, 38);
float BOXMAX = 200;

using namespace std;

class Point
{
public:
    float x;
    float y;
    Point(float i, float j)
    {
        x = i;
        y = j;
    }
};

class Cell
{
public:
    Point p;
    std::deque<Point> listpoints;
    Cell(Point q) : p(q.x, q.y){};
    // void Set_List(deque<Point>listpoints);
};

float Round(float var)
{
    float value = (int)(var * 1000 + .5);
    return (float)value / 1000;
}

deque<float> Get_Bisector(Point A, Point B)
{
    Point B0((A.x + B.x) / 2, (A.y + B.y) / 2);
    float a = B.x - A.x;
    float b = B.y - A.y;
    float c = -a * B0.x - b * B0.y;
    // isso é igual a xa+yb+c=0
    deque<float> abc;
    abc.push_back(a);
    abc.push_back(b);
    abc.push_back(c);
    return abc;
}

deque<Point> Array_to_PointList(float *arr, int size)
{
    deque<Point> points;
    for (int i = 0; i < size - 1; i = i + 2)
    {
        Point alfa(arr[i], arr[i + 1]);
        points.push_back(alfa);
    }
    return points;
}

float Get_Distance(Point A, Point B)
{
    return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

bool Is_in_line(Point a, Point b, Point c)
{
    // cleprintf("DISTNORM %f ... %f\n\n\n",Get_Distance(c,b),Get_Distance(a,b));
    printf("DISTROUND %f ... %f\n\n\n", round(Get_Distance(a, c) + Get_Distance(b, c)), round(Get_Distance(a, b)));
    if (round(Get_Distance(a, c) + Get_Distance(b, c)) == round(Get_Distance(a, b)))
    {

        return true;
    }
    return false;
}

float Get_Angle(Point P, Point Q1, Point Q2)
{
    // P vai estar no centro
    Point PQ1(Q1.x - P.x, Q1.y - P.y);
    Point PQ2(Q2.x - P.x, Q2.y - P.y);
    float Q1PQ2 = PQ1.x * PQ2.x + PQ1.y * PQ2.y;
    float distPQ1 = Get_Distance(P, Q1);
    float distPQ2 = Get_Distance(P, Q2);
    float angle = acos(Q1PQ2 / (distPQ1 * distPQ2));
    return (angle * 180 / M_PI);
}

Point intersec_Bi_Pnt(deque<float> Bi, Point A, Point B)
{
    deque<float> v;
    v.push_back(A.y - B.y);
    v.push_back(B.x - A.x);
    v.push_back(A.x * B.y - A.y * B.x);

    deque<float> z;
    z.push_back(Bi[1] * v[2] - Bi[2] * v[1]);
    z.push_back(Bi[2] * v[0] - Bi[0] * v[2]);
    z.push_back(Bi[0] * v[1] - Bi[1] * v[0]);

    if (z[2] == 0)
        return Point(MAXIMO, 0);

    Point intersec(z[0] / z[2], z[1] / z[2]);

    if (intersec.x > max(A.x, B.x) and intersec.x < min(A.x, B.x))
        return Point(MAXIMO, 0);

    if (intersec.y > max(A.y, B.y) and intersec.y < min(A.y, B.y))
        return Point(MAXIMO, 0);

    return intersec;
}

deque<Point> Sort_Points_Clockwise(deque<Point> list, Point p)
{
    deque<Point> oldlist = list;
    deque<Point> newlist;
    Point q(0, p.y);
    Point min(0, 100);
    bool points_left;

    do
    {
        deque<Point>::iterator i, j;
        i = oldlist.begin();
        j = oldlist.begin();
        points_left = false;
        float min_angle = 360;
        for (Point b : oldlist)
        {
            if (min_angle >= Get_Angle(p, q, b) and q.y >= b.y)
            {
                min_angle = Get_Angle(p, q, b);
                min = b;
                j = i;
                points_left = true;
            }
            i++;
        }
        if (points_left)
        {
            newlist.push_front(min);
            oldlist.erase(j);
        };
    } while (points_left != false and oldlist.size() != 0);

    q.x = BOXMAX;

    do
    {
        deque<Point>::iterator i, j;
        i = oldlist.begin();
        j = oldlist.begin();
        points_left = false;
        float min_angle = 360;
        for (Point b : oldlist)
        {
            if (min_angle >= Get_Angle(p, q, b) and q.y <= b.y)
            {
                min_angle = Get_Angle(p, q, b);
                min = b;
                j = i;
                points_left = true;
            }
            i++;
        }
        if (points_left)
        {
            newlist.push_front(min);
            oldlist.erase(j);
        }
    } while (points_left != false and oldlist.size() != 0);
    return newlist;
}

deque<Point> New_Cell(Cell oldcell, Point p, Point q)
{

    deque<Point> newlist;
    deque<Point> dellist;

    deque<float> Bi = Get_Bisector(p, q);
    Point lastt(0, 0);

    printf("##########################\n\n\n");

    printf("PONTO Q %f...%f\n\n\n", q.x, q.y);

    for (int i = 0; i < oldcell.listpoints.size(); i++)
    {
        int j = i + 1;
        if (j == oldcell.listpoints.size())
        {
            j = 0;
        }

        Point t = intersec_Bi_Pnt(Bi, oldcell.listpoints[i], oldcell.listpoints[j]);
        printf("Parede percorrida : %f...%f a %f...%f\n\n\n", oldcell.listpoints[i].x, oldcell.listpoints[i].y, oldcell.listpoints[j].x, oldcell.listpoints[j].y);
        if (!Is_in_line(oldcell.listpoints[i], oldcell.listpoints[j], t))
        {
            // printf("FORA : %f...%f\n\n\n",t.x,t.y);
            // printf("Parede nao atingida : %f...%f a %f...%f\n\n\n",oldcell.listpoints[i].x,oldcell.listpoints[i].y,oldcell.listpoints[j].x,oldcell.listpoints[j].y);
        }
        else
        {
            newlist.push_front(t);
            printf("Intersec : %f...%f\n\n\n", t.x, t.y);
            // printf("Parede atingida : %f...%f a %f...%f\n\n\n",oldcell.listpoints[i].x,oldcell.listpoints[i].y,oldcell.listpoints[j].x,oldcell.listpoints[j].y);
            for (int k = 0; k < oldcell.listpoints.size(); k++)
            {
                Point t_extra = intersec_Bi_Pnt(Bi, oldcell.listpoints[k], p);
                printf("Linha percorrida : %f...%f a %f...%f\n\n\n", oldcell.listpoints[k].x, oldcell.listpoints[k].y, p.x, p.y);
                // printf("Distancias : %f...%f\n\n\n",Get_Distance(t_extra,oldcell.listpoints[k])+Get_Distance(t_extra,p),Get_Distance(p,oldcell.listpoints[k]));
                if (Is_in_line(oldcell.listpoints[k], p, t_extra))
                {
                    dellist.push_front(oldcell.listpoints[k]);
                    printf("excluido : %f...%f\n\n\n", oldcell.listpoints[k].x, oldcell.listpoints[k].y);
                }
            }
        }
    }

    if (newlist.size() == 0)
    {
        return oldcell.listpoints;
    }
    for (Point a : oldcell.listpoints)
    {
        bool is_on = true;
        for (Point b : dellist)
        {
            if (a.x == b.x and a.y == b.y)
            {
                is_on = false;
            }
        }
        if (is_on)
        {
            newlist.push_front(a);
        }
        else
        {
            printf("DELETANDO : %f...%f\n\n\n", a.x, a.y);
        }
    }
    return newlist;
}

deque<Cell> Voronoi(Cell box, deque<Point> listpoints)
{

    Cell cell = box;
    deque<Cell> cells;
    int i = 0;
    int j = 0;
    for (Point p : listpoints)
    {
        cell.p = p;
        for (Point q : listpoints)
        {
            if (p.x != q.x or p.y != q.y)
            {
                cell.listpoints = New_Cell(cell, p, q);
                cell.listpoints = Sort_Points_Clockwise(cell.listpoints, p);
            }
            j++;
        }
        i++;
        cells.push_back(cell);
        cell = box;
    }
    return cells;
}

int main()
{
    float lpntsf[14] = {50, 100, 106, 49, 66, 175, 137, 197, 195, 147, 178, 73, 123, 123};

    auto start = std::chrono::high_resolution_clock::now();

    Point A(0, 0);
    Point B(200, 0);
    Point C(200, 200);
    Point D(0, 200);

    deque<Point> lpnts = Array_to_PointList(lpntsf, 14);

    deque<Point> lbox;
    lbox.push_back(A);
    lbox.push_back(B);
    lbox.push_back(C);
    lbox.push_back(D);
    Point bx(10, 10);
    Cell box(bx);
    box.listpoints = lbox;

    printf("%d\n\n", sizeof(lpnts));
    printf("%d\n\n", sizeof(lpntsf));

    deque<Cell> cells = Voronoi(box, lpnts);

    // printf("%f",Round(11.142303));

    cout << "\n\n###CELULAS###\n\n";

    int i = 0;
    for (Cell cell : cells)
    {
        cout << "[";
        for (Point point : cell.listpoints)
        {
            cout << "[" << point.x << "," << point.y << "],";
        }
        cout << "],";
        i++;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 10;
}