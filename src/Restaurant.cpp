#include "Restaurant.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <cstdlib>

restaurant::restaurant(std::string nume) : nume(std::move(nume)), nivelDecor(0), nivelPublicitate(0) {}

void restaurant::AdaugaMasa(const masa& m) {
    Mese.push_back(m);
}

void restaurant::incepeZiua(int ziuaCurenta, const stoc& Stoc) {
    comenziFinalizate = 0;
    comenziRefuzate = 0;
    eficienta = 0.0;

    for(auto& m : Mese) {
        m.setOcupata(false);
    }

    std::cout << "\n==========================================\n";
    std::cout << "           ZIUA " << ziuaCurenta << " A INCEPUT\n";
    std::cout << "==========================================\n";
    std::cout << "Nivel Decor: " << nivelDecor << " (Bonus pret: " << nivelDecor * 5 << "%)\n";
    std::cout << "Nivel Publicitate: " << nivelPublicitate << " (Flux clienti marit)\n";
    std::cout << "\n--- Stoc disponibil ---\n";
    std::cout << Stoc;
}

void restaurant::actualizeazaMese() {
    bool SauEliberatMese = false;
    for(auto& m : Mese) {
        if(m.isOcupata()) {
            m.scadeTimp();
            if(m.getTimpRamas() <= 0) {
                m.setOcupata(false);
                std::cout << "[INFO] Masa " << m.getId() << " s-a eliberat.\n";
                SauEliberatMese = true;
            }
        }
    }
    if(SauEliberatMese) std::cout << "\n";
}

void restaurant::gestioneazaComanda(const meniu& Meniu, stoc& Stoc, double& profitTotal) {
    actualizeazaMese();

    int numarClienti = 1 + rand() % 8;
    std::vector<std::pair<produs*, int>> comandaCurenta;
    const auto &produseDisponibile = Meniu.getProduse();

    if (produseDisponibile.empty()) {
        std::cout << "Nu exista produse in meniu!\n";
        return;
    }

    int numarProduseComandate = numarClienti + (rand() % (numarClienti + 2));
    double costTotalComanda = 0.0;

    std::cout << "\n>>> COMANDA NOUA IN ASTEPTARE >>>\n";
    std::cout << "Clienti la usa: " << numarClienti << "\n";

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

    double bonusDecor = costTotalComanda * (nivelDecor * 0.05);
    costTotalComanda += bonusDecor;

    for(const auto& item : comandaCurenta) {
        std::cout << " - " << item.second << " x " << item.first->getNume() << "\n";
    }
    std::cout << "Valoare comanda: " << costTotalComanda << " RON (din care bonus decor: " << bonusDecor << " RON)\n";

    std::cout << "\nStatus Mese:\n";
    bool meseLibere = false;
    for(const auto& m : Mese) {
        std::cout << m << "\n";
        if (!m.isOcupata()) meseLibere = true;
    }

    if (!meseLibere) {
        std::cout << "COMBAT: Nu sunt mese libere! Comanda pleaca...\n";
        comenziRefuzate++;
        return;
    }

    int idMasaAleasa;
    std::cout << "Asignati masa (ID sau 0 refuz): ";
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
                std::cout << "Eroare: Masa ocupata! Comanda pierduta.\n";
                comenziRefuzate++;
                return;
            }
            if (m.getCapacitate() < numarClienti) {
                std::cout << "Eroare: Masa prea mica! Clientii au plecat suparati.\n";
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
            const std::vector<ingredient>& stocReal = Stoc.getStoc();

            for(const auto& par : necesarTotal) {
                bool ingGasit = false;
                for(const auto& ingStoc : stocReal) {
                    if(ingStoc.getNume() == par.first) {
                        ingGasit = true;
                        if(ingStoc.getCantitate() < par.second) {
                            stocSuficient = false;
                            std::cout << "[EROARE STOC] Insuficient: " << par.first
                                      << " (Necesar: " << par.second
                                      << ", Disponibil: " << ingStoc.getCantitate() << ")\n";
                        }
                        break;
                    }
                }
                if(!ingGasit) {
                    stocSuficient = false;
                    std::cout << "[EROARE STOC] Ingredient lipsa din inventar: " << par.first
                              << " (Verifica Produse.txt vs Stoc.txt)\n";
                }

                if(!stocSuficient) break;
            }

            if(stocSuficient) {
                for(const auto& par : necesarTotal) {
                    Stoc.Consuma(par.first, par.second);
                }
                int durataOcupare = 2 + (rand() % 3);
                m.setOcupata(true, durataOcupare);
                profitTotal += costTotalComanda;
                comenziFinalizate++;
                std::cout << "Succes! Masa " << m.getId() << " ocupata pentru " << durataOcupare << " ture.\n";
            } else {
                std::cout << "Lipsa stoc! Comanda anulata.\n";
                comenziRefuzate++;
            }
            break;
        }
    }

    if (!masaGasita) {
        std::cout << "ID Masa invalid. Comanda anulata.\n";
        comenziRefuzate++;
    }
}

void restaurant::incheieTranzactiile(const stoc& Stoc, double profitTotal) {
    if (comenziFinalizate + comenziRefuzate > 0)
        eficienta = static_cast<double>(comenziFinalizate) / (comenziFinalizate + comenziRefuzate);
    else
        eficienta = 0.0;

    std::cout << "\n--- Rezumatul Zilei ---\n";
    std::cout << "Eficienta: " << eficienta * 100 << "%\n";
    std::cout << "Bani curenti: " << profitTotal << " RON\n";
    std::cout << "\n--- Stoc ramas la finalul zilei ---\n" << Stoc;
}

void restaurant::aprovizionareAutomata(stoc& Stoc, double& profitTotal) const {
    double costSalarii = getSalariiAngajati();
    if (profitTotal < costSalarii) {
        std::cout << "Fonduri insuficiente pentru aprovizionare automata.\n";
        return;
    }

    double bugetAprovizionare = profitTotal - costSalarii;
    std::cout << "\n--- Aprovizionare Automata (Buget: " << bugetAprovizionare << " RON) ---\n";

    std::vector<ingredient*> necesar;
    double costMinim = 0;
    for (auto& ing : Stoc.getStoc()) {
        if (ing.getCantitate() < 10.0) {
            necesar.push_back(&ing);
            costMinim += ing.getPretAchizitie();
        }
    }

    if(necesar.empty() || costMinim > bugetAprovizionare) {
        std::cout << "Nu e necesara aprovizionarea sau bani insuficienti.\n";
        return;
    }

    double unitati = std::floor(bugetAprovizionare / costMinim);
    if(unitati > 20) unitati = 20;

    if (unitati >= 1) {
        double costTotal = 0;
        for (auto &ing: necesar) {
            ing->scadeCantitate(-unitati);
            costTotal += unitati * ing->getPretAchizitie();
            std::cout << "+ " << unitati << " " << ing->getNume() << "\n";
        }
        profitTotal -= costTotal;
        std::cout << "Cost aprovizionare: " << costTotal << " RON.\n";
    }
}

void restaurant::aprovizionareManuala(stoc& Stoc, double& profitTotal) {
    std::cout << "\n--- Aprovizionare Manuala ---\n";
    for (auto& ing : Stoc.getStoc()) {
        std::cout << ing.getNume() << " (Stoc: " << ing.getCantitate() << ") - Pret: " << ing.getPretAchizitie() << " RON. Cumperi? (0=Nu, >0=Cantitate): ";
        int cant;
        std::cin >> cant;
        if(cant > 0) {
            double cost = cant * ing.getPretAchizitie();
            if(profitTotal >= cost) {
                profitTotal -= cost;
                ing.scadeCantitate(-cant);
                std::cout << "Cumparat!\n";
            } else {
                std::cout << "Fonduri insuficiente!\n";
            }
        }
    }
}

void restaurant::gestioneazaAprovizionare(stoc& Stoc, double& profitTotal) const {
    std::cout << "\nAlegeti aprovizionarea:\n1. Automata\n2. Manuala\n3. Sari peste\nOptiune: ";
    int raspuns;
    std::cin >> raspuns;
    if (raspuns == 1) aprovizionareAutomata(Stoc, profitTotal);
    else if (raspuns == 2) aprovizionareManuala(Stoc, profitTotal);
}

void restaurant::MeniuAdministrare(double& profitTotal, bool& jocActiv) {
    int optiune = 0;
    while(true) {
        std::cout << "\n=== ADMINISTRARE & SHOP ===\n";
        std::cout << "Buget Actual: " << profitTotal << " RON\n";
        std::cout << "1. Cumpara Masa Noua (Cost: 200 RON)\n";
        std::cout << "2. Upgrade Decor (Cost: 300 RON) -> Creste preturile cu 5%\n";
        std::cout << "3. Publicitate (Cost: 150 RON) -> Mai multi clienti maine\n";
        std::cout << "4. Start Ziua Urmatoare\n";
        std::cout << "5. Parasiti jocul\n";
        std::cout << "Alegeti: ";
        std::cin >> optiune;

        if (optiune == 1) {
            if(profitTotal >= 200) {
                profitTotal -= 200;
                int idNou = Mese.size() + 1;
                Mese.push_back(masa(idNou, 4));
                std::cout << "Masa " << idNou << " (Capacitate 4) a fost adaugata!\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 2) {
            if(profitTotal >= 300) {
                profitTotal -= 300;
                nivelDecor++;
                std::cout << "Decor imbunatatit la nivelul " << nivelDecor << "!\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 3) {
            if(profitTotal >= 150) {
                profitTotal -= 150;
                nivelPublicitate++;
                std::cout << "Campanie publicitara pornita! Nivel curent: " << nivelPublicitate << "\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 4) {
            break;
        }
        else if (optiune == 5) {
            jocActiv = false;
            break;
        }
    }
}

void restaurant::finalizeazaZiua(const stoc& Stoc, double& profitTotal) {
    std::cout << "\n--- Plata Salarii ---\n";
    for (const auto& i : Angajati) {
        profitTotal -= i.getSalariu();
    }

    if (eficienta <= 0.5) stele = 1;
    else if (eficienta <= 0.7) stele = 2;
    else if (eficienta <= 0.9) stele = 3;
    else stele = 5;

    std::cout << "Rating curent: " << stele << " Stele\n";
    std::cout << "Profit ramas dupa salarii: " << profitTotal << " RON\n";

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

void restaurant::ZiRestaurant(const meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta, bool& jocActiv) {
    incepeZiua(ziuaCurenta, Stoc);

    int numarComenziZi = 4 + (nivelPublicitate * 2);
    std::cout << "Astazi sunt asteptate aproximativ " << numarComenziZi << " grupuri de clienti.\n";

    for(int i = 0; i < numarComenziZi; ++i) {
        gestioneazaComanda(Meniu, Stoc, profitTotal);
    }

    incheieTranzactiile(Stoc, profitTotal);
    finalizeazaZiua(Stoc, profitTotal);
    gestioneazaAprovizionare(Stoc, profitTotal);

    if(profitTotal > 0) {
        MeniuAdministrare(profitTotal, jocActiv);
    } else {
        std::cout << "Nu puteti accesa magazinul (Fonduri insuficiente sau datorii).\n";
    }
}

std::ostream& operator<<(std::ostream& out, const restaurant& s) {
    out << "=========" << s.nume << " =========\n";
    for (const auto& ang : s.Angajati)
        out << ang << "\n";
    return out;
}