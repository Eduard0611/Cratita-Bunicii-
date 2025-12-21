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
    out << "=========== MENIU ===========\n";
    for (const auto* p : m.produse) {
        out << *p << "\n";
    }
    return out;
}