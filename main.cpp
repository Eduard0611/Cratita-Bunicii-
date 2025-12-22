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

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    stoc Stoc;
    std::ifstream fin1("Informatii/Stoc.txt");
    if (!fin1.is_open()) {
        std:: cout << "Eroare: nu s-a putut deschide fisierul Stoc pentru citire!\n";
        return 1;
    }
    std::string cuv; double cant, pretunitar;
    while (fin1 >> cuv >> cant >> pretunitar) Stoc.AdaugaIngredienteInStoc(ingredient(cuv, cant,pretunitar));
    fin1.close();

    meniu Meniu;
    std::ifstream fin2("Informatii/Meniu.txt");
    if (!fin2.is_open()) {
        std::cout << "Eroare: nu s-a putut deschide fisierul Meniu pentru citire!\n";
        return 1;
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
        std::cout << "Eroare: nu s-a putut deschide fisierul Produse pentru citire!\n";
        return 1;
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
        std::cout << "Eroare: nu s-a putut deschide fisierul Angajati pentru citire!\n";
        return 1;
    }
    std::string numeAngajat, functie; double salariu;
    while (fin4 >> numeAngajat >> functie >> salariu) {
        Restaurant.Angajeaza(angajat(numeAngajat, functie, salariu));
    }
    fin4.close();

    std::ifstream fin5("Informatii/Mese.txt");
    if (!fin5.is_open()) {
        std::cout << "Eroare: nu s-a putut deschide fisierul Mese pentru citire!\n";
        return 1;
    }
    int idMasa, capMasa;
    while (fin5 >> idMasa >> capMasa) {
        Restaurant.AdaugaMasa(masa(idMasa, capMasa));
    }
    fin5.close();

    std::cout << Meniu;

    bool restaurantDeschis = true;
    double profitTotal = 100.0;
    int ziuaCurenta = 1;
    char continua;

    std::cout << "\nDoriti sa deschideti restaurantul si sa primiti clienti? (y/n): ";
    std::cin >> continua;
    if (continua != 'y' && continua != 'Y') {
        restaurantDeschis = false;
    }
    while (restaurantDeschis) {
        Restaurant.ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta);

        std::cout << "\nDoriti sa deschideti restaurantul ziua urmatoare si sa primiti clienti? (y/n): ";
        std::cin >> continua;
        if (continua != 'y' && continua != 'Y') {
            restaurantDeschis = false;
        }
        else ziuaCurenta++;
    }

    std::cout << "\nRestaurantul s-a inchis definitiv.\nProfit total final: " << profitTotal << " RON\n";

    return 0;
}