#pragma once
#include <vector>
#include <iostream>
#include <string>

template <typename T, int Limita>
class DepozitSigur {
private:
    std::vector<T> obiecte;
public:
    void adauga(T element) {
        if (obiecte.size() < Limita) {
            obiecte.push_back(element);
        } else {
            std::cout << "[SISTEM] Depozit plin!\n";
        }
    }

    size_t getNrElemente() const { return obiecte.size(); }
};

template <typename T>
bool esteMaiMare(T a, T b) {
    return a > b;
}