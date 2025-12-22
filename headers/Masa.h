#pragma once
#include <iostream>

class masa {
private:
    int id;
    int capacitate;
    bool ocupata;
    int timpRamas; // Numarul de comenzi pana la eliberare

public:
    masa(int id, int capacitate);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getCapacitate() const;
    [[nodiscard]] bool isOcupata() const;
    [[nodiscard]] int getTimpRamas() const;

    // Default 0 pentru compatibilitate
    void setOcupata(bool status, int timp = 0);
    void scadeTimp();

    friend std::ostream& operator<<(std::ostream& out, const masa& m);
};