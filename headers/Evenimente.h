#pragma once
#include <string>
#include <vector>

class restaurant;
class stoc;

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

enum class TipEveniment { INSPECTIE, PANA, INFLUENCER, CRITIC };

class FactoryEvenimente {
public:
    static Eveniment* creeaza(TipEveniment tip) {
        if (tip == TipEveniment::INSPECTIE) return new InspectieSanitara();
        if (tip == TipEveniment::PANA) return new PanaCurent();
        if (tip == TipEveniment::INFLUENCER) return new Influencer();
        if (tip == TipEveniment::CRITIC) return new CriticCulinar();
        return nullptr;
    }
};