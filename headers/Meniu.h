#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Produs.h"

class meniu {
private:
    std::vector<produs> produse;

public:

    void AdaugaProduse(const produs& p);

    std::vector<produs>& getProduse();

    produs* CautaProdus(const std::string& nume);

    friend std::ostream& operator<<(std::ostream& out, const meniu& m);
};