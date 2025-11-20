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
    while (fin2 >> cuv >> pret) Meniu.AdaugaProduse(produs(cuv, pret));
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