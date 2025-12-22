#include "Restaurant.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
restaurant::restaurant(std::string nume) : nume(std::move(nume)) {}

void restaurant::AdaugaMasa(const masa& m) {
    Mese.push_back(m);
}

void restaurant::incepeZiua(int ziuaCurenta, const stoc& Stoc) {
    continuaZi = 'y';
    comenziFinalizate = 0;
    comenziRefuzate = 0;
    eficienta = 0.0;

    for(auto& m : Mese) {
        m.setOcupata(false);
    }

    std::cout << "\n=== Ziua " << ziuaCurenta << " ===\n";
    std::cout << "\n--- Stoc disponibil la inceputul zilei ---\n";
    std::cout << Stoc;
}

void restaurant::actualizeazaMese() {
    bool SauEliberatMese = false;
    for(auto& m : Mese) {
        if(m.isOcupata()) {
            m.scadeTimp();
            if(m.getTimpRamas() <= 0) {
                m.setOcupata(false);
                std::cout << "[INFO] Masa " << m.getId() << " s-a eliberat (clientii au plecat).\n";
                SauEliberatMese = true;
            }
        }
    }
    if(SauEliberatMese) {
        std::cout << "------------------------------------------\n";
    }
}

void restaurant::gestioneazaComanda(const meniu& Meniu, stoc& Stoc, double& profitTotal) {
    actualizeazaMese();

    int numarClienti = 1 + rand() % 8;

    std::vector<std::pair<produs*, int>> comandaCurenta;
    const auto &produseDisponibile = Meniu.getProduse();

    if (produseDisponibile.empty()) {
        std::cout << "Nu exista produse in meniu!\n";
        continuaZi = 'n';
        return;
    }

    int numarProduseComandate = numarClienti + (rand() % (numarClienti + 1));
    double costTotalComanda = 0.0;

    std::cout << "\n==============Comanda=============\n";
    std::cout << "Numar clienti: " << numarClienti << "\n";

    for (int i = 0; i < numarProduseComandate; ++i) {
        int index = rand() % produseDisponibile.size();
        produs* p = produseDisponibile[index];

        bool gasit = false;
        for(auto& item : comandaCurenta) {
            if(item.first->getNume() == p->getNume()) {
                item.second++;
                gasit = true;
                break;
            }
        }
        if(!gasit) {
            comandaCurenta.push_back({p, 1});
        }
        costTotalComanda += p->getPretVanzare();
    }

    for(const auto& item : comandaCurenta) {
        std::cout << item.second << " x " << item.first->getNume() << "\n";
    }
    std::cout << "Total de plata: " << costTotalComanda << " RON\n";
    std::cout << "==================================\n";

    std::cout << "\nLista Mese (Status Actualizat):\n";
    bool meseLibere = false;
    for(const auto& m : Mese) {
        std::cout << m << "\n";
        if (!m.isOcupata()) meseLibere = true;
    }

    if (!meseLibere) {
        std::cout << "Ne pare rau, nu mai sunt mese libere! Comanda refuzata automat.\n";
        comenziRefuzate++;
        return;
    }

    int idMasaAleasa;
    std::cout << "\nAsignati o masa (introduceti ID-ul mesei sau 0 pentru a refuza comanda): ";
    std::cin >> idMasaAleasa;

    if (idMasaAleasa == 0) {
        std::cout << "Comanda refuzata manual.\n";
        comenziRefuzate++;
        return;
    }

    bool masaGasita = false;
    for (auto& m : Mese) {
        if (m.getId() == idMasaAleasa) {
            masaGasita = true;
            if (m.isOcupata()) {
                std::cout << "Eroare: Masa este deja ocupata! Comanda anulata.\n";
                comenziRefuzate++;
                return;
            }
            if (m.getCapacitate() < numarClienti) {
                std::cout << "Eroare: Capacitate insuficienta (" << m.getCapacitate() << " locuri vs " << numarClienti << " clienti)! Comanda anulata.\n";
                comenziRefuzate++;
                return;
            }

            std::map<std::string, double> necesarTotal;
            for(const auto& item : comandaCurenta) {
                for(const auto& ing : item.first->getIngrediente()) {
                    necesarTotal[ing.nume] += ing.cantitate * item.second;
                }
            }

            bool stocSuficient = true;
            const std::vector<ingredient>&  stocReal = Stoc.getStoc();

            for(const auto& par : necesarTotal) {
                std::string numeIngNecesar = par.first;
                double cantitateNecesara = par.second;
                bool ingredientGasitInStoc = false;

                for(const auto& ingStoc : stocReal) {
                    if(ingStoc.getNume() == numeIngNecesar) {
                        ingredientGasitInStoc = true;
                        if(ingStoc.getCantitate() < cantitateNecesara) {
                            stocSuficient = false;
                            std::cout << "Stoc insuficient pentru " << numeIngNecesar
                                      << "! Necesar total: " << cantitateNecesara
                                      << ", Disponibil: " << ingStoc.getCantitate() << "\n";
                        }
                        break;
                    }
                }
                if(!ingredientGasitInStoc) {
                    stocSuficient = false;
                    std::cout << "Ingredientul " << numeIngNecesar << " lipseste complet din stoc!\n";
                }

                if(!stocSuficient) break; // Nu are rost sa verificam mai departe
            }

            if(stocSuficient) {
                for(const auto& par : necesarTotal) {
                    Stoc.Consuma(par.first, par.second);
                }

                int durataOcupare = 2 + (rand() % 3);
                m.setOcupata(true, durataOcupare);

                profitTotal += costTotalComanda;
                comenziFinalizate++;
                std::cout << "Masa " << m.getId() << " a fost ocupata pentru urmatoarele " << durataOcupare << " comenzi.\n";
            } else {
                std::cout << "Comanda a fost refuzata din cauza lipsei de stoc.\n";
                comenziRefuzate++;
            }
            break;
        }
    }

    if (!masaGasita) {
        std::cout << "ID masa invalid! Comanda anulata.\n";
        comenziRefuzate++;
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
            ing->scadeCantitate(-cantitateDeCumparat);
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
            std::cout << "Cate unitati de " << ing.getNume() << " (" << ing.getPretAchizitie()
            << "RON/BUCATA" << ")"<< " doriti sa cumparati? ";
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

void restaurant::ZiRestaurant(const meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta) {
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