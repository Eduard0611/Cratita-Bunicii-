#include "Restaurant.h"
#include "Exceptii.h"
#include "Evenimente.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <algorithm>
#include <map>


restaurant::restaurant(std::string nume) : nume(std::move(nume)), nivelDecor(0), nivelPublicitate(0), gradMurdarie(0.0) {}

void restaurant::AdaugaMasa(const masa& m) {
    Mese.push_back(m);
}

void restaurant::incepeZiua(int ziuaCurenta, stoc& Stoc, double& profitTotal) {
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
    std::cout << "Grad Murdarie: " << gradMurdarie << "%\n";

    int sansaEveniment = rand() % 100;
    if (sansaEveniment < 20) {
        InspectieSanitara inspectie;
        inspectie.Executa(*this, Stoc, profitTotal);
    } else if (sansaEveniment < 35) {
        PanaCurent pana;
        pana.Executa(*this, Stoc, profitTotal);
    }

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
        std::cout << "Nu sunt mese libere! Comanda pleaca...\n";

        int tipClient = rand() % 100;
        if (tipClient < 10) {
            Influencer inf;
            inf.Executa(*this, Stoc, profitTotal, costTotalComanda, false);
        } else if (tipClient < 20) {
            CriticCulinar critic;
            critic.Executa(*this, Stoc, profitTotal, costTotalComanda, false);
        }

        comenziRefuzate++;
        return;
    }

    int idMasaAleasa;
    std::cout << "Asignati masa (ID sau 0 refuz): ";
    std::cin >> idMasaAleasa;


    if (idMasaAleasa == 0) {
        std::cout << "Comanda refuzata manual.\n";

        int tipClient = rand() % 100;
        if (tipClient < 10) {
            Influencer inf;
            inf.Executa(*this, Stoc, profitTotal, costTotalComanda, false);
        } else if (tipClient < 20) {
            CriticCulinar critic;
            critic.Executa(*this, Stoc, profitTotal, costTotalComanda, false);
        }

        comenziRefuzate++;
        return;
    }

    try {
        bool masaGasita = false;
        for (auto& m : Mese) {
            if (m.getId() == idMasaAleasa) {
                masaGasita = true;

                if (m.isOcupata()) {
                    throw EroareComandaMasa("Masa selectata este deja OCUPATA!");
                }
                if (m.getCapacitate() < numarClienti) {
                    throw EroareComandaMasa("Capacitate insuficienta (Clienti: " + std::to_string(numarClienti) + ", Locuri: " + std::to_string(m.getCapacitate()) + ")");
                }

                std::map<std::string, double> necesarTotal;
                for(const auto& item : comandaCurenta) {
                    for(const auto& ing : item.first->getIngrediente()) {
                        necesarTotal[ing.nume] += ing.cantitate * item.second;
                    }
                }

                const std::vector<ingredient>& stocReal = Stoc.getStoc();
                for(const auto& par : necesarTotal) {
                    bool ingGasit = false;
                    for(const auto& ingStoc : stocReal) {
                        if(ingStoc.getNume() == par.first) {
                            ingGasit = true;
                            if(ingStoc.getCantitate() < par.second) {
                                throw EroareStocInsuficient(par.first);
                            }
                            break;
                        }
                    }
                    if(!ingGasit) throw EroareStocInsuficient(par.first + " (Lipsa totala)");
                }

                for(const auto& par : necesarTotal) {
                    Stoc.Consuma(par.first, par.second);
                }

                int durataOcupare = 2 + (rand() % 3);
                m.setOcupata(true, durataOcupare);
                profitTotal += costTotalComanda;
                comenziFinalizate++;
                CresteMurdarie(5.0);

                std::cout << "Succes! Masa " << m.getId() << " ocupata pentru " << durataOcupare << " ture.\n";

                int tipClient = rand() % 100;
                if (tipClient < 10) {
                    Influencer inf;
                    inf.Executa(*this, Stoc, profitTotal, costTotalComanda, true);
                } else if (tipClient < 20) {
                    CriticCulinar critic;
                    critic.Executa(*this, Stoc, profitTotal, costTotalComanda, true);
                }

                break;
            }
        }

        if (!masaGasita) {
            throw EroareComandaMasa("ID-ul mesei nu exista in restaurant!");
        }

    } catch (const EroareRestaurant& e) {
        std::cout << "\n[!] COMANDA ANULATA: " << e.what() << "\n";
        comenziRefuzate++;
    } catch (const std::exception& e) {
        std::cout << "\n[!] Eroare neasteptata: " << e.what() << "\n";
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
        std::cout << "Grad Murdarie: " << gradMurdarie << "%\n";
        std::cout << "1. Cumpara Masa Noua (Cost: 200 RON)\n";
        std::cout << "2. Upgrade Decor (Cost: 300 RON) -> Creste preturile cu 5%\n";
        std::cout << "3. Publicitate (Cost: 150 RON) -> Mai multi clienti maine\n";
        std::cout << "4. Curatenie Generala (Cost: 100 RON)\n";
        std::cout << "5. Start Ziua Urmatoare\n";
        std::cout << "6. Iesire din Joc\n";
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
            if(profitTotal >= 100) {
                profitTotal -= 100;
                CurataRestaurant();
                std::cout << "Restaurantul a fost curatat luna!\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 5) {
            break;
        }
        else if (optiune == 6) {
            jocActiv = false;
            break;
        }
    }
}

void restaurant::finalizeazaZiua(const stoc& Stoc, double& profitTotal) {
    for (const auto& i : Angajati) {
        profitTotal -= i.getSalariu();
    }

    if (eficienta <= 0.5) stele = 1;
    else if (eficienta <= 0.7) stele = 2;
    else if (eficienta <= 0.9) stele = 3;
    else stele = 5;

    std::cout << "Rating curent: " << stele << " Stele\n";
    std::cout << "Profit ramas dupa plata salarilor: " << profitTotal << " RON\n";

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
    incepeZiua(ziuaCurenta, Stoc, profitTotal);

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