#include "Stoc.h"
#include "Exceptii.h"
#include <fstream>
#include <iostream>

void stoc::AdaugaIngredienteInStoc(const ingredient& ingredient) {
    Stocul_Restaurantului.push_back(ingredient);
}

std::vector<ingredient>& stoc::getStoc() {
    return Stocul_Restaurantului;
}

bool stoc::Consuma(const std::string& nume, double cantitate) {
    for (auto& ing : Stocul_Restaurantului) {
        if (ing.getNume() == nume) {
            if (ing.getCantitate() < cantitate) {
                throw EroareStocInsuficient(nume);
            } else {
                ing.scadeCantitate(cantitate);
                return true;
            }
        }
    }
    throw EroareStocInsuficient(nume + " (Ingredient inexistent in inventar)");
}

void stoc::DegradeazaStocPerisabil(double procent) {
    int contor = 0;
    for(auto& ing : Stocul_Restaurantului) {
        std::string nume = ing.getNume();
        if(nume.find("Carne") != std::string::npos ||
           nume.find("Oua") != std::string::npos ||
           nume.find("Lapte") != std::string::npos ||
           nume.find("Branza") != std::string::npos ||
           nume.find("Unt") != std::string::npos) {

            double cantitatePierduta = ing.getCantitate() * procent;
            ing.scadeCantitate(cantitatePierduta);
            contor++;
        }
    }
    std::cout << "[SISTEM] " << contor << " tipuri de ingrediente perisabile au scazut cu " << (procent*100) << "%.\n";
}

void stoc::SalveazaStoc(const std::string& numeFisier) const {
    std::ofstream fout(numeFisier);
    if (!fout.is_open()) {
        std::cout << "Eroare la salvarea stocului!\n";
        return;
    }
    for (const auto& ing : Stocul_Restaurantului)
        fout << ing.getNume() << " " << ing.getCantitate() << "\n";
    
    fout.close();
    std::cout << "Stocul actualizat a fost salvat in fisierul " << numeFisier << "\n";
}

std::ostream& operator<<(std::ostream& out, const stoc& s) {
    out << "+++++++STOCUL RESTAURANTULUI+++++++\n";
    for (const auto& ing : s.Stocul_Restaurantului)
        out << ing << "\n";
    return out;
}