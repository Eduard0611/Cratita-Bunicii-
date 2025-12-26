#include "Angajat.h"
#include <utility>

angajat::angajat(std::string nume, double salariu)
    : nume(std::move(nume)), salariu(salariu) {}

const std::string& angajat::getNume() const {
    return nume;
}

double angajat::getSalariu() const {
    return salariu;
}

void angajat::afisare(std::ostream& out) const {
    out << nume << " ocupa functia de " << getFunctie()
        << " si a fost platit cu " << salariu << " RON pentru aceasta zi.";
}

std::ostream& operator<<(std::ostream& out, const angajat& i) {
    i.afisare(out);
    return out << std::endl;
}

bucatar::bucatar(std::string nume, double salariu) : angajat(std::move(nume), salariu) {}

std::string bucatar::getFunctie() const {
    return "Bucatar";
}

ospatar::ospatar(std::string nume, double salariu) : angajat(std::move(nume), salariu) {}

std::string ospatar::getFunctie() const {
    return "Ospatar";
}