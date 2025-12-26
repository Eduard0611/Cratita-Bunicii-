#pragma once
#include <string>
#include <iostream>

class angajat {
protected:
    std::string nume;
    double salariu;

public:
    angajat(std::string nume, double salariu);
    virtual ~angajat() = default;

    const std::string& getNume() const;
    double getSalariu() const;

    virtual std::string getFunctie() const = 0;
    virtual void afisare(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const angajat& i);
};

class bucatar : public angajat {
public:
    bucatar(std::string nume, double salariu);
    std::string getFunctie() const override;
};

class ospatar : public angajat {
public:
    ospatar(std::string nume, double salariu);
    std::string getFunctie() const override;
};