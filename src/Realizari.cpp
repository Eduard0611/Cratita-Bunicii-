#include "Realizari.h"
#include <iostream>

ManagerRealizari::ManagerRealizari() : realizariDeblocateTotal(0) {
    listaRealizari.emplace_back("Inceput Promitator", "Strange primii tai 2000 RON.",
        [](double bani, int, int, int, int) { return bani >= 2000; });

    listaRealizari.emplace_back("Magnat Local", "Strange o avere de 5000 RON.",
        [](double bani, int, int, int, int) { return bani >= 5000; });

    listaRealizari.emplace_back("Supravietuitor", "Rezista timp de 5 zile fara sa dai faliment.",
        [](double, int zile, int, int, int) { return zile >= 5; });

    listaRealizari.emplace_back("Veteran", "Rezista timp de 10 zile in industrie.",
        [](double, int zile, int, int, int) { return zile >= 10; });

    listaRealizari.emplace_back("Echipa Completa", "Angajeaza cel putin 3 bucatari.",
        [](double, int, int bucatari, int, int) { return bucatari >= 3; });

    listaRealizari.emplace_back("Atmosfera de Vis", "Imbunatateste decorul la nivelul 3.",
        [](double, int, int, int decor, int) { return decor >= 3; });

    listaRealizari.emplace_back("Restaurant de Lux", "Obtine un rating perfect de 5 stele.",
        [](double, int, int, int, int stele) { return stele >= 5; });

    listaRealizari.emplace_back("La Limita", "Termina o zi cu mai putin de 100 RON in cont.",
        [](double bani, int, int, int, int) { return bani < 100 && bani > 0; });
}

void ManagerRealizari::VerificaProgres(double bani, int zile, int bucatari, int decor, int stele) {
    bool cevaNou = false;

    for (auto& r : listaRealizari) {
        if (!r.deblocata && r.conditie(bani, zile, bucatari, decor, stele)) {
            r.deblocata = true;
            cevaNou = true;
        }
    }

    if (cevaNou) {
        std::cout << "\n****************************************\n";
        std::cout << "* REALIZARE NOUA DEBLOCATA!            *\n";
        std::cout << "****************************************\n";
        realizariDeblocateTotal = 0;
        for(const auto& r : listaRealizari) {
            if(r.deblocata) {
                std::cout << " -> [X] " << r.titlu << ": " << r.descriere << "\n";
                realizariDeblocateTotal++;
            }
        }
        std::cout << "\n";
    }
}

void ManagerRealizari::AfiseazaStatus() const {
    std::cout << "\n=== PANOU REALIZARI (" << realizariDeblocateTotal << "/" << listaRealizari.size() << ") ===\n";
    for(const auto& r : listaRealizari) {
        if(r.deblocata) {
            std::cout << "[DEBLOCAT] " << r.titlu << "\n";
        } else {
            std::cout << "[LOCKED]   " << r.titlu << " ( ??? ) \n";
        }
    }
    std::cout << "======================================\n";
}

int ManagerRealizari::getScorRealizari() const {
    return realizariDeblocateTotal;
}