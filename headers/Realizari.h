#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

struct Realizare {
    std::string titlu;
    std::string descriere;
    bool deblocata;
    std::function<bool(double, int, int, int, int)> conditie;

    Realizare(std::string t, std::string d, std::function<bool(double, int, int, int, int)> c)
        : titlu(std::move(t)), descriere(std::move(d)), deblocata(false), conditie(std::move(c)) {}
};

class ManagerRealizari {
private:
    std::vector<Realizare> listaRealizari;
    int realizariDeblocateTotal;

public:
    ManagerRealizari();

    void VerificaProgres(double bani, int zile, int bucatari, int decor, int stele);
    void AfiseazaStatus() const;
    [[nodiscard]] int getScorRealizari() const;
};