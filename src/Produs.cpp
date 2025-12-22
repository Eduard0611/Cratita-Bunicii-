#include "Produs.h"
#include <utility>

produs::produs(std::string nume, double pret) : nume(std::move(nume)), pret(pret) {}

produs::~produs() = default;

void produs::afisare(std::ostream& out) const {
    out << "Produs: " << nume << " | Pret Base: " << pret << " RON";
}

double produs::getPretVanzare() const {
    return pret;
}

void produs::AdaugaIngredienteInProdus(const std::string& numeIng, double cantitate) {
    Ingrediente.push_back({numeIng, cantitate});
}
const std::string& produs::getNume() const { return nume; }
std::vector<IngNecesar>& produs::getIngrediente() { return Ingrediente; }

std::ostream& operator<<(std::ostream& out, const produs& p) {
    p.afisare(out);
    if (!p.Ingrediente.empty()) {
        out << "\n   Ingrediente:";
        for (const auto& ing : p.Ingrediente)
            out << " " << ing.nume << "(" << ing.cantitate << ")";
    }
    return out;
}

mancare::mancare(std::string nume, double pret, bool dePost)
    : produs(std::move(nume), pret), dePost(dePost) {}

produs* mancare::clone() const { return new mancare(*this); }

void mancare::afisare(std::ostream& out) const {
    produs::afisare(out);
    out << " [Tip: Mancare" << (dePost ? " de Post" : "") << "]";
}

bautura::bautura(std::string nume, double pret, double volumAlcool)
    : produs(std::move(nume), pret), volumAlcool(volumAlcool) {}

produs* bautura::clone() const { return new bautura(*this); }

void bautura::afisare(std::ostream& out) const {
    produs::afisare(out);
    out << " [Tip: Bautura " << getVolumAlcool() << "% alc.]";
}

double bautura::getPretVanzare() const {
    if (volumAlcool > 0) return pret * 1.20;
    return pret;
}

double bautura::getVolumAlcool() const { return volumAlcool; }

desert::desert(std::string nume, double pret, bool contineZahar)
    : produs(std::move(nume), pret), contineZahar(contineZahar) {}

produs* desert::clone() const { return new desert(*this); }

void desert::afisare(std::ostream& out) const {
    produs::afisare(out);
    out << " [Tip: Desert" << (contineZahar ? " cu Zahar" : " fara Zahar") << "]";
}