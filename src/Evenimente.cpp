#include "Evenimente.h"
#include <iostream>
#include <limits>

std::string InspectieSanitara::getNume() const {
    return "Inspectie Sanitara";
}

void InspectieSanitara::Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda, bool areLocLaMasa) {
    (void)s;
    (void)costComanda;
    (void)areLocLaMasa;

    std::cout << "\n[!] ALERTA: Inspectorii sanitari au intrat in restaurant!\n";
    double murdarie = r.getGradMurdarie();
    std::cout << "Nivel actual de murdarie: " << murdarie << "%\n";

    if (murdarie > 50.0) {
        double amenda = 500.0;
        profitTotal -= amenda;
        std::cout << "[!] AMENDA! Restaurantul este prea murdar. Ati platit " << amenda << " RON.\n";
    } else {
        std::cout << "[OK] Curatenia este in limite acceptabile.\n";
    }
}

std::string PanaCurent::getNume() const {
    return "Pana de Curent";
}

void PanaCurent::Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda, bool areLocLaMasa) {
    (void)r;
    (void)costComanda;
    (void)areLocLaMasa;

    std::cout << "\n[!] ALERTA: Pana de curent majora! Frigiderele s-au oprit.\n";

    double costReparatie = 300.0;
    std::cout << "Reparati de urgenta? Cost: " << costReparatie << " RON.\n";
    std::cout << "1. Da\n";
    std::cout << "2. Nu (Pierzi 20% din stocul perisabil)\n";

    int optiune;
    std::cout << "Alegeti: ";
    std::cin >> optiune;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        optiune = 2;
    }

    if (optiune == 1) {
        if (profitTotal >= costReparatie) {
            profitTotal -= costReparatie;
            std::cout << "Curentul a revenit. Stocul este salvat.\n";
        } else {
            std::cout << "Fonduri insuficiente! Stocul se degradeaza.\n";
            s.DegradeazaStocPerisabil(0.20);
        }
    } else {
        std::cout << "Ati refuzat reparatia. Stocul perisabil s-a degradat.\n";
        s.DegradeazaStocPerisabil(0.20);
    }
}

std::string Influencer::getNume() const {
    return "Influencer";
}

void Influencer::Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda, bool areLocLaMasa) {
    (void)s;

    std::cout << "\n[!] EVENIMENT: Unul dintre clienti este un INFLUENCER faimos!\n";

    if (areLocLaMasa) {
        std::cout << "Influencerul cere toata comanda GRATIS (" << costComanda << " RON) pentru un Story.\n";
        std::cout << "1. Accepta (Publicitate +1)\n";
        std::cout << "2. Refuza (Publicitate -1)\n";
        int opt;
        std::cin >> opt;


        if (opt == 1) {
            profitTotal -= costComanda;
            r.modificaPublicitate(1);
            std::cout << "Ai acceptat. Ai platit nota din buzunar, dar Influencerul te-a promovat! Nivel Publicitate: " << r.getNivelPublicitate() << "\n";
        } else {
            r.modificaPublicitate(-1);
            std::cout << "Ai refuzat. Influencerul a platit, dar a lasat un review oribil. Nivel Publicitate: " << r.getNivelPublicitate() << "\n";
        }
    } else {
        std::cout << "Influencerul nu a primit masa si a plecat nervos! (Publicitate -1)\n";
        r.modificaPublicitate(-1);
    }
}

std::string CriticCulinar::getNume() const {
    return "Critic Culinar";
}

void CriticCulinar::Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda, bool areLocLaMasa) {
    (void)s;
    (void)costComanda;

    std::cout << "\n[!] EVENIMENT: Un CRITIC CULINAR se afla in acest grup!\n";
    if (areLocLaMasa) {
        std::cout << "(Criticul degusta preparatele...)\n";

        if (rand() % 2 == 0) {
            std::cout << "Criticul a fost IMPRESIONAT! (+200 RON Bonus + Publicitate)\n";
            profitTotal += 200;
            r.modificaPublicitate(1);
        } else {
            std::cout << "Criticul a fost DEZAMAGIT de mancare! (Publicitate -1)\n";
            r.modificaPublicitate(-1);
        }
    } else {
        std::cout << "Criticul nu a primit masa! (Publicitate -1)\n";
        r.modificaPublicitate(-1);
    }
}