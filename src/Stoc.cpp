#include "stoc.h"
#include <fstream>  // Necesar pentru scrierea in fisiere (ofstream)
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
                std::cout << "Nu exista suficient/i/e " << nume << " in stoc!\n";
                return false;
            } else {
                ing.scadeCantitate(cantitate);
                return true;
            }
        }
    }
    std::cout << "Ingredientul " << nume << " nu exista in stoc!\n";
    return false;
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