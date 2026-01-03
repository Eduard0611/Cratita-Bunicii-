#include "Angajat.h"
#include <utility>

angajat::angajat(std::string nume, std::string functie, double salariu)
    : nume(std::move(nume)), functie(std::move(functie)), salariu(salariu) {}

const std::string& angajat::getNume() const {
    return nume;
}

double angajat::getSalariu() const {
    return salariu;
}

std::string angajat::getFunctie() const {
    return functie;
}

angajat* angajat::clone() const {
    return new angajat(*this);
}

void angajat::afisare(std::ostream& out) const {
    out << nume << " ocupa functia de " << functie
        << " si a fost platit cu " << salariu << " RON pentru aceasta zi.";
}

std::ostream& operator<<(std::ostream& out, const angajat& i) {
    i.afisare(out);
    return out << std::endl;
}