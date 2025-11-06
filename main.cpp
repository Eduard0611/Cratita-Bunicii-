#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>

class ingrediente {
private:
    std::string nume;
    double cantitate;
    double pretAchizitie;
public:
    ingrediente(std::string nume, double cantitate, double pretAchizitie)
        : nume(std::move(nume)), cantitate(cantitate), pretAchizitie(pretAchizitie) {}

    [[nodiscard]]double getPretAchizitie() const { return pretAchizitie; }
    [[nodiscard]]const std::string& getNume() const { return nume; }
    [[nodiscard]]double getCantitate() const { return cantitate; }
    void scadeCantitate(double x) { cantitate -= x; }

    friend std::ostream& operator<<(std::ostream& out, const ingrediente& i) {
        out << i.nume << " : " << i.cantitate << " : " << "Pretul per unitate " << i.pretAchizitie;
        return out;
    }
};

class angajati {
private:
    std::string nume;
    std::string functie;
    double salariu;
public:
    angajati(std::string nume, std::string functie, double salariu)
        : nume(std::move(nume)), functie(std::move(functie)), salariu(salariu) {}
    [[nodiscard]]const std::string& getNume() const { return nume; }
    //[[nodiscard]]const std::string& getFunctie() const { return functie; }
    [[nodiscard]]double getSalariu() const { return salariu; }
    friend std::ostream& operator<<(std::ostream& out, const angajati& i) {
        out << i.nume << " ocupa functia de " << i.functie << " si a fost platit cu " << i.salariu <<"RON pentru aceasta zi."<< std::endl;
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
                if (ing.getCantitate() < cantitate) {
                    std::cout << "Nu exista suficient/i/e " << nume << " in stoc!\n";
                    return false;
                } else {
                    ing.scadeCantitate(cantitate);
                    return true;
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
    produs(std::string nume, double pret) : nume(std::move(nume)), pret(pret) {}

    produs(const produs& other) : nume(other.nume), pret(other.pret), Ingrediente(other.Ingrediente) {}

    produs& operator=(const produs& other) {
        if (this != &other) {
            nume = other.nume;
            pret = other.pret;
            Ingrediente = other.Ingrediente;
        }
        return *this;
    }

    ~produs() {};

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

class restaurant {
private:
    std::string nume;
    std::vector<angajati> Angajati;
    char continuaZi = 'y';
    int comenziFinalizate = 0;
    int comenziRefuzate = 0;
    int stele = 0;
    bool ok = true;
    int cantitateProdus = 1 + rand() % 3;
    double eficienta = 0.0;
    char raspuns = ' ';
public:
    explicit restaurant(std::string nume) : nume(std::move(nume)) {}
    void Angajeaza(const angajati& x) {
        Angajati.push_back(x);
    }
    [[nodiscard]]double getSalariiAngajati() const {
        double salarii = 0.0;
        for (const auto& i : Angajati) {
            salarii += i.getSalariu();
        }
        return salarii;
    }


    friend std::ostream& operator<<(std::ostream& out, const restaurant& s) {
        out << "=========" << s.nume << " =========\n";
        for (const auto& ang : s.Angajati)
            out << ang << "\n";
        return out;
    }
    void ZiRestaurant(meniu& Meniu, stoc& Stoc, double& profitTotal, int ziuaCurenta) {
            continuaZi = 'y';
            ok = true;
            comenziFinalizate = 0;
            comenziRefuzate = 0;
            eficienta = 0.0;
            cantitateProdus = 1 + rand() % 3;

            std::cout << "\n=== Ziua " << ziuaCurenta << " ===\n";

            std::cout << "\n--- Stoc disponibil la inceputul zilei ---\n";
            std::cout << Stoc;

            while (continuaZi == 'y' || continuaZi == 'Y') {
                // Comanda aleatorie
                const auto &produse = Meniu.getProduse();
                if (produse.empty()) {
                    std::cout << "Nu exista produse in meniu!\n";
                    return;
                }
                int indexProdus = static_cast<int>(rand() % produse.size());
                produs p = produse[indexProdus];
                std::cout << "\nClientul doreste " << cantitateProdus << " x " << p.getNume() << "\n";
                std::cout << "Acceptati comanda? (y/n): ";
                std::cin >> raspuns;

                if (raspuns != 'y' && raspuns != 'Y') {
                    std::cout << "Comanda a fost anulata.\n";
                    comenziRefuzate++;
                } else {
                    for (const auto &ing: p.getIngrediente()) {
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

        if (comenziFinalizate + comenziRefuzate > 0)
            eficienta = static_cast<double>(comenziFinalizate) / (comenziFinalizate + comenziRefuzate);

        std::cout << "\nEficienta zilei: " << eficienta * 100 << "%\n";
        std::cout << "\nProfitul zilei: "<< profitTotal<< " RON\n";

        std::cout << "Doriti aprovizionare automata (1) sau aprovizionare manuala (2):";
        std::cin >> raspuns;
        if (raspuns == '1') {

            double costSalarii = getSalariiAngajati();

            if (profitTotal < costSalarii) {
                std::cout << "\nAtentie! Profit insuficient (" << profitTotal << " RON) pentru a acoperi salariile (" << costSalarii << " RON). Aprovizionarea Automata Anulata.\n";
            } else {
                double bugetAprovizionare = profitTotal - costSalarii;
                std::cout << "\n--- Aprovizionare Automata ---\n";
                std::cout << "Rezerva Salarii: " << costSalarii << " RON. Buget Aprovizionare: " << bugetAprovizionare << " RON.\n";

                constexpr double PRAG_MINIM = 5.0;
                std::vector<ingrediente*> ingredienteNecesare;
                double costPentruOUnitateDinFiecare = 0.0;

                for (auto& ing : Stoc.getStoc()) {
                    if (ing.getCantitate() < PRAG_MINIM) {
                    ingredienteNecesare.push_back(&ing);
                    costPentruOUnitateDinFiecare += ing.getPretAchizitie();
                }
            }

            if (ingredienteNecesare.empty() || costPentruOUnitateDinFiecare == 0.0) {
                std::cout << "Nu este necesara aprovizionarea.\n";
            } else {
                double cantitateDeCumparat = std::floor(bugetAprovizionare / costPentruOUnitateDinFiecare);

                double unitatiDoriteMax = 0.0;


                for (const auto &ing: ingredienteNecesare) {
                    constexpr double MAX_UNITATI_DE_ADAUGAT = 15.0;
                    double cantitateDeficit = PRAG_MINIM - ing->getCantitate();
                    if (cantitateDeficit < 0)
                    unitatiDoriteMax = std::max(unitatiDoriteMax, MAX_UNITATI_DE_ADAUGAT - ing->getCantitate());
                }

                cantitateDeCumparat = std::min(cantitateDeCumparat, unitatiDoriteMax);

                if (cantitateDeCumparat >= 1) {
                    double costFinalAprovizionare = 0.0;

                    std::cout << "Se vor cumpara " << cantitateDeCumparat <<
                            " unitati din fiecare ingredient necesar.\n";

                    for (const auto &ing: ingredienteNecesare) {
                        double costCumparat = cantitateDeCumparat * ing->getPretAchizitie();

                        ing->scadeCantitate(-cantitateDeCumparat);
                        costFinalAprovizionare += costCumparat;

                        std::cout << "Aprovizionat " << ing->getNume() << " cu " << cantitateDeCumparat
                                << " unitati. Cost: " << costCumparat << " RON.\n";
                    }

                    profitTotal -= costFinalAprovizionare;
                    std::cout << "Cost total aprovizionare: " << costFinalAprovizionare << " RON.\n";
                } else
                    std::cout <<
                            "Bugetul ramas nu permite cumpararea nici macar a unei unitati din fiecare ingredient necesar.\n";
            }
            }
        }
        else {
            std::cout << "\n--- Reaprovizionare manuala ---\n";
            std::cout << "Tineti cont de salariile angajatilor: " << getSalariiAngajati() << " RON si banii disponibili: " << profitTotal << std::endl;
            for (auto& ing : Stoc.getStoc()) {
                int cantCumparata;
                do {
                    std::cout << "Cate unitati de " << ing.getNume() << " doriti sa cumparati? ";
                    std::cin >> cantCumparata;
                    if (cantCumparata * ing.getPretAchizitie() > profitTotal)
                        std::cout << "Bani insuficienti. Prea multe unitati." << std::endl;
                }while (cantCumparata * ing.getPretAchizitie() > profitTotal);
                ing.scadeCantitate(-cantCumparata);
                profitTotal -= cantCumparata * ing.getPretAchizitie();
            }
        }

        std::cout << "\n";
        for (const auto& i : Angajati) {
            profitTotal -= i.getSalariu();
            std::cout << i << std::endl;
        }

        std::cout << "Profitul final este: " << profitTotal << " RON" << std::endl;
        // Stele
        if (eficienta <= 0.5) stele = 1;
        else if (eficienta <= 0.7) stele = 2;
        else if (eficienta <= 0.9) stele = 3;
        else stele = 5;

        std::cout << "\nRating restaurant pe aceasta zi: " << stele << " Stea(e)\n";

        // Salvam stoc
        Stoc.SalveazaStoc("StocActualizat.txt");
}
};


int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    stoc Stoc;
    std::ifstream fin1("Informatii/Stoc.txt");
    if (!fin1.is_open()) {
        std:: cout << "Eroare: nu s-a putut deschide fisierul Stoc pentru citire!\n";
        return 1;
    }
    std::string cuv; double cant, pretunitar;
    while (fin1 >> cuv >> cant >> pretunitar) Stoc.AdaugaIngredienteInStoc(ingrediente(cuv, cant,pretunitar));
    fin1.close();

    meniu Meniu;
    std::ifstream fin2("Informatii/Meniu.txt");
    if (!fin2.is_open()) {
        std::cout << "Eroare: nu s-a putut deschide fisierul Meniu pentru citire!\n";
        return 1;
    }
    double pret;
    while (fin2 >> cuv >> pret) Meniu.AdaugaProduse(produs(cuv, pret));
    fin2.close();

    std::ifstream fin3("Informatii/Produse.txt");
    if (!fin3.is_open()) {
        std::cout << "Eroare: nu s-a putut deschide fisierul Produse pentru citire!\n";
        return 1;
    }
    std::string numeProdus, numeIng; double cantIng;
    while (fin3 >> numeProdus >> numeIng >> cantIng) {
        if (auto* p = Meniu.CautaProdus(numeProdus))
            p->AdaugaIngredienteInProdus(numeIng, cantIng);
    }
    fin3.close();

    restaurant Restaurant("Cratita Bunicii");
    std::ifstream fin4("Informatii/Angajati.txt");
    if (!fin4.is_open()) {
        std::cout << "Eroare: nu s-a putut deschide fisierul Angajati pentru citire!\n";
        return 1;
    }
    std::string numeAngajat, functie; double salariu;
    while (fin4 >> numeAngajat >> functie >> salariu) {
        Restaurant.Angajeaza(angajati(numeAngajat, functie, salariu));
    }
    fin4.close();

    std::cout << Meniu;

    bool restaurantDeschis = true;
    double profitTotal = 100.0;
    int ziuaCurenta = 1;
    char continua;

    std::cout << "\nDoriti sa deschideti restaurantul si sa primiti clienti? (y/n): ";
    std::cin >> continua;
    if (continua != 'y' && continua != 'Y') {
        restaurantDeschis = false;
    }
    while (restaurantDeschis) {
        Restaurant.ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta);


        std::cout << "\nDoriti sa deschideti restaurantul ziua urmatoare si sa primiti clienti? (y/n): ";
        std::cin >> continua;
        if (continua != 'y' && continua != 'Y') {
            restaurantDeschis = false;
        }
        else ziuaCurenta++;
    }

    std::cout << "\nRestaurantul s-a inchis definitiv.\nProfit total final: " << profitTotal << " RON\n";




    return 0;
}