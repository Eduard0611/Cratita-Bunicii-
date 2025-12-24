#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <filesystem>
#include "Produs.h"
#include "Ingredient.h"
#include "Angajat.h"
#include "Restaurant.h"
#include "Meniu.h"
#include "Stoc.h"
#include "Masa.h"
#include "Exceptii.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    try {
        stoc Stoc;
        std::ifstream fin1("Informatii/Stoc.txt");
        if (!fin1.is_open()) {
            throw EroareFisierCritica("Informatii/Stoc.txt");
        }
        std::string cuv; double cant, pretunitar;
        while (fin1 >> cuv >> cant >> pretunitar) Stoc.AdaugaIngredienteInStoc(ingredient(cuv, cant,pretunitar));
        fin1.close();

        meniu Meniu;
        std::ifstream fin2("Informatii/Meniu.txt");
        if (!fin2.is_open()) {
            throw EroareFisierCritica("Informatii/Meniu.txt");
        }
        double pret;
        while (fin2 >> cuv >> pret) {
            if (cuv.find("Vin") != std::string::npos || cuv.find("Bere") != std::string::npos || cuv.find("Suc") != std::string::npos) {
                double alcool = (cuv.find("Suc") != std::string::npos) ? 0.0 : 12.0;
                Meniu.AdaugaProdus(new bautura(cuv, pret, alcool));
            }
            else if (cuv.find("Papanasi") != std::string::npos || cuv.find("Clatite") != std::string::npos || cuv.find("Lava") != std::string::npos) {
                Meniu.AdaugaProdus(new desert(cuv, pret, true));
            }
            else {
                Meniu.AdaugaProdus(new mancare(cuv, pret));
            }
        }
        fin2.close();

        std::ifstream fin3("Informatii/Produse.txt");
        if (!fin3.is_open()) {
            throw EroareFisierCritica("Informatii/Produse.txt");
        }
        std::string numeProdus, numeIng; double cantIng;
        while (fin3 >> numeProdus >> numeIng >> cantIng) {
            if (auto* p = Meniu.CautaProdus(numeProdus))
                p->AdaugaIngredienteInProdus(numeIng, cantIng);
        }
        fin3.close();

        restaurant Restaurant("Cratita Bunicii");
        std::ifstream fin4("Informatii/Angajati.txt");
        if (!fin4.is_open()) {
            throw EroareFisierCritica("Informatii/Angajati.txt");
        }
        std::string numeAngajat, functie; double salariu;
        while (fin4 >> numeAngajat >> functie >> salariu) {
            Restaurant.Angajeaza(angajat(numeAngajat, functie, salariu));
        }
        fin4.close();

        std::ifstream fin5("Informatii/Mese.txt");
        if (!fin5.is_open()) {
            throw EroareFisierCritica("Informatii/Mese.txt");
        }
        int idMasa, capMasa;
        while (fin5 >> idMasa >> capMasa) {
            Restaurant.AdaugaMasa(masa(idMasa, capMasa));
        }
        fin5.close();

        std::cout << Meniu;

        bool jocActiv = true;
        double profitTotal = 1500.0;
        int ziuaCurenta = 1;

        std::cout << "\nBun venit! Apasati ENTER pentru a incepe simularea restaurantului...";
        std::cin.get();

        while (jocActiv) {
            Restaurant.ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta, jocActiv);

            if (!jocActiv) {
                std::cout << "Jocul a fost inchis manual. La revedere!\n";
                break;
            }

            if (profitTotal < 0) {
                std::cout << "\n!!! FALIMENT !!!\n";
                std::cout << "Ati ramas fara bani. Restaurantul a fost inchis de banca.\n";
                std::cout << "Ati rezistat " << ziuaCurenta << " zile.\n";
                jocActiv = false;
            } else {
                ziuaCurenta++;
            }
        }

    } catch (const std::exception& e) {
        std::cout << "\n\n[FATAL ERROR] Programul s-a oprit neasteptat:\n";
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}