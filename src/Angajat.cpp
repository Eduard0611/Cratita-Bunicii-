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

std::ostream& operator<<(std::ostream& out, const angajat& i) {
    out << i.nume << " ocupa functia de " << i.functie 
        << " si a fost platit cu " << i.salariu << " RON pentru aceasta zi." << std::endl;
    return out;
}