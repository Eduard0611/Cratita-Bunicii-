#include "Meniu.h"
#include <vector>
#include <string>

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

void swap(meniu& first, meniu& second) noexcept {
    using std::swap;
    swap(first.produse, second.produse);
}

meniu& meniu::operator=(meniu other) {
    swap(*this, other);
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

    std::vector<std::string> categorii = {"CIORBE", "FELURI PRINCIPALE", "BAUTURI", "DESERT"};

    for (const auto& cat : categorii) {
        out << "\n--------------------- " << cat << " ---------------------\n";
        bool gasit = false;
        for (const auto* p : m.produse) {
            if (p->getCategorie() == cat) {
                out << *p << "\n";
                gasit = true;
            }
        }
        if (!gasit) {
            out << "   (Momentan nu servim produse din categoria " << cat << ")\n";
        }
    }

    out << "\n===================================================\n";
    return out;
}