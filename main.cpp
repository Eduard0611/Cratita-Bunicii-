#include <iostream>
#include <ctime>
#include "Restaurant.h"
#include "Meniu.h"
#include "Stoc.h"
#include "FileManager.h"
#include "Exceptii.h"
#include "Template_uri.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    DepozitSigur<int, 5> seifCoduri;
    seifCoduri.adauga(101);
    seifCoduri.adauga(202);

    DepozitSigur<std::string, 2> furnizoriCritici;
    furnizoriCritici.adauga("Furnizor Carne");

    try {
        stoc Stoc;
        meniu Meniu;
        restaurant Restaurant("Cratita Bunicii");

        FileManager::getInstanta()->IncarcaDate(Stoc, Meniu, Restaurant);

        if (esteMaiMare<double>(2500.0, 1000.0)) {
            std::cout << "[SISTEM] Analiza financiara: Buget peste pragul critic.\n";
        }

        if (esteMaiMare<int>(seifCoduri.getNrElemente(), 1)) {
            std::cout << "[SISTEM] Securitate: Multiple chei de acces detectate.\n";
        }

        bool jocActiv = true;
        double profitTotal = 1500.0;
        int ziuaCurenta = 1;

        std::cout << "\nBun venit! Apasati ENTER pentru a incepe simularea...";
        std::cin.get();

        while (jocActiv) {
            Restaurant.ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta, jocActiv);
            if (!jocActiv) break;
            if (profitTotal < 0) {
                std::cout << "\n!!! FALIMENT !!!\n";
                jocActiv = false;
            } else {
                ziuaCurenta++;
            }
        }

    } catch (const std::exception& e) {
        std::cout << "\n[FATAL ERROR]: " << e.what() << "\n";
        return 1;
    }

    return 0;
}