#include "Realizari.h"
#include <iostream>

ManagerRealizari::ManagerRealizari() : realizariDeblocateTotal(0) {
    listaRealizari.emplace_back("Inceput Promitator", "Strange primii tai 2000 RON.");
    listaRealizari.emplace_back("Magnat Local", "Strange o avere de 5000 RON.");
    listaRealizari.emplace_back("Supravietuitor", "Rezista timp de 5 zile fara sa dai faliment.");
    listaRealizari.emplace_back("Veteran", "Rezista timp de 10 zile in industrie.");
    listaRealizari.emplace_back("Echipa Completa", "Angajeaza cel putin 3 bucatari.");
    listaRealizari.emplace_back("Atmosfera de Vis", "Imbunatateste decorul la nivelul 3.");
    listaRealizari.emplace_back("Restaurant de Lux", "Obtine un rating perfect de 5 stele.");
    listaRealizari.emplace_back("La Limita", "Termina o zi cu mai putin de 100 RON in cont.");
}

void ManagerRealizari::VerificaProgres(double bani, int zile, int bucatari, int decor, int stele) {
    bool cevaNou = false;

    if (!listaRealizari[0].deblocata && bani >= 2000) {
        listaRealizari[0].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[1].deblocata && bani >= 5000) {
        listaRealizari[1].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[2].deblocata && zile >= 5) {
        listaRealizari[2].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[3].deblocata && zile >= 10) {
        listaRealizari[3].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[4].deblocata && bucatari >= 3) {
        listaRealizari[4].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[5].deblocata && decor >= 3) {
        listaRealizari[5].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[6].deblocata && stele >= 5) {
        listaRealizari[6].deblocata = true;
        cevaNou = true;
    }

    if (!listaRealizari[7].deblocata && bani < 100 && bani > 0) {
        listaRealizari[7].deblocata = true;
        cevaNou = true;
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