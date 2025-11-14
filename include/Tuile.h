#ifndef TUILE_H
#define TUILE_H

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
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
    //constructeurs et destructeurs
    Tuile() = default;
    ~Tuile()=default;
    Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3);
    Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3, Hexagone* hex4);

    //getters
    const std::vector<Hexagone*>& getHexagones() const { return hex; }
    const std::vector<OffsetQR>&  getOffsets()   const { return offsets; }

    //manipulations de la tuile
    void pivoterTuile() {
        for (auto& o : offsets) rotation60deg(o);
    }

    void pivoterTuile(int k) {
        k %= 6;
        for (int i = 0; i < k; ++i)
            for (auto& o : offsets) rotation60deg(o);
    }

    //surchage opérateur
    friend std::ostream& operator<<(std::ostream& os, const Tuile& t) {
        if (t.hex.empty()) return os << "(tuile vide)\n";
        std::map<int, std::vector<size_t>> lignes;
        for (size_t i = 0; i < t.offsets.size(); ++i)
            lignes[t.offsets[i].r].push_back(i);
        for (auto& [r, id] : lignes) {
            std::sort(id.begin(), id.end(),[&](size_t a, size_t b){ return t.offsets[a].q < t.offsets[b].q; });
            for (size_t k = 0; k < id.size(); ++k) {
                t.hex[id[k]]->afficher(true);
                if (k + 1 < id.size()) os << ' ';
            }
            os << '\n';
        }
        return os;
    }
};

#endif
