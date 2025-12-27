#pragma once
#include <string>
#include <vector>
#include <iostream>

struct Realizare {
    std::string titlu;
    std::string descriere;
    bool deblocata;

    Realizare(std::string t, std::string d) 
        : titlu(std::move(t)), descriere(std::move(d)), deblocata(false) {}
};

class ManagerRealizari {
private:
    std::vector<Realizare> listaRealizari;
    int realizariDeblocateTotal;

public:
    ManagerRealizari();

    void VerificaProgres(double bani, int zile, int bucatari, int decor, int stele);
    void AfiseazaStatus() const;
    int getScorRealizari() const;
};