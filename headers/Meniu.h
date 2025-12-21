#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Produs.h"

class meniu {
private:
    std::vector<produs*> produse;

    void sterge();
    void copiaza(const std::vector<produs*>& altele);

public:
    meniu() = default;

    ~meniu();
    meniu(const meniu& other);
    meniu& operator=(const meniu& other);

    void AdaugaProdus(produs* p);

    const std::vector<produs*>& getProduse() const;
    produs* CautaProdus(const std::string& nume);

    friend std::ostream& operator<<(std::ostream& out, const meniu& m);
};