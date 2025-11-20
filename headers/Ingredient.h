#pragma once 
#include <string>
#include <iostream>

class ingredient {
private:
    std::string nume;
    double cantitate;
    double pretAchizitie;

public:
    ingredient(std::string nume, double cantitate, double pretAchizitie);


    [[nodiscard]] double getPretAchizitie() const;
    [[nodiscard]] const std::string& getNume() const;
    [[nodiscard]] double getCantitate() const;

    void scadeCantitate(double x);

    friend std::ostream& operator<<(std::ostream& out, const ingredient& i);
};