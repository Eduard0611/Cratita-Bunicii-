#include "Ingredient.h"
#include <utility>

ingredient::ingredient(std::string nume, double cantitate, double pretAchizitie)
    : nume(std::move(nume)), cantitate(cantitate), pretAchizitie(pretAchizitie) {}

double ingredient::getPretAchizitie() const {
    return pretAchizitie;
}

const std::string& ingredient::getNume() const {
    return nume;
}

double ingredient::getCantitate() const {
    return cantitate;
}

void ingredient::scadeCantitate(double x) {
    cantitate -= x;
}

std::ostream& operator<<(std::ostream& out, const ingredient& i) {
    out << i.nume << " : " << i.cantitate << " : " << "Pretul per unitate " << i.pretAchizitie;
    return out;
}