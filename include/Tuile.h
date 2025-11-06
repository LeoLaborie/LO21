#ifndef TUILE_H
#define TUILE_H

#include <vector>
#include "Hexagone.h"

class Tuile {
private:
    std::vector<Hexagone*> hex;   // hex[0] = ancre
    int orientation = 0;          // 0..5

public:
    Tuile() = default;
    Tuile(Hexagone* h1, Hexagone* h2, Hexagone* h3);
    Tuile(Hexagone* h1, Hexagone* h2, Hexagone* h3, Hexagone* h4);
    void creerTuile(Hexagone* h1, Hexagone* h2, Hexagone* h3);

    const std::vector<Hexagone*>& getHexagones() const { return hex; }
    static inline const int (&offsetsTable())[6][2][2] {
        static const int T[6][2][2] = {
            { {-1,  1}, { 0,  1} }, // 0  (orientation de base)
            { {-1,  0}, {-1,  1} }, // 1  +60°
            { { 0, -1}, {-1,  0} }, // 2  +120°
            { { 1, -1}, { 0, -1} }, // 3  +180°
            { { 1,  0}, { 1, -1} }, // 4  +240°
            { { 0,  1}, { 1,  0} }  // 5  +300°
        };
        return T;
    }
    void rotation() { orientation = (orientation + 1) % 6; }
    void getOffsets(int& dq1, int& dr1, int& dq2, int& dr2) const {
        const auto& T = offsetsTable();
        dq1 = T[orientation][0][0]; dr1 = T[orientation][0][1];
        dq2 = T[orientation][1][0]; dr2 = T[orientation][1][1];
    }

    void afficher() const {
        for (const auto& h : hex) {
            h->afficher();
        }
    }
};

#endif
