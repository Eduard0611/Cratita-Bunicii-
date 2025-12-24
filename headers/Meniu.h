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

    meniu& operator=(meniu other);

    friend void swap(meniu& first, meniu& second) noexcept;

    void AdaugaProdus(produs* p);

    [[nodiscard]]const std::vector<produs*>& getProduse() const;
    produs* CautaProdus(const std::string& nume);

    friend std::ostream& operator<<(std::ostream& out, const meniu& m);
};