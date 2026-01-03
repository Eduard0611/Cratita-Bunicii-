#pragma once
#include <string>
#include "Restaurant.h"
#include "Stoc.h"

class restaurant;

struct ContextEveniment {
    restaurant& r;
    stoc& s;
    double& profitTotal;
    double costComanda;
    bool areLocLaMasa;
};

class Eveniment {
public:
    virtual ~Eveniment() = default;
    virtual void Executa(ContextEveniment& ctx) = 0;
    virtual std::string getNume() const = 0;
};

class InspectieSanitara : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(ContextEveniment& ctx) override;
};

class PanaCurent : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(ContextEveniment& ctx) override;
};

class Influencer : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(ContextEveniment& ctx) override;
};

class CriticCulinar : public Eveniment {
public:
    std::string getNume() const override;
    void Executa(ContextEveniment& ctx) override;
};