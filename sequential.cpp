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

//Ele tira algumas casas decimais
float Round(float var)
{
    float value = (int)(var * 1000 + .5);
    return (float)value / 1000;
}

//Ele consegue Gera uma equação de linha e devolve um array com 3 posições (deixei a formula e o método no readme)
deque<float> Get_Bisector(Point A, Point B)
{
    Point B0((A.x + B.x) / 2, (A.y + B.y) / 2);
    float a = B.x - A.x;
    float b = B.y - A.y;
    float c = -a * B0.x - b * B0.y;
    //O a,b,c representão as constantes da equação de linha do Bisetor
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

//Calula a distancia entre dois pontos
float Get_Distance(Point A, Point B)
{
    return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

//Ele faz o senguinte calculo, se o ponto c, estiver contido na linha ab, então a distancia de ac + bc é igual a ab
bool Is_in_line(Point a, Point b, Point c)
{   //A função round (que retira algumas casas decimais) para evitar que alguns problemas gerados pela raiz quadrada ou divisões ou arredondamentos automaticos não interfiram com o calculo
    if (round(Get_Distance(a, c) + Get_Distance(b, c)) == round(Get_Distance(a, b)))
    {

        return true;
    }
    return false;
}

//Ele obtem o angulo que Q1 e Q2 tem com P no centro
//Vou deixar a formula no Git
float Get_Angle(Point P, Point Q1, Point Q2)
{
    Point PQ1(Q1.x - P.x, Q1.y - P.y);
    Point PQ2(Q2.x - P.x, Q2.y - P.y);
    float Q1PQ2 = PQ1.x * PQ2.x + PQ1.y * PQ2.y;
    float distPQ1 = Get_Distance(P, Q1);
    float distPQ2 = Get_Distance(P, Q2);
    float angle = acos(Q1PQ2 / (distPQ1 * distPQ2));
    return (angle * 180 / M_PI);
}

//Esta função é feita para encontrar um ponto de intersecção entre a linha gerado no Bi setor e um dado segmento AB
//Vou deixar a formula no Git
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

    //Se z[2] for igual a zero, ele não intersecta nenhum ponto
    if (z[2] == 0)
        return Point(MAXIMO, 0);

    Point intersec(z[0] / z[2], z[1] / z[2]);

    //Verificar essas formulas pois ela serve para verificar se o ponto esta contido no x, porem já uma função fazendo isso
    if (intersec.x > max(A.x, B.x) and intersec.x < min(A.x, B.x))
        return Point(MAXIMO, 0);

    if (intersec.y > max(A.y, B.y) and intersec.y < min(A.y, B.y))
        return Point(MAXIMO, 0);

    return intersec;
}

//Esta função ordena os pontos em sentido anti horario
//Eu devia ter criado uma classe linha e ir modificando por ela
//Essa função é a mais ineficiente de todo código na minha opinião (Brum)
deque<Point> Sort_Points_Anti_Clockwise(deque<Point> list, Point p)
{
    deque<Point> oldlist = list;
    deque<Point> newlist;
    Point q(0, p.y);
    Point min(0, 100);
    bool points_left;

    //Neste loop ele fica basicamente selecionando o ponto mais a esquerda até o mais a direita
    do
    {
        deque<Point>::iterator i, j;
        i = oldlist.begin();
        j = oldlist.begin();
        points_left = false;
        float min_angle = 360;
        //Ele vai e testa cada ponto até achar o mais a esquerda
        for (Point b : oldlist)
        {   
            //Condição que verifica o menor angulo do set
            //q é um ponto fixo a esquerda
            //p é o ponto que a celula se orienta
            //b o ponto testado
            //A condição q.y >= b.y garante o sentido horario
            if (min_angle >= Get_Angle(p, q, b) and q.y >= b.y)
            {
                min_angle = Get_Angle(p, q, b);
                min = b;
                j = i;
                points_left = true;
            }
            i++;
        }
        //Esse condição apaga os pontos já percorridos na lista velha e add na lista organizada
        if (points_left)
        {
            newlist.push_front(min);
            oldlist.erase(j);
        };
        //O loop continua ate o points left der como falso ou acabarem -se os pontos na lista antiga
    } while (points_left != false and oldlist.size() != 0);

    q.x = BOXMAX;
    //joga Q para o outro lado e realiza o sort agora da direita para esquerda
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

//Função que executa aquele loop inteiro (achar o bistor, intersecção na bora) em um ponto q 
deque<Point> New_Cell(Cell oldcell, Point p, Point q)
{

    deque<Point> newlist;
    deque<Point> dellist;

    //Acha o Bi-setor (linha que se encontra os pontos p e q e segue perpendicular a eles)
    deque<float> Bi = Get_Bisector(p, q);
    Point lastt(0, 0);

    //Loop para verificar se há intersecção desta linha à alguma borda da célula (testa em todas as bordas)
    for (int i = 0; i < oldcell.listpoints.size(); i++)
    {
        int j = i + 1;
        if (j == oldcell.listpoints.size())
        {
            j = 0;
        }

        //Função para achar pontos de intersecção 
        Point t = intersec_Bi_Pnt(Bi, oldcell.listpoints[i], oldcell.listpoints[j]);
        
        //Função para verificar se o ponto achado esta dentro da celula ou nao
        //A função verfica se o ponto t esta entre uma linha de I e J
        if (!Is_in_line(oldcell.listpoints[i], oldcell.listpoints[j], t))
        {

        }
        else
        {
            //Add o ponto de intersecção
            newlist.push_front(t);
            
            //Loop para excluir pontos que estiverem fora do novo setor
            for (int k = 0; k < oldcell.listpoints.size(); k++)
            {
                Point t_extra = intersec_Bi_Pnt(Bi, oldcell.listpoints[k], p);
                
                if (Is_in_line(oldcell.listpoints[k], p, t_extra))
                {
                    dellist.push_front(oldcell.listpoints[k]);
                }
            }
        }
    }

    //Se não houver interseptação alguma é retornado a celula antiga
    if (newlist.size() == 0)
    {
        return oldcell.listpoints;
    }

    //Aqui à remoção dos pontos que ficaram de fora para a nova celula
    //Eu devia melhorar isso, não encontrei um pop que funciona-se direito aqui
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
    }
    return newlist;
}

//Rotina para criação de setores;
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
            {   //Primeiro se é executado a procura do bi-setor, da intersecção e da remoção dos pontos
                cell.listpoints = New_Cell(cell, p, q);
                //Aqui é feito a ordenação dos pontos da celula em sentido anti horario
                //Fica mais facil trabalhar com as celulas com os pontos desse jeito quando passados em loop.
                //Pois cada proximo ponto é a outra ponta da borda do ponto atual.
                cell.listpoints = Sort_Points_Anti_Clockwise(cell.listpoints, p);
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

    //Optei por dar direto os limites do plano
    Point A(0, 0);
    Point B(200, 0);
    Point C(200, 200);
    Point D(0, 200);

    //Inicia o array de pontos do set
    deque<Point> lpnts = Array_to_PointList(lpntsf, 14);

    deque<Point> lbox;
    lbox.push_back(A);
    lbox.push_back(B);
    lbox.push_back(C);
    lbox.push_back(D);
    Point bx(10, 10);
    Cell box(bx);
    box.listpoints = lbox;

    deque<Cell> cells = Voronoi(box, lpnts);


    cout << "\n\n###CELULAS###\n\n";

    int i = 0;
    for (Cell cell : cells)
    {
        cout << "Celula: "<< i <<"\n";
        for (Point point : cell.listpoints)
        {
            cout << ",[" << point.x << "," << point.y << "] ";
        }
        cout << "\n";
        i++;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "\n\nTempo levado: " << duration.count() << " segundos" << std::endl;

    return 10;
}