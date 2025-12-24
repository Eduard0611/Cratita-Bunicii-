#pragma once
#include <string>
#include "Restaurant.h"
#include "Stoc.h"

class restaurant;

class Eveniment {
public:
    virtual ~Eveniment() = default;
    virtual void Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda = 0.0, bool areLocLaMasa = false) = 0;
    virtual std::string getNume() const = 0;
};

class InspectieSanitara : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda = 0.0, bool areLocLaMasa = false) override;
};

class PanaCurent : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda = 0.0, bool areLocLaMasa = false) override;
};

class Influencer : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda = 0.0, bool areLocLaMasa = false) override;
};

class CriticCulinar : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(restaurant& r, stoc& s, double& profitTotal, double costComanda = 0.0, bool areLocLaMasa = false) override;
};