#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Angajat.h"
#include "Stoc.h"
#include "Meniu.h"
#include "Masa.h"

class restaurant {
private:
    std::string nume;
    std::vector<angajat> Angajati;
    std::vector<masa> Mese;

    int comenziFinalizate = 0;
    int comenziRefuzate = 0;
    int stele = 0;
    double eficienta = 0.0;

    int nivelDecor = 0;
    int nivelPublicitate = 0;

    void incepeZiua(int ziuaCurenta, const stoc& Stoc);
    void actualizeazaMese();
    void gestioneazaComanda(const meniu& Meniu, stoc& Stoc, double& profitTotal);
    void incheieTranzactiile(const stoc& Stoc, double profitTotal);

    void aprovizionareAutomata(stoc& Stoc, double& profitTotal) const;
    static void aprovizionareManuala(stoc& Stoc, double& profitTotal);
    void gestioneazaAprovizionare(stoc& Stoc, double& profitTotal) const;

    void finalizeazaZiua(const stoc& Stoc, double& profitTotal);
    void MeniuAdministrare(double& profitTotal);

public:
    explicit restaurant(std::string nume);

    void Angajeaza(const angajat& x);
    void AdaugaMasa(const masa& m);

    [[nodiscard]] double getSalariiAngajati() const;

    void ZiRestaurant(const meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta);

    friend std::ostream& operator<<(std::ostream& out, const restaurant& s);
};