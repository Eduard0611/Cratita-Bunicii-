#pragma once
#include <exception>
#include <string>
#include <utility>

class EroareRestaurant : public std::exception {
protected:
    std::string mesaj;
public:
    explicit EroareRestaurant(std::string msg) : mesaj(std::move(msg)) {}
    
    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};


class EroareFisierCritica : public EroareRestaurant {
public:
    explicit EroareFisierCritica(const std::string& numeFisier) 
        : EroareRestaurant("Eroare: Nu s-a putut deschide fisierul: " + numeFisier) {}
};


class EroareStocInsuficient : public EroareRestaurant {
public:
    explicit EroareStocInsuficient(const std::string& ingredient) 
        : EroareRestaurant("Stoc insuficient pentru ingredientul: " + ingredient) {}
};


class EroareComandaMasa : public EroareRestaurant {
public:
    explicit EroareComandaMasa(const std::string& detalii) 
        : EroareRestaurant("Eroare asignare masa: " + detalii) {}
};