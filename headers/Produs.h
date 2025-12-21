#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "IngNecesar.h"

class produs {
protected:
    std::string nume;
    double pret;
    std::vector<IngNecesar> Ingrediente;

    virtual void afisare(std::ostream& out) const;

public:
    produs(std::string nume, double pret);
    virtual ~produs();
    [[nodiscard]] virtual produs* clone() const = 0;

    [[nodiscard]] virtual double getPretVanzare() const;

    void AdaugaIngredienteInProdus(const std::string& numeIng, double cantitate);
    [[nodiscard]] double getPretBase() const;
    [[nodiscard]] const std::string& getNume() const;
    std::vector<IngNecesar>& getIngrediente();

    friend std::ostream& operator<<(std::ostream& out, const produs& p);
};

class mancare : public produs {
private:
    bool dePost;
protected:
    void afisare(std::ostream& out) const override;
public:
    mancare(std::string nume, double pret, bool dePost = false);
    produs* clone() const override;
};

class bautura : public produs {
private:
    double volumAlcool;
protected:
    void afisare(std::ostream& out) const override;
public:
    bautura(std::string nume, double pret, double volumAlcool);
    produs* clone() const override;

    double getPretVanzare() const override;
    [[nodiscard]] double getVolumAlcool() const;
};

class desert : public produs {
private:
    bool contineZahar;
protected:
    void afisare(std::ostream& out) const override;
public:
    desert(std::string nume, double pret, bool contineZahar = true);
    produs* clone() const override;
};