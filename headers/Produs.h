//
// Created by Eduard-Ionut on 20.11.2025.
//

#pragma once // Foarte important!
#include <string>
#include <vector>
#include <iostream>
#include "IngNecesar.h"

class produs {
private:
    std::string nume;
    double pret;
    std::vector<IngNecesar> Ingrediente;

public:
    produs(std::string nume, double pret);

    produs(const produs& other);

    produs& operator=(const produs& other);

    ~produs();

    void AdaugaIngredienteInProdus(const std::string& numeIng, double cantitate);

    [[nodiscard]] double getPret() const;
    [[nodiscard]] const std::string& getNume() const;
    std::vector<IngNecesar>& getIngrediente();

    friend std::ostream& operator<<(std::ostream& out, const produs& p);
};