#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <deque>
#include <chrono>
#include <C:\Users\Jao Brum\Documents\PROJETO ALEXANDRO\programacao-paralela-e-distribuida\points_gen.h>

#define MAXIMO (3.40283 * pow(10, 38))
#define BOXMAX 200

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
  void Roundxy()
  {
    x = roundf(x);
    y = roundf(y);
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

// Ele tira algumas casas decimais
float Round(float var)
{
  float value = (int)(var * 1000 + .5);
  return (float)value / 1000;
}

// Ele consegue Gera uma equação de linha e devolve um array com 3 posições (deixei a formula e o método no readme)
deque<float> Get_Bisector(Point A, Point B)
{
  Point B0((A.x + B.x) / 2, (A.y + B.y) / 2);
  float a = B.x - A.x;
  float b = B.y - A.y;
  float c = -a * B0.x - b * B0.y;
  // O a,b,c representão as constantes da equação de linha do Bisetor
  //  isso é igual a xa+yb+c=0
  deque<float> abc;
  abc.push_back(a);
  abc.push_back(b);
  abc.push_back(c);
  return abc;
}

deque<Point> Array_to_PointList(deque<float> arr)
{
  deque<Point> points;
  for (int i = 0; i < arr.size(); i = i + 2)
  {
    Point alfa(arr[i], arr[i + 1]);
    points.push_back(alfa);
  }
  return points;
}

// Calula a distancia entre dois pontos
float Get_Distance(Point A, Point B)
{
  return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

// Ele faz o senguinte calculo, se o ponto c, estiver contido na linha ab, então a distancia de ac + bc é igual a ab
bool Is_in_line(Point a, Point b, Point c)
{ // A função round (que retira algumas casas decimais) para evitar que alguns problemas gerados pela raiz quadrada ou divisões ou arredondamentos automaticos não interfiram com o calculo
  bool is_in_line = round(Get_Distance(a, c) + Get_Distance(b, c)) == round(Get_Distance(a, b));
  return is_in_line;
}

// Ele obtem o angulo que Q1 e Q2 tem com P no centro
// Vou deixar a formula no Git
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

// Esta função é feita para encontrar um ponto de intersecção entre a linha gerado no Bi setor e um dado segmento AB
// Vou deixar a formula no Git
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

  // Se z[2] for igual a zero, ele não intersecta nenhum ponto
  if (z[2] == 0)
    return Point(MAXIMO, 0);

  Point intersec(z[0] / z[2], z[1] / z[2]);

  // Verificar essas formulas pois ela serve para verificar se o ponto esta contido no x, porem já uma função fazendo isso
  if (intersec.x > max(A.x, B.x) and intersec.x < min(A.x, B.x))
    return Point(MAXIMO, 0);

  if (intersec.y > max(A.y, B.y) and intersec.y < min(A.y, B.y))
    return Point(MAXIMO, 0);

  return intersec;
}

deque<Point> Sort_Points_Anti_Clockwise(deque<Point> list, Point p, float boxmax)
{
  deque<Point> oldlist = list;
  deque<Point> newlist_under_y;
  deque<Point> newlist_above_y;
  deque<Point> newlist_a;
  deque<Point> newlist_b;
  Point q(0, p.y);

  for (Point b : oldlist)
  {
    if (b.y <= q.y)
    {
      newlist_under_y.push_front(b);
    }
    else
    {
      newlist_above_y.push_front(b);
    }
  }

  for (Point b : newlist_under_y)
  {
    deque<Point>::iterator index = newlist_a.begin();
    bool pos_found = false;
    for (Point c : newlist_a)
    {
      if (Get_Angle(p, q, b) < Get_Angle(p, q, c) and !pos_found)
      {
        pos_found = true;
      }
      if (!pos_found)
      {
        index++;
      }
    }
    if (pos_found)
    {
      newlist_a.insert(index, 1, b);
    }
    else
    {
      newlist_a.push_back(b);
    }
  }

  q.x = boxmax; ///////////////////////////////////////////////

  for (Point b : newlist_above_y)
  {
    deque<Point>::iterator index = newlist_b.begin();
    bool pos_found = false;
    for (Point c : newlist_b)
    {
      if (Get_Angle(p, q, b) < Get_Angle(p, q, c) and !pos_found)
      {
        pos_found = true;
      }
      if (!pos_found)
      {
        index++;
      }
    }
    if (pos_found)
    {
      newlist_b.insert(index, 1, b);
    }
    else
    {
      newlist_b.push_back(b);
    }
  }

  deque<Point> newlist;

  for (Point p : newlist_a)
  {
    newlist.push_back(p);
  }

  for (Point p : newlist_b)
  {
    newlist.push_back(p);
  }

  return newlist;
}

// Função que executa aquele loop inteiro (achar o bistor, intersecção na bora) em um ponto q
deque<Point> New_Cell(Cell oldcell, Point p, Point q)
{
  deque<Point> newlist;
  deque<Point> dellist;
  // Acha o Bi-setor (linha que se encontra os pontos p e q e segue perpendicular a eles)
  deque<float> Bi = Get_Bisector(p, q);
  Point lastt(0, 0);

// Loop para verificar se há intersecção desta linha à alguma borda da célula (testa em todas as bordas)
#pragma omp parallel for
  for (int i = 0; i < oldcell.listpoints.size(); i++)
  {
    int j = i + 1;
    if (j == oldcell.listpoints.size())
    {
      j = 0;
    }

    // Função para achar pontos de intersecção
    Point t = intersec_Bi_Pnt(Bi, oldcell.listpoints[i], oldcell.listpoints[j]);

    // Função para verificar se o ponto achado esta dentro da celula ou nao
    if (Is_in_line(oldcell.listpoints[i], oldcell.listpoints[j], t))
    {
// Add o ponto de intersecção
#pragma omp critical
      {
        // Seção crítica porque é alterada a lista, o que impacta em todo o contexto.
        newlist.push_front(t);
      }

      // Loop para excluir pontos que estiverem fora do novo setor
      for (int k = 0; k < oldcell.listpoints.size(); k++)
      {
        Point t_extra = intersec_Bi_Pnt(Bi, oldcell.listpoints[k], p);

        if (Is_in_line(oldcell.listpoints[k], p, t_extra))
        {
#pragma omp critical
          // Seção crítica porque é alterada a lista, o que impacta em todo o contexto.
          {
            dellist.push_front(oldcell.listpoints[k]);
          }
        }
      }
    }
  }

  // Se não houver interseptação alguma é retornado a celula antiga
  if (newlist.size() == 0)
  {
    return oldcell.listpoints;
  }

// Aqui à remoção dos pontos que ficaram de fora para a nova celula
// Eu devia melhorar isso, não encontrei um pop que funciona-se direito aqui
#pragma omp parallel for
  for (int a = 0; a < oldcell.listpoints.size(); a++)
  {
    Point point_a = oldcell.listpoints[a];
    bool is_on = true;
    for (int b = 0; b < dellist.size(); b++)
    {
      if (point_a.x == dellist[b].x && point_a.y == dellist[b].y)
      {
        is_on = false;
        break;
      }
    }
    if (is_on)
    {
#pragma omp critical
        // Seção crítica porque é alterada a lista, o que impacta em todo o contexto.
      {
        newlist.push_front(point_a);
      }
    }
  }
  return newlist;
}

// Rotina para criação de setores;
deque<Cell> Voronoi(Cell box, deque<Point> listpoints, float boxmax)
{
  Cell cell = box;
  deque<Cell> cells;
  for (Point p : listpoints)
  {
    cell.p = p;
    for (Point q : listpoints)
    {
      if (p.x != q.x or p.y != q.y)
      {
        // Primeiro se é executado a procura do bi-setor, da intersecção e da remoção dos pontos
        cell.listpoints = New_Cell(cell, p, q);
        // Aqui é feito a ordenação dos pontos da celula em sentido anti horario
        // Fica mais facil trabalhar com as celulas com os pontos desse jeito quando passados em loop.
        // Pois cada proximo ponto é a outra ponta da borda do ponto atual.
        cell.listpoints = Sort_Points_Anti_Clockwise(cell.listpoints, p, boxmax);
      }
    }
    cells.push_back(cell);
    cell = box;
  }
  return cells;
}

int main(int argc, char *argv[])
{

  float boxsize = strtol(argv[1], NULL, 10);
  float gridsize = strtol(argv[0], NULL, 10);

  boxsize = strtol(argv[1], NULL, 10);
  gridsize = strtoll(argv[2], NULL, 10);

  printf("BS %f,GS %f", boxsize, gridsize);
  deque<float> lpntsf = pointgen(boxsize, gridsize);
  deque<Point> lpnts = Array_to_PointList(lpntsf);

  // Optei por dar direto os limites do plano
  Point A(0, 0);
  Point B(boxsize, 0);
  Point C(boxsize, boxsize);
  Point D(0, boxsize);

  // Inicia o array de pontos do set

  deque<Point> lbox;
  lbox.push_back(A);
  lbox.push_back(B);
  lbox.push_back(C);
  lbox.push_back(D);
  Point bx(boxsize / 2, boxsize / 2);
  Cell box(bx);
  box.listpoints = lbox;

  auto start = std::chrono::high_resolution_clock::now();

  deque<Cell> cells = Voronoi(box, lpnts, boxsize);

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> duration = end - start;

  std::cout << "\n\nTempo levado: " << duration.count() << " segundos" << std::endl;

  int i = 0;

  cout << "\n\n###PONTOS###\n\n";
  for (Point point : lpnts)
  {
    i++;
    cout << "[" << point.x << "," << point.y << "]";
    if (i < lpnts.size())
    {
      cout << ",";
    }
  }

  cout << "\n\n###CELULAS###\n\n";

  i = 0;
  int j = 0;
  for (Cell cell : cells)
  {
    j++;
    i = 0;
    cout << "[";
    for (Point point : cell.listpoints)
    {
      i++;
      cout << "[" << point.x << "," << point.y << "]";
      if (i < cell.listpoints.size())
      {
        cout << ",";
      }
    }
    cout << "]";
    if (j < cells.size())
    {
      cout << ",";
    }
  }

  return 0;
}