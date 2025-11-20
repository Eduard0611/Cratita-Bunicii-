#pragma once

#include <vector>
#include <string>
#include <iostream>

// Includem produs.h pentru că folosim vector<produs>
#include "produs.h"

class meniu {
private:
    std::vector<produs> produse;

public:

    void AdaugaProduse(const produs& p);

    std::vector<produs>& getProduse();

    produs* CautaProdus(const std::string& nume);

    friend std::ostream& operator<<(std::ostream& out, const meniu& m);
};