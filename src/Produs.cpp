#include "Produs.h"
#include <utility>


produs::produs(std::string nume, double pret) : nume(std::move(nume)), pret(pret) {}

produs::produs(const produs& other) : nume(other.nume), pret(other.pret), Ingrediente(other.Ingrediente) {}

produs& produs::operator=(const produs& other) {
    if (this != &other) {
        nume = other.nume;
        pret = other.pret;
        Ingrediente = other.Ingrediente;
    }
    return *this;
}

produs::~produs() {}

void produs::AdaugaIngredienteInProdus(const std::string& numeIng, double cantitate) {
    Ingrediente.push_back({numeIng, cantitate});
}

double produs::getPret() const {
    return pret;
}

const std::string& produs::getNume() const {
    return nume;
}

std::vector<IngNecesar>& produs::getIngrediente() {
    return Ingrediente;
}

std::ostream& operator<<(std::ostream& out, const produs& p) {
    out << p.nume << " | Pret: " << p.pret << " RON\nIngrediente:\n";
    for (const auto& ing : p.Ingrediente)
        out << "  - " << ing.nume << " : " << ing.cantitate << "\n";
    return out;
}