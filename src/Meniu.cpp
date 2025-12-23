#include "Meniu.h"

void meniu::sterge() {
    for (auto* p : produse) delete p;
    produse.clear();
}

void meniu::copiaza(const std::vector<produs*>& altele) {
    for (const auto* p : altele) {
        produse.push_back(p->clone());
    }
}

meniu::~meniu() {
    sterge();
}

meniu::meniu(const meniu& other) {
    copiaza(other.produse);
}

meniu& meniu::operator=(const meniu& other) {
    if (this != &other) {
        sterge();
        copiaza(other.produse);
    }
    return *this;
}

void meniu::AdaugaProdus(produs* p) {
    produse.push_back(p);
}

const std::vector<produs*>& meniu::getProduse() const {
    return produse;
}

produs* meniu::CautaProdus(const std::string& nume) {
    for (auto* p : produse) {
        if (p->getNume() == nume) return p;
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, const meniu& m) {
    out << "\n====================== MENIU ======================\n";

    out << "\n--------------------- MANCARE ---------------------\n";
    bool gasitMancare = false;
    for (const auto* p : m.produse) {
        if (dynamic_cast<const mancare*>(p) != nullptr) {
            out << *p << "\n";
            gasitMancare = true;
        }
    }
    if (!gasitMancare) out << "   (Momentan nu servim mancare)\n";

    out << "\n--------------------- BAUTURI ---------------------\n";
    bool gasitBautura = false;
    for (const auto* p : m.produse) {
        if (dynamic_cast<const bautura*>(p) != nullptr) {
            out << *p << "\n";
            gasitBautura = true;
        }
    }
    if (!gasitBautura) out << "   (Momentan nu servim bauturi)\n";

    out << "\n--------------------- DESERT ----------------------\n";
    bool gasitDesert = false;
    for (const auto* p : m.produse) {
        if (dynamic_cast<const desert*>(p) != nullptr) {
            out << *p << "\n";
            gasitDesert = true;
        }
    }
    if (!gasitDesert) out << "   (Momentan nu servim desert)\n";

    out << "\n===================================================\n";
    return out;
}