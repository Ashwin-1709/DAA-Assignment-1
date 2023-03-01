#include "utils.hh"
#include <algorithm>
#include <bits/stdc++.h>

// The angle swept by a counterclockwise rotation from bc to ba
double angle(const Point &a, const Point &b, const Point &c) {
    const auto mag_ba =
        std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    const auto mag_bc =
        std::sqrt((c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y));
    const auto dot = (a.x - b.x) * (c.x - b.x) + (a.y - b.y) * (c.y - b.y);
    const auto cos_theta = std::clamp(dot / (mag_ba * mag_bc), -1.0, 1.0);
    const auto cross = (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x);
    if (cross == 0) {
        if (cos_theta > 0) {
            return 0.0;
        } else {
            return 180.0;
        }
    } else if (cross > 0) {
        return std::acos(cos_theta) * 180 / M_PI;
    } else {
        return 360.0 - (std::acos(cos_theta) * 180 / M_PI);
    }
}

void Enumerate_Polygons(std::set<Face*> Polygons) {
    usize cnt = 1;
    for(auto &f : Polygons) {
        std::cout << "Face " << cnt++ << ":\n";
        Edge *now = f->edge;
        do {
            std::cout << "(" << now->origin->point.x << "," << now->origin->point.y << ") ";
            now = now->next;
        } while(now != f->edge);
        std::cout << '\n';
    }
}

std::deque<Vertex *> get_notches(const std::deque<Vertex *> &polygon) {
    std::deque<Vertex *> notches;
    const usize n = polygon.size();
    if (n < 3)
        return notches;
    for (usize i = 0; i < n; i++) {
        Vertex *base = polygon[i];
        Vertex *left = polygon[(i - 1 + n) % n];
        Vertex *right = polygon[(i + 1) % n];
        if (angle(left->point, base->point, right->point) > 180)
            notches.push_back(base);
    }
    return notches;
}

// rectangle with minimum area that encloses vertices of list L
std::array<Point, 2> get_rectangle(const std::deque<Vertex *> &L) {
    Point minimum, maximum;
    minimum.x = 1e9, minimum.y = 1e9;
    maximum.x = -1e9, maximum.y = -1e9;
    for (auto &v : L) {
        minimum.x = std::min(minimum.x, v->point.x);
        minimum.y = std::min(minimum.y, v->point.y);
        maximum.x = std::max(maximum.x, v->point.x);
        maximum.y = std::max(maximum.y, v->point.y);
    }
    return {minimum, maximum};
}

bool inside_rectangle(const std::array<Point, 2> &rectangle,
                      const Point &point) {
    if (point.x > rectangle[0].x and point.x < rectangle[1].x and
        point.y > rectangle[0].y and point.y < rectangle[1].y)
        return true;
    return false;
}

std::array<double, 3> get_line(const Point &a, const Point &b) {
    // x1x + y1y + c = 0
    std::array<double, 3> coefficients;
    coefficients[0] = b.y - a.y;
    coefficients[1] = a.x - b.x;
    coefficients[2] = -(coefficients[0] * a.x + coefficients[1] * a.y);
    return coefficients;
}

bool same_side_semiplane(const std::array<double, 3> &coef, const Point &a,
                         const Point &b) {
    double L1 = coef[0] * a.x + coef[1] * a.y + coef[2];
    double L2 = coef[0] * b.x + coef[1] * b.y + coef[2];
    if ((L1 * L2) > 0)
        return true;
    return false;
}

Vertex *next_vertex(const Vertex *vertex) {
    return vertex->incident_edge->next->origin;
}

bool check_notch(const Vertex *a, const Vertex *b, const Vertex *c,
                 const Vertex *start, const Vertex *second) {
    double angle_b = angle(a->point, b->point, c->point);
    double angle_c = angle(b->point, c->point, start->point);
    double angle_start = angle(c->point, start->point, second->point);
    return angle_b <= 180 and angle_c <= 180 and angle_start <= 180;
}

void update_face(Edge *edge, Face *face) {
    edge->left_face = face;
    Edge *next = edge->next;
    while (next != edge) {
        next->left_face = face;
        next = next->next;
    }
}

std::deque<Vertex *> get_LPVS(std::deque<Vertex *> &notches,
                              std::deque<Vertex *> &L,
                              std::deque<Vertex *> &P) {
    std::deque<Vertex *> LPVS;
    for (auto notch : notches) {
        bool in_P = false, in_L = false;
        for (auto cur : L)
            in_L |= (cur == notch);
        for (auto cur : P)
            in_P |= (cur == notch);
        if (in_P and !in_L)
            LPVS.push_back(notch);
    }
    return LPVS;
}

// void enumerate_face(const Face &face) {
//     Edge *start = face.edge;
//     auto cur = start->next;
//     std::vector<Vertex *> vertices;
//     vertices.push_back(start->origin);
//     while (cur != start) {
//         vertices.push_back(cur->origin);
//         cur = cur->next;
//     }
//     for (auto &u : vertices) {
//         std::cout << u->point.x << " " << u->point.y << "\n";
//     }
// }
