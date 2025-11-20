#include "Restaurant.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>

restaurant::restaurant(std::string nume) : nume(std::move(nume)) {}


void restaurant::incepeZiua(int ziuaCurenta, const stoc& Stoc) {
    continuaZi = 'y';
    comenziFinalizate = 0;
    comenziRefuzate = 0;
    eficienta = 0.0;

    std::cout << "\n=== Ziua " << ziuaCurenta << " ===\n";
    std::cout << "\n--- Stoc disponibil la inceputul zilei ---\n";
    std::cout << Stoc;
}

void restaurant::gestioneazaComanda(meniu& Meniu, stoc& Stoc, double& profitTotal) {
    char raspuns = ' ';
    int cantitateProdus = 1 + rand() % 3;

    const auto &produse = Meniu.getProduse();
    if (produse.empty()) {
        std::cout << "Nu exista produse in meniu!\n";
        return;
    }

    int indexProdus = static_cast<int>(rand() % produse.size());
    // Facem o copie a produsului selectat
    produs p = produse[indexProdus]; 

    std::cout << "\nClientul doreste " << cantitateProdus << " x " << p.getNume() << "\n";
    std::cout << "Acceptati comanda? (y/n): ";
    std::cin >> raspuns;

    if (raspuns != 'y' && raspuns != 'Y') {
        std::cout << "Comanda a fost anulata.\n";
        comenziRefuzate++;
    } else {
        bool ok = true;
        for (const auto &ing: p.getIngrediente()) {
            if (!Stoc.Consuma(ing.nume, ing.cantitate * cantitateProdus)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            double profitComanda = p.getPret() * cantitateProdus;
            profitTotal += profitComanda;
            comenziFinalizate++;
            std::cout << "Comanda realizata! Profit: " << profitComanda << " RON\n";
        } else {
            std::cout << "Comanda nu poate fi realizata complet!\n";
            comenziRefuzate++;
        }
    }
}

void restaurant::incheieTranzactiile(const stoc& Stoc, double profitTotal) {
    std::cout << "\n--- Stoc actual ---\n" << Stoc;
    if (comenziFinalizate + comenziRefuzate > 0)
        eficienta = static_cast<double>(comenziFinalizate) / (comenziFinalizate + comenziRefuzate);
    else
        eficienta = 0.0;

    std::cout << "\nEficienta zilei: " << eficienta * 100 << "%\n";
    std::cout << "\nProfitul zilei (inainte de salarii si aprovizionare): "<< profitTotal<< " RON\n";
}

void restaurant::aprovizionareAutomata(stoc& Stoc, double& profitTotal) const {
    double costSalarii = getSalariiAngajati();

    if (profitTotal < costSalarii) {
        std::cout << "\nAtentie! Profit insuficient (" << profitTotal << " RON) pentru a acoperi salariile (" << costSalarii << " RON). Aprovizionarea Automata Anulata.\n";
        return;
    }

    double bugetAprovizionare = profitTotal - costSalarii;
    std::cout << "\n--- Aprovizionare Automata ---\n";
    std::cout << "Rezerva Salarii: " << costSalarii << " RON. Buget Aprovizionare: " << bugetAprovizionare << " RON.\n";

    constexpr double PRAG_MINIM = 5.0;
    std::vector<ingredient*> ingredienteNecesare;
    double costPentruOUnitateDinFiecare = 0.0;

    for (auto& ing : Stoc.getStoc()) {
        if (ing.getCantitate() < PRAG_MINIM) {
            ingredienteNecesare.push_back(&ing);
            costPentruOUnitateDinFiecare += ing.getPretAchizitie();
        }
    }

    if (ingredienteNecesare.empty() || costPentruOUnitateDinFiecare == 0.0) {
        std::cout << "Nu este necesara aprovizionarea.\n";
        return;
    }

    double cantitateDeCumparat = std::floor(bugetAprovizionare / costPentruOUnitateDinFiecare);
    double unitatiDoriteMax = std::numeric_limits<double>::max();

    for (const ingredient* ing : ingredienteNecesare) {
        double cantitateDeficit = PRAG_MINIM - ing->getCantitate();
        if (cantitateDeficit > 0) {
            constexpr double MAX_UNITATI_DE_ADAUGAT = 15.0;
            unitatiDoriteMax = std::min(unitatiDoriteMax, MAX_UNITATI_DE_ADAUGAT - ing->getCantitate());
        }
    }

    if (unitatiDoriteMax < 1) unitatiDoriteMax = 1;

    cantitateDeCumparat = std::min(cantitateDeCumparat, unitatiDoriteMax);

    if (cantitateDeCumparat >= 1) {
        double costFinalAprovizionare = 0.0;
        std::cout << "Se vor cumpara " << cantitateDeCumparat << " unitati din fiecare ingredient necesar.\n";

        for (auto &ing: ingredienteNecesare) {
            double costCumparat = cantitateDeCumparat * ing->getPretAchizitie();
            ing->scadeCantitate(-cantitateDeCumparat); // Scadere negativa = Adunare
            costFinalAprovizionare += costCumparat;

            std::cout << "Aprovizionat " << ing->getNume() << " cu " << cantitateDeCumparat
                    << " unitati. Cost: " << costCumparat << " RON.\n";
        }

        profitTotal -= costFinalAprovizionare;
        std::cout << "Cost total aprovizionare: " << costFinalAprovizionare << " RON.\n";
    } else {
        std::cout << "Bugetul ramas nu permite cumpararea nici macar a unei unitati din fiecare ingredient necesar.\n";
    }
}

void restaurant::aprovizionareManuala(stoc& Stoc, double& profitTotal) const {
    std::cout << "\n--- Aprovizionare manuala ---\n";
    std::cout << "Tineti cont de salariile angajatilor: " << getSalariiAngajati() << " RON si banii disponibili: " << profitTotal << std::endl;
    
    for (auto& ing : Stoc.getStoc()) {
        int cantCumparata;
        do {
            std::cout << "Cate unitati de " << ing.getNume() << " doriti sa cumparati? ";
            std::cin >> cantCumparata;
            if (static_cast<double>(cantCumparata) * ing.getPretAchizitie() > profitTotal)
                std::cout << "Bani insuficienti. Prea multe unitati." << std::endl;
        } while (static_cast<double>(cantCumparata) * ing.getPretAchizitie() > profitTotal);
        
        ing.scadeCantitate(-cantCumparata);
        profitTotal -= static_cast<double>(cantCumparata) * ing.getPretAchizitie();
    }
}

void restaurant::gestioneazaAprovizionare(stoc& Stoc, double& profitTotal) const {
    char raspuns;
    std::cout << "Doriti aprovizionare automata (1) sau aprovizionare manuala (2):";
    std::cin >> raspuns;
    if (raspuns == '1') {
        aprovizionareAutomata(Stoc, profitTotal);
    } else {
        aprovizionareManuala(Stoc, profitTotal);
    }
}

void restaurant::finalizeazaZiua(const stoc& Stoc, double& profitTotal) {
    std::cout << "\n";
    for (const auto& i : Angajati) {
        profitTotal -= i.getSalariu();
        std::cout << i << std::endl;
    }

    std::cout << "Profitul final este: " << profitTotal << " RON" << std::endl;

    // Calcul Stele
    if (eficienta <= 0.5) stele = 1;
    else if (eficienta <= 0.7) stele = 2;
    else if (eficienta <= 0.9) stele = 3;
    else stele = 5;

    std::cout << "\nRating restaurant pe aceasta zi: " << stele << " Stea(e)\n";
    

    Stoc.SalveazaStoc("Informatii/StocActualizat.txt");
}

void restaurant::Angajeaza(const angajat& x) {
    Angajati.push_back(x);
}

double restaurant::getSalariiAngajati() const {
    double salarii = 0.0;
    for (const auto& i : Angajati) {
        salarii += i.getSalariu();
    }
    return salarii;
}

void restaurant::ZiRestaurant(meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta) {
    incepeZiua(ziuaCurenta, Stoc);

    while (continuaZi == 'y' || continuaZi == 'Y') {
        gestioneazaComanda(Meniu, Stoc, profitTotal);

        std::cout << "\nDoriti o alta comanda azi? (y/n): ";
        std::cin >> continuaZi;
    }

    incheieTranzactiile(Stoc, profitTotal);
    gestioneazaAprovizionare(Stoc, profitTotal);
    finalizeazaZiua(Stoc, profitTotal);
}

std::ostream& operator<<(std::ostream& out, const restaurant& s) {
    out << "=========" << s.nume << " =========\n";
    for (const auto& ang : s.Angajati)
        out << ang << "\n";
    return out;
}