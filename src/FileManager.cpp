#include "FileManager.h"
#include "Exceptii.h"
#include "Produs.h"
#include <fstream>
#include <iostream>

void FileManager::IncarcaDate(stoc& S, meniu& M, restaurant& R) {
    IncarcaStoc(S, "Informatii/Stoc.txt");
    IncarcaMeniu(M, "Informatii/Meniu.txt");
    IncarcaRetete(M, "Informatii/Produse.txt");
    IncarcaAngajati(R, "Informatii/Angajati.txt");
    IncarcaMese(R, "Informatii/Mese.txt");
}

void FileManager::IncarcaStoc(stoc& S, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw EroareFisierCritica(path);
    }
    std::string cuv;
    double cant, pretunitar;
    while (fin >> cuv >> cant >> pretunitar) {
        S.AdaugaIngredienteInStoc(ingredient(cuv, cant, pretunitar));
    }
    fin.close();
}

void FileManager::IncarcaMeniu(meniu& M, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw EroareFisierCritica(path);
    }
    std::string cuv;
    double pret;
    while (fin >> cuv >> pret) {
        if (cuv.find("Vin") != std::string::npos || cuv.find("Bere") != std::string::npos || cuv.find("Suc") != std::string::npos) {
            double alcool = (cuv.find("Suc") != std::string::npos) ? 0.0 : 12.0;
            M.AdaugaProdus(new bautura(cuv, pret, alcool));
        }
        else if (cuv.find("Papanasi") != std::string::npos || cuv.find("Clatite") != std::string::npos || cuv.find("Lava") != std::string::npos) {
            M.AdaugaProdus(new desert(cuv, pret, true));
        }
        else {
            M.AdaugaProdus(new mancare(cuv, pret));
        }
    }
    fin.close();
}

void FileManager::IncarcaRetete(meniu& M, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw EroareFisierCritica(path);
    }
    std::string numeProdus, numeIng;
    double cantIng;
    while (fin >> numeProdus >> numeIng >> cantIng) {
        if (auto* p = M.CautaProdus(numeProdus)) {
            p->AdaugaIngredienteInProdus(numeIng, cantIng);
        }
    }
    fin.close();
}

void FileManager::IncarcaAngajati(restaurant& R, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw EroareFisierCritica(path);
    }
    std::string numeAngajat, functie;
    double salariu;
    while (fin >> numeAngajat >> functie >> salariu) {
        R.Angajeaza(angajat(numeAngajat, functie, salariu));
    }
    fin.close();
}

void FileManager::IncarcaMese(restaurant& R, const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw EroareFisierCritica(path);
    }
    int idMasa, capMasa;
    while (fin >> idMasa >> capMasa) {
        R.AdaugaMasa(masa(idMasa, capMasa));
    }
    fin.close();
}