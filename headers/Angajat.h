#pragma once
#include <string>
#include <iostream>

class angajat {
protected:
    std::string nume;
    std::string functie;
    double salariu;

public:
    angajat(std::string nume, std::string functie, double salariu);
    virtual ~angajat() = default;

    const std::string& getNume() const;
    double getSalariu() const;
    std::string getFunctie() const;

    virtual angajat* clone() const;
    virtual void afisare(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const angajat& i);
};