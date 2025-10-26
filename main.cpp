#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Ingredient {
private:
    std::string nume;
    double cantitateDisponibila; // grame

public:
    Ingredient(const std::string& nume, double cantitateDisponibila)
        : nume(nume), cantitateDisponibila(cantitateDisponibila) {}

    const std::string& getNume() const { return nume; }
    double getCantitate() const { return cantitateDisponibila; }

    // Scade stocul când e folosit intr-o comanda
    void scadeCantitate(double cantitate) {
        if (cantitateDisponibila >= cantitate)
            cantitateDisponibila -= cantitate;
        else
            cantitateDisponibila = 0;
    }

    friend std::ostream& operator<<(std::ostream& out, const Ingredient& ing) {
        out << ing.nume << " - " << ing.cantitateDisponibila << "g";
        return out;
    }
};

class Produs {
private:
    std::string nume;
    double pret;
    std::vector<std::pair<std::string, double>> ingredienteNecesare; // (numeIngredient, cantitate)

public:
    Produs(const std::string& nume, double pret)
        : nume(nume), pret(pret) {}

    const std::string& getNume() const { return nume; }
    double getPret() const { return pret; }

    void adaugaIngredient(const std::string& numeIng, double cantitate) {
        ingredienteNecesare.push_back({numeIng, cantitate});
    }

    const std::vector<std::pair<std::string, double>>& getIngrediente() const {
        return ingredienteNecesare;
    }

    friend std::ostream& operator<<(std::ostream& out, const Produs& p) {
        out << "Produs: " << p.nume << " | Pret: " << p.pret << " lei\n   Ingrediente: ";
        for (auto& ing : p.ingredienteNecesare)
            out << ing.first << "(" << ing.second << "g) ";
        return out;
    }
};

class Comanda {
private:
    std::vector<Produs> produse;

public:
    void adaugaProdus(const Produs& p) {
        produse.push_back(p);
    }

    double calculeazaTotal() const {
        double total = 0;
        for (auto& p : produse)
            total += p.getPret();
        return total;
    }

    friend std::ostream& operator<<(std::ostream& out, const Comanda& c) {
        out << "Comanda:\n";
        for (auto& p : c.produse)
            out << "  - " << p.getNume() << " (" << p.getPret() << " lei)\n";
        out << "Total: " << c.calculeazaTotal() << " lei\n";
        return out;
    }
};

class Restaurant {
private:
    std::string nume;
    std::vector<Ingredient> stoc;
    std::vector<Produs> meniu;
    std::vector<Comanda> comenzi;
    double profit;

public:
    Restaurant(const std::string& nume) : nume(nume), profit(0) {}

    // Constructor de copiere
    Restaurant(const Restaurant& other)
        : nume(other.nume), stoc(other.stoc),
          meniu(other.meniu), comenzi(other.comenzi),
          profit(other.profit) {}

    // Operator= de copiere
    Restaurant& operator=(const Restaurant& other) {
        if (this != &other) {
            nume = other.nume;
            stoc = other.stoc;
            meniu = other.meniu;
            comenzi = other.comenzi;
            profit = other.profit;
        }
        return *this;
    }

    // Destructor
    ~Restaurant() {}

    void adaugaIngredient(const Ingredient& ing) { stoc.push_back(ing); }
    void adaugaProdus(const Produs& p) { meniu.push_back(p); }

    void afiseazaStoc() const {
        std::cout << "\n=== Stoc Ingrediente ===\n";
        for (auto& ing : stoc)
            std::cout << ing << "\n";
    }

    void afiseazaMeniu() const {
        std::cout << "\n=== Meniu ===\n";
        for (auto& p : meniu)
            std::cout << p << "\n";
    }

    // Cauta un produs în meniu după nume
    Produs* cautaProdus(const std::string& numeProdus) {
        for (auto& p : meniu)
            if (p.getNume() == numeProdus)
                return &p;
        return nullptr;
    }

    // Cauta un ingredient în stoc după nume
    Ingredient* cautaIngredient(const std::string& numeIng) {
        for (auto& i : stoc)
            if (i.getNume() == numeIng)
                return &i;
        return nullptr;
    }

    // Plaseaza o comanda si actualizeaza stocul si profitul
    void plaseazaComanda(const std::vector<std::string>& produseDorite) {
        Comanda c;
        double totalComanda = 0;

        for (auto& numeProdus : produseDorite) {
            Produs* produs = cautaProdus(numeProdus);
            if (produs != nullptr) {
                bool stocOk = true;
                // Verifica dacă exista ingrediente suficiente
                for (auto& ing : produs->getIngrediente()) {
                    Ingredient* stocIng = cautaIngredient(ing.first);
                    if (stocIng == nullptr || stocIng->getCantitate() < ing.second) {
                        stocOk = false;
                        break;
                    }
                }

                if (stocOk) {
                    // Scade ingredientele din stoc
                    for (auto& ing : produs->getIngrediente()) {
                        Ingredient* stocIng = cautaIngredient(ing.first);
                        stocIng->scadeCantitate(ing.second);
                    }
                    c.adaugaProdus(*produs);
                    totalComanda += produs->getPret();
                    profit += produs->getPret() * 0.3; // 30% profit
                }
            }
        }

        if (totalComanda > 0)
            comenzi.push_back(c);
    }

    void afiseazaComenzi() const {
        std::cout << "\n=== Comenzi ===\n";
        for (auto& c : comenzi)
            std::cout << c << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, const Restaurant& r) {
        out << "=== Restaurant: " << r.nume << " ===\n";
        out << "Profit total: " << r.profit << " lei\n";
        return out;
    }
};

int main() {
    std::ifstream fin("tastatura.txt");
    if (!fin) {
        std::cout << "Eroare la deschiderea fisierului tastatura.txt\n";
        return 1;
    }

    Restaurant r("Cratita Bunicii");

    int nIngrediente;
    fin >> nIngrediente;
    for (int i = 0; i < nIngrediente; i++) {
        std::string nume;
        double cant;
        fin >> nume >> cant;
        r.adaugaIngredient(Ingredient(nume, cant));
    }

    int nProduse;
    fin >> nProduse;
    for (int i = 0; i < nProduse; i++) {
        std::string numeProd;
        double pret;
        fin >> numeProd >> pret;
        Produs p(numeProd, pret);

        int nIng;
        fin >> nIng;
        for (int j = 0; j < nIng; j++) {
            std::string numeIng;
            double cant;
            fin >> numeIng >> cant;
            p.adaugaIngredient(numeIng, cant);
        }
        r.adaugaProdus(p);
    }

int nComenzi;
    fin >> nComenzi;
    for (int i = 0; i < nComenzi; i++) {
        int nProdusecomanda;
        fin >> nProdusecomanda;
        std:: vector <std::string> produse;
        for (int j = 0; j < nProdusecomanda; j++) {
            std::string numeProdus;
            fin >> numeProdus;
            produse.push_back(numeProdus);
        }
        r.plaseazaComanda(produse);
    }

    r.afiseazaMeniu();
    r.afiseazaStoc();
    r.afiseazaComenzi();
    std::cout << r;



    return 0;
}

