#ifndef UTILS_H
#define UTILS_H
#include "dcel.hh"
#include "bits/stdc++.h"
using namespace std;

void enumerate_face(const Face &face);

double angle(const Point &a, const Point &b, const Point &c);
deque<Vertex *> get_notches(deque<Vertex *> polygon);
deque<Vertex*> get_LPVS(deque<Vertex*> &notches , deque<Vertex*> &L , deque<Vertex*> &P);
array<Point, 2> get_rectangle(deque<Vertex *> &L);
bool inside_rectangle(array<Point, 2> &rectangle, const Point &point);
array<double, 3> get_line(const Point &a, const Point &b);
bool same_side_semiplane(array<double, 3> &coef, const Point &a,
                         const Point &b);
// void decompose(const DCEL &dcel);
Vertex *next_vertex(Vertex *v);
bool check_notch(Vertex *a, Vertex *b, Vertex *c, Vertex *start,
                 Vertex *second);
void add_edge(Vertex *v1, Vertex *vr);
void update_face(Edge *e , Face *f);
#endif // UTILS_H
