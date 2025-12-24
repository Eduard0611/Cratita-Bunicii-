#pragma once
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <string>

template <typename T>
class Tracker {
private:
    std::vector<T> istoric;

public:
    void adauga(T valoare) {
        istoric.push_back(valoare);
    }

    double getMedie() const {
        if (istoric.empty()) return 0.0;
        T suma = std::accumulate(istoric.begin(), istoric.end(), T(0));
        return static_cast<double>(suma) / istoric.size();
    }

    T getMin() const {
        if (istoric.empty()) return T(0);
        return *std::min_element(istoric.begin(), istoric.end());
    }

    T getMax() const {
        if (istoric.empty()) return T(0);
        return *std::max_element(istoric.begin(), istoric.end());
    }

    void afiseaza(const std::string& numeMetric) const {
        if (istoric.empty()) {
            std::cout << "   " << numeMetric << ": Nu sunt date.\n";
            return;
        }
        std::cout << "--- " << numeMetric << " ---\n";
        std::cout << "   Media: " << getMedie() << "\n";
        std::cout << "   Minim: " << getMin() << " | Maxim: " << getMax() << "\n\n";
    }
};

class Statistica {
private:
    Tracker<double> profit;
    Tracker<int> clienti;
    Tracker<int> refuzuri;
    Tracker<int> stele;

public:
    void adaugaZi(double p, int c, int r, int s) {
        profit.adauga(p);
        clienti.adauga(c);
        refuzuri.adauga(r);
        stele.adauga(s);
    }

    void raportFinal() const {
        std::cout << "\n\n======== RAPORT FINAL DE ACTIVITATE ========\n";
        profit.afiseaza("Profit Zilnic (RON)");
        clienti.afiseaza("Clienti Serviti");
        refuzuri.afiseaza("Clienti Refuzati");
        stele.afiseaza("Rating (Stele)");
        std::cout << "============================================\n";
    }
};