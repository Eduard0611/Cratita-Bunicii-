#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

class ingrediente {
private:
    std::string nume;
    double cantitate;
public:
    ingrediente(const std::string& nume, double cantitate) : nume(nume), cantitate(cantitate) {}

    [[nodiscard]]const std::string& getNume() const { return nume; }
    [[nodiscard]]double getCantitate() const { return cantitate; }
    void scadeCantitate(double x) { cantitate -= x; }

    friend std::ostream& operator<<(std::ostream& out, const ingrediente& i) {
        out << i.nume << " : " << i.cantitate;
        return out;
    }
};

class stoc {
private:
    std::vector<ingrediente> Stocul_Restaurantului;
public:
    void AdaugaIngredienteInStoc(const ingrediente& ingredient) {
        Stocul_Restaurantului.push_back(ingredient);
    }

    std::vector<ingrediente>& getStoc() { return Stocul_Restaurantului; }

    bool Consuma(const std::string& nume, double cantitate) {
        for (auto& ing : Stocul_Restaurantului) {
            if (ing.getNume() == nume) {
                if (ing.getCantitate() >= cantitate) {
                    ing.scadeCantitate(cantitate);
                    return true;
                } else {
                    std::cout << "Nu exista suficient " << nume << " in stoc!\n";
                    return false;
                }
            }
        }
        std::cout << "Ingredientul " << nume << " nu exista in stoc!\n";
        return false;
    }


    void SalveazaStoc(const std::string& numeFisier) const {
        std::ofstream fout(numeFisier);
        if (!fout.is_open()) {
            std::cout << "Eroare la salvarea stocului!\n";
            return;
        }
        for (const auto& ing : Stocul_Restaurantului)
            fout << ing.getNume() << " " << ing.getCantitate() << "\n";
        fout.close();
        std::cout << "Stocul actualizat a fost salvat in fisierul " << numeFisier << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, const stoc& s) {
        out << "+++++++STOCUL RESTAURANTULUI+++++++\n";
        for (const auto& ing : s.Stocul_Restaurantului)
            out << ing << "\n";
        return out;
    }
};

struct IngNecesar {
    std::string nume;
    double cantitate;
};

class produs {
private:
    std::string nume;
    double pret;
    std::vector<IngNecesar> Ingrediente;
public:
    produs(const std::string& nume, double pret) : nume(nume), pret(pret) {}

    produs(const produs& other) : nume(other.nume), pret(other.pret), Ingrediente(other.Ingrediente) {}

    produs& operator=(const produs& other) {
        if (this != &other) {
            nume = other.nume;
            pret = other.pret;
            Ingrediente = other.Ingrediente;
        }
        return *this;
    }

    ~produs() {}

    void AdaugaIngredienteInProdus(const std::string& numeIng, double cantitate) {
        Ingrediente.push_back({numeIng, cantitate});
    }

    [[nodiscard]]double getPret() const { return pret; }
    [[nodiscard]]const std::string& getNume() const { return nume; }
    std::vector<IngNecesar>& getIngrediente() { return Ingrediente; }

    friend std::ostream& operator<<(std::ostream& out, const produs& p) {
        out << p.nume << " | Pret: " << p.pret << " RON\nIngrediente:\n";
        for (const auto& ing : p.Ingrediente)
            out << "  - " << ing.nume << " : " << ing.cantitate << "\n";
        return out;
    }
};

class meniu {
private:
    std::vector<produs> produse;
public:
    void AdaugaProduse(const produs& p) { produse.push_back(p); }

    std::vector<produs>& getProduse() { return produse; }

    produs* CautaProdus(const std::string& nume) {
        for (auto& p : produse)
            if (p.getNume() == nume) return &p;
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& out, const meniu& m) {
        out << "===========MENIU===========" << std::endl;
        for (const auto& p : m.produse)
            out << p << "\n";
        return out;
    }
};

void ZiRestaurant(meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta) {
    int comenziFinalizate = 0;
    int comenziRefuzate = 0;

    std::cout << "\n=== Ziua " << ziuaCurenta << " ===\n";

    char continuaZi = 'y';
    while (continuaZi == 'y' || continuaZi == 'Y') {
        // Comanda aleatorie
        const auto& produse = Meniu.getProduse();
        if (produse.empty()) {
            std::cout << "Nu exista produse in meniu!\n";
            return;
        }
        int indexProdus = rand() % produse.size();
        produs p = produse[indexProdus];
        int cantitateProdus = 1 + rand() % 3;

        std::cout << "\nClientul doreste " << cantitateProdus << " x " << p.getNume() << "\n";
        char raspuns;
        std::cout << "Acceptati comanda? (y/n): ";
        std::cin >> raspuns;

        if (raspuns != 'y' && raspuns != 'Y') {
            std::cout << "Comanda a fost anulata.\n";
            comenziRefuzate++;
        } else {
            bool ok = true;
            for (const auto& ing : p.getIngrediente()) {
                if (!Stoc.Consuma(ing.nume, ing.cantitate * cantitateProdus)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                double profitComanda = p.getPret() * cantitateProdus;
                profitTotal += profitComanda;
                comenziFinalizate++;
                std::cout << "Comanda realizata! Profit: " << profitComanda << " RON\n";
            } else {
                std::cout << "Comanda nu poate fi realizata complet!\n";
                comenziRefuzate++;
            }
        }

        std::cout << "\n--- Stoc actual ---\n" << Stoc;

        std::cout << "\nDoriti o alta comanda azi? (y/n): ";
        std::cin >> continuaZi;
    }

    // Calcul eficienta si rating
    double eficienta = 0.0;
    if (comenziFinalizate + comenziRefuzate > 0)
        eficienta = static_cast<double>(comenziFinalizate) / (comenziFinalizate + comenziRefuzate);

    std::cout << "\nEficienta zilei: " << eficienta * 100 << "%\n";
    std::cout << "\nProfitul zilei:" << profitTotal<< "%\n";

    std::cout << "\n--- Reaprovizionare manuala ---\n";
    for (auto& ing : Stoc.getStoc()) {
        int cantCumparata;
        do {
            std::cout << "Cate unitati de " << ing.getNume() << " doriti sa cumparati? ";
            std::cin >> cantCumparata;
            if (cantCumparata * 2.0 > profitTotal)
                std::cout << "Bani insuficienti. Prea multe unitati." << std::endl;
        }while (cantCumparata * 2.0 > profitTotal);
        ing.scadeCantitate(-cantCumparata);
        profitTotal -= cantCumparata * 2.0;
    }

    // Stele
    int stele = 0;
    if (eficienta <= 0.5) stele = 1;
    else if (eficienta <= 0.7) stele = 2;
    else if (eficienta <= 0.9) stele = 3;
    else stele = 5;

    std::cout << "\nRating restaurant: " << stele << " Stea(e)\n";

    // Salvam stoc
    Stoc.SalveazaStoc("StocActualizat.txt");
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    stoc Stoc;
    std::ifstream fin1("Stoc.txt");
    std::string cuv; double cant;
    while (fin1 >> cuv >> cant) Stoc.AdaugaIngredienteInStoc(ingrediente(cuv, cant));
    fin1.close();

    meniu Meniu;
    std::ifstream fin2("Meniu.txt");
    double pret;
    while (fin2 >> cuv >> pret) Meniu.AdaugaProduse(produs(cuv, pret));
    fin2.close();

    std::ifstream fin3("Produse.txt");
    std::string numeProdus, numeIng; double cantIng;
    while (fin3 >> numeProdus >> numeIng >> cantIng) {
        if (auto* p = Meniu.CautaProdus(numeProdus))
            p->AdaugaIngredienteInProdus(numeIng, cantIng);
    }
    fin3.close();

    std::cout << Meniu << Stoc;

    bool restaurantDeschis = true;
    double profitTotal = 0.0;
    int ziuaCurenta = 1;

    while (restaurantDeschis) {
        ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta);

        char continua;
        std::cout << "\nDoriti sa inceapa ziua urmatoare? (y/n): ";
        std::cin >> continua;
        if (continua != 'y' && continua != 'Y') restaurantDeschis = false;
        else ziuaCurenta++;
    }

    std::cout << "\nRestaurantul s-a inchis definitiv.\nProfit total final: " << profitTotal << " RON\n";

    return 0;
}