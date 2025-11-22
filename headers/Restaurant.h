#pragma once

#include <string>
#include <vector>
#include <iostream>


#include "Angajat.h"
#include "Stoc.h"
#include "Meniu.h"

class restaurant {
private:
    std::string nume;
    std::vector<angajat> Angajati;
    char continuaZi = 'y';
    int comenziFinalizate = 0;
    int comenziRefuzate = 0;
    int stele = 0;
    double eficienta = 0.0;

    void incepeZiua(int ziuaCurenta, const stoc& Stoc);
    void gestioneazaComanda(meniu& Meniu, stoc& Stoc, double& profitTotal);
    void incheieTranzactiile(const stoc& Stoc, double profitTotal);

    void aprovizionareAutomata(stoc& Stoc, double& profitTotal) const;
    void aprovizionareManuala(stoc& Stoc, double& profitTotal) const;
    void gestioneazaAprovizionare(stoc& Stoc, double& profitTotal) const;
    
    void finalizeazaZiua(const stoc& Stoc, double& profitTotal);

public:

    explicit restaurant(std::string nume);

    void Angajeaza(const angajat& x);
    
    [[nodiscard]] double getSalariiAngajati() const;
    
    void ZiRestaurant(meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta);

    friend std::ostream& operator<<(std::ostream& out, const restaurant& s);
};