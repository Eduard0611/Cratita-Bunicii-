#include "Restaurant.h"
#include "Exceptii.h"
#include "Evenimente.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <cstdlib>

restaurant::restaurant(std::string nume) : nume(std::move(nume)), nivelDecor(0), nivelPublicitate(0), gradMurdarie(0.0) {}

restaurant::~restaurant() {
    for (auto* a : Angajati) {
        delete a;
    }
    Angajati.clear();
}

void restaurant::AdaugaMasa(const masa& m) {
    Mese.push_back(m);
}

int restaurant::getNrBucatari() const {
    int count = 0;
    for (const auto* a : Angajati) {
        if (a->getFunctie() == "Bucatar") count++;
    }
    return count;
}

int restaurant::getNrOspatari() const {
    int count = 0;
    for (const auto* a : Angajati) {
        if (a->getFunctie() == "Ospatar") count++;
    }
    return count;
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

    int sansaEveniment = rand() % 100;
    ContextEveniment ctx{*this, Stoc, profitTotal, 0.0, false};

    if (sansaEveniment < 20) {
        InspectieSanitara inspectie;
        inspectie.Executa(ctx);
    } else if (sansaEveniment < 35) {
        PanaCurent pana;
        pana.Executa(ctx);
    }

    std::cout << "\n--- Stoc disponibil ---\n";
    std::cout << Stoc;

    std::cout << "\n--- Info Zi ---\n";
    std::cout << "Echipa: " << getNrBucatari() << " Bucatari | " << getNrOspatari() << " Ospatari\n";
    std::cout << "Nivel Decor: " << nivelDecor << " (Bonus pret: " << nivelDecor * 5 << "%)\n";
    std::cout << "Nivel Publicitate: " << nivelPublicitate << " (Flux clienti marit)\n";
    std::cout << "Grad Murdarie: " << gradMurdarie << "%\n";
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

void restaurant::GenerareComanda(const meniu& Meniu, int& numarClienti, std::vector<std::pair<produs*, int>>& comandaCurenta, double& costTotalComanda) const {
    const auto &produseDisponibile = Meniu.getProduse();
    if (produseDisponibile.empty()) return;

    numarClienti = 1 + rand() % 8;
    int numarProduseComandate = numarClienti + (rand() % (numarClienti + 2));
    costTotalComanda = 0.0;

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
            comandaCurenta.emplace_back(p, 1);
        }
        costTotalComanda += p->getPretVanzare();
    }

    double bonusChef = costTotalComanda * (getNrBucatari() * 0.05);
    costTotalComanda += bonusChef;
    double bonusDecor = costTotalComanda * (nivelDecor * 0.05);
    costTotalComanda += bonusDecor;

    for(const auto& item : comandaCurenta) {
        std::cout << " - " << item.second << " x " << item.first->getNume() << "\n";
    }
    std::cout << "Valoare comanda: " << costTotalComanda << " RON (Bonus Chef: " << bonusChef << ")\n";
}

masa* restaurant::CautaSiOcupaMasa(int numarClienti, stoc& Stoc, const std::vector<std::pair<produs*, int>>& comandaCurenta, double& profitTotal, double costTotalComanda) {
    std::cout << "\nStatus Mese:\n";
    bool meseLibere = false;
    for(const auto& m : Mese) {
        std::cout << m << "\n";
        if (!m.isOcupata()) meseLibere = true;
    }

    if (!meseLibere) {
        std::cout << "COMBAT: Nu sunt mese libere! Comanda pleaca...\n";
        return nullptr;
    }

    int idMasaAleasa;
    std::cout << "Asignati masa (ID sau 0 refuz): ";
    std::cin >> idMasaAleasa;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        idMasaAleasa = 0;
    }

    if (idMasaAleasa == 0) {
        std::cout << "Comanda refuzata manual.\n";
        return nullptr;
    }

    masa* masaGasita = nullptr;
    for (auto& m : Mese) {
        if (m.getId() == idMasaAleasa) {
            masaGasita = &m;
            break;
        }
    }

    if (!masaGasita) throw EroareComandaMasa("ID-ul mesei nu exista in restaurant!");
    if (masaGasita->isOcupata()) throw EroareComandaMasa("Masa selectata este deja OCUPATA!");
    if (masaGasita->getCapacitate() < numarClienti) throw EroareComandaMasa("Capacitate insuficienta!");

    std::map<std::string, double> necesarTotal;
    for(const auto& item : comandaCurenta) {
        for(const auto& ing : item.first->getIngrediente()) {
            necesarTotal[ing.nume] += ing.cantitate * item.second;
        }
    }

    Stoc.VerificaSiConsuma(necesarTotal);

    int durataOcupare = 2 + (rand() % 3);
    masaGasita->setOcupata(true, durataOcupare);
    profitTotal += costTotalComanda;
    CresteMurdarie(5.0);

    std::cout << "Succes! Masa " << masaGasita->getId() << " ocupata pentru " << durataOcupare << " ture.\n";
    return masaGasita;
}

void restaurant::TriggerEvenimentClient(stoc& Stoc, double& profitTotal, double costTotalComanda, bool areLocLaMasa) {
    int tipClient = rand() % 100;
    ContextEveniment ctx{*this, Stoc, profitTotal, costTotalComanda, areLocLaMasa};

    if (tipClient < 10) {
        Influencer inf;
        inf.Executa(ctx);
    } else if (tipClient < 20) {
        CriticCulinar critic;
        critic.Executa(ctx);
    }
}

void restaurant::gestioneazaComanda(const meniu& Meniu, stoc& Stoc, double& profitTotal) {
    actualizeazaMese();

    int numarClienti = 0;
    std::vector<std::pair<produs*, int>> comandaCurenta;
    double costTotalComanda = 0.0;

    GenerareComanda(Meniu, numarClienti, comandaCurenta, costTotalComanda);

    if(comandaCurenta.empty()) return;

    try {
        const masa* m = CautaSiOcupaMasa(numarClienti, Stoc, comandaCurenta, profitTotal, costTotalComanda);
        if (m) {
            comenziFinalizate++;
            TriggerEvenimentClient(Stoc, profitTotal, costTotalComanda, true);
        } else {
            comenziRefuzate++;
            TriggerEvenimentClient(Stoc, profitTotal, costTotalComanda, false);
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

    std::cout << "\n--- Stoc ramas la finalul zilei ---\n" << Stoc;

    std::cout << "\n--- Rezumatul Zilei ---\n";
    std::cout << "Eficienta: " << eficienta * 100 << "%\n";
    std::cout << "Bani inainte de salarii: " << profitTotal << " RON\n";
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

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cant = 0;
        }

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

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        raspuns = 3;
    }

    if (raspuns == 1) aprovizionareAutomata(Stoc, profitTotal);
    else if (raspuns == 2) aprovizionareManuala(Stoc, profitTotal);
}

void restaurant::afiseazaRaportFinal() const {
    std::cout << "\n\n======== RAPORT FINAL DE ACTIVITATE ========\n";
    stats.raportFinal();
    std::cout << "============================================\n";
    achievements.AfiseazaStatus();
    std::cout << "Scor Total Realizari: " << achievements.getScorRealizari() << "\n";
}

void restaurant::MeniuAdministrare(double& profitTotal, bool& jocActiv) {
    int optiune = 0;
    while(true) {
        std::cout << "\n=== ADMINISTRARE & SHOP ===\n";
        std::cout << "Buget Actual: " << profitTotal << " RON\n";
        std::cout << "Grad Murdarie: " << gradMurdarie << "%\n";
        std::cout << "Echipa: " << getNrBucatari() << " Bucatari, " << getNrOspatari() << " Ospatari\n";
        std::cout << "1. Cumpara Masa Noua (Cost: 200 RON) - Necesar: 1 Ospatar la 2 Mese\n";
        std::cout << "2. Upgrade Decor (Cost: 300 RON) -> Creste preturile cu 5%\n";
        std::cout << "3. Publicitate (Cost: 150 RON) -> Mai multi clienti maine\n";
        std::cout << "4. Curatenie Generala (Cost: 100 RON)\n";
        std::cout << "5. Angajeaza Bucatar (Cost: 500 RON, Salariu: 250)\n";
        std::cout << "6. Angajeaza Ospatar (Cost: 300 RON, Salariu: 150)\n";
        std::cout << "7. Start Ziua Urmatoare\n";
        std::cout << "8. Iesire din Joc\n";
        std::cout << "9. Vezi Realizari\n";
        std::cout << "Alegeti: ";
        std::cin >> optiune;

        if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             optiune = 0;
             std::cout << "Input invalid.\n";
             continue;
        }

        if (optiune == 1) {
            int limitaMese = getNrOspatari() * 2;
            if (static_cast<int>(Mese.size()) >= limitaMese) {
                std::cout << "Nu aveti suficienti ospatari! (Max 2 mese per ospatar)\n";
            }
            else {
                std::cout << "Cate locuri doriti la masa? (1 loc = 50 RON): ";
                int locuri;
                std::cin >> locuri;

                if (std::cin.fail() || locuri <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Numar de locuri invalid!\n";
                }
                else {
                    double costMasa = locuri * 50.0;

                    if (profitTotal >= costMasa) {
                        profitTotal -= costMasa;
                        int idNou = Mese.size() + 1;
                        Mese.emplace_back(idNou, locuri);
                        std::cout << "Masa " << idNou << " (Capacitate: " << locuri << ") a fost cumparata cu " << costMasa << " RON!\n";
                    } else {
                        std::cout << "Fonduri insuficiente! Ai nevoie de " << costMasa << " RON.\n";
                    }
                }
            }
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
            if(profitTotal >= 500) {
                profitTotal -= 500;
                Angajeaza(new angajat("Bucatar Nou", "Bucatar", 250));
                std::cout << "Bucatar angajat! Pretul comenzilor creste.\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 6) {
            if(profitTotal >= 300) {
                profitTotal -= 300;
                Angajeaza(new angajat("Ospatar Nou", "Ospatar", 150));
                std::cout << "Ospatar angajat! Puteti cumpara mese noi.\n";
            } else std::cout << "Fonduri insuficiente!\n";
        }
        else if (optiune == 7) {
            break;
        }
        else if (optiune == 8) {
            afiseazaRaportFinal();
            jocActiv = false;
            break;
        }
        else if (optiune == 9) {
            achievements.AfiseazaStatus();
        }
    }
}

void restaurant::finalizeazaZiua(const stoc& Stoc, double& profitTotal, int ziuaCurenta) {
    for (const auto* i : Angajati) {
        profitTotal -= i->getSalariu();
    }

    stats.adaugaZi(profitTotal, comenziFinalizate, comenziRefuzate, stele);

    if (eficienta <= 0.5) stele = 1;
    else if (eficienta <= 0.7) stele = 2;
    else if (eficienta <= 0.9) stele = 3;
    else stele = 5;

    std::cout << "Rating curent: " << stele << " Stele\n";
    std::cout << "Profit ramas dupa salarii: " << profitTotal << " RON\n";

    achievements.VerificaProgres(profitTotal, ziuaCurenta, getNrBucatari(), nivelDecor, stele);

    Stoc.SalveazaStoc("Informatii/StocActualizat.txt");
}

void restaurant::Angajeaza(angajat* x) {
    Angajati.push_back(x);
}

double restaurant::getSalariiAngajati() const {
    double salarii = 0.0;
    for (const auto* i : Angajati) {
        salarii += i->getSalariu();
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
    finalizeazaZiua(Stoc, profitTotal, ziuaCurenta);
    gestioneazaAprovizionare(Stoc, profitTotal);

    if(profitTotal > 0) {
        MeniuAdministrare(profitTotal, jocActiv);
    } else {
        afiseazaRaportFinal();
        std::cout << "Nu puteti accesa magazinul (Fonduri insuficiente sau datorii).\n";
    }
}

std::ostream& operator<<(std::ostream& out, const restaurant& s) {
    out << "=========" << s.nume << " =========\n";
    for (const auto* ang : s.Angajati)
        out << *ang;
    return out;
}