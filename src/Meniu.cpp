#include "Meniu.h"
#include <iostream>

void meniu::AdaugaProduse(const produs& p) {
    produse.push_back(p);
}

std::vector<produs>& meniu::getProduse() {
    return produse;
}

produs* meniu::CautaProdus(const std::string& nume) {
    for (auto& p : produse) {
        if (p.getNume() == nume) {
            return &p;
        }
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, const meniu& m) {
    out << "===========MENIU===========" << std::endl;
    for (const auto& p : m.produse)
        out << p << "\n"; // Se bazează pe operatorul << din clasa produs
    return out;
}