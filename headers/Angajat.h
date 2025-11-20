#pragma once

#include <string>
#include <iostream>

class angajat {
private:
    std::string nume;
    std::string functie;
    double salariu;

public:

    angajat(std::string nume, std::string functie, double salariu);

    [[nodiscard]] const std::string& getNume() const;
    [[nodiscard]] double getSalariu() const;

    friend std::ostream& operator<<(std::ostream& out, const angajat& i);
};
