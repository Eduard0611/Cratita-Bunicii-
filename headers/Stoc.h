#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Ingredient.h"

class stoc {
private:
    std::vector<ingredient> Stocul_Restaurantului;

public:

    void AdaugaIngredienteInStoc(const ingredient& ingredient);

    std::vector<ingredient>& getStoc();

    bool Consuma(const std::string& nume, double cantitate);

    void SalveazaStoc(const std::string& numeFisier) const;

    friend std::ostream& operator<<(std::ostream& out, const stoc& s);
};