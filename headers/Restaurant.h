#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Angajat.h"
#include "Stoc.h"
#include "Meniu.h"
#include "Masa.h"
#include "Statistica.h"
#include "Realizari.h"

class restaurant {
private:
    std::string nume;
    std::vector<angajat*> Angajati;
    std::vector<masa> Mese;

    Statistica stats;
    ManagerRealizari achievements;

    int comenziFinalizate = 0;
    int comenziRefuzate = 0;
    int stele = 0;
    double eficienta = 0.0;

    int nivelDecor = 0;
    int nivelPublicitate = 0;
    double gradMurdarie = 0.0;

    void incepeZiua(int ziuaCurenta, stoc& Stoc, double& profitTotal);
    void actualizeazaMese();
    void gestioneazaComanda(const meniu& Meniu, stoc& Stoc, double& profitTotal);
    void incheieTranzactiile(const stoc& Stoc, double profitTotal);

    void aprovizionareAutomata(stoc& Stoc, double& profitTotal) const;
    static void aprovizionareManuala(stoc& Stoc, double& profitTotal);
    void gestioneazaAprovizionare(stoc& Stoc, double& profitTotal) const;

    void finalizeazaZiua(const stoc& Stoc, double& profitTotal, int ziuaCurenta);
    void MeniuAdministrare(double& profitTotal, bool& jocActiv);

    void afiseazaRaportFinal() const;

    [[nodiscard]]int getNrBucatari() const;
    [[nodiscard]]int getNrOspatari() const;

public:
    explicit restaurant(std::string nume);
    ~restaurant();

    void Angajeaza(angajat* x);
    void AdaugaMasa(const masa& m);

    [[nodiscard]] double getSalariiAngajati() const;

    [[nodiscard]] double getGradMurdarie() const { return gradMurdarie; }
    void CurataRestaurant() { gradMurdarie = 0.0; }
    void CresteMurdarie(double val) { gradMurdarie += val; }

    [[nodiscard]] int getNivelPublicitate() const { return nivelPublicitate; }
    void modificaPublicitate(int val) { nivelPublicitate += val; if(nivelPublicitate < 0) nivelPublicitate = 0; }

    void ZiRestaurant(const meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta, bool& jocActiv);

    friend std::ostream& operator<<(std::ostream& out, const restaurant& s);
};