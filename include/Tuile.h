#ifndef TUILE_H
#define TUILE_H

#include <vector>
#include <iostream>
#include "Hexagone.h"

struct OffsetQR {
    int q;
    int r;
};

inline void rotation60deg(OffsetQR& o) {
    int q = o.q, r = o.r;
    o.q = -r;
    o.r = q + r;
}

class Tuile {
private:
    std::vector<Hexagone*> hex;
    std::vector<OffsetQR> offsets;

public:
    Tuile() = default;
    Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3);
    Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3, Hexagone* hex4);
    void creerTuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3);

    const std::vector<Hexagone*>& getHexagones() const { return hex; }
    const std::vector<OffsetQR>&  getOffsets()   const { return offsets; }

    void pivoterTuile() {
        for (auto& o : offsets) rotation60deg(o);
    }

    void pivoterTuile(int k) {
        k %= 6;
        for (int i = 0; i < k; ++i)
            for (auto& o : offsets) rotation60deg(o);
    }

    void afficher(bool court = false) const {
        if (court) {
            for (const auto* h : hex) { h->afficher(true); std::cout << ' '; }
            std::cout << "\n";
        } else {
            for (const auto* h : hex) h->afficher();
        }
    }
};

#endif
