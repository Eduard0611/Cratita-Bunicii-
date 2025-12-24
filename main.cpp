#include <iostream>
#include <ctime>
#include "Restaurant.h"
#include "Meniu.h"
#include "Stoc.h"
#include "FileManager.h"
#include "Exceptii.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    try {
        stoc Stoc;
        meniu Meniu;
        restaurant Restaurant("Cratita Bunicii");

        FileManager::IncarcaDate(Stoc, Meniu, Restaurant);

        std::cout << Meniu;

        bool jocActiv = true;
        double profitTotal = 1500.0;
        int ziuaCurenta = 1;

        std::cout << "\nBun venit! Apasati ENTER pentru a incepe simularea restaurantului...";
        std::cin.get();

        while (jocActiv) {
            Restaurant.ZiRestaurant(Meniu, Stoc, profitTotal, ziuaCurenta, jocActiv);

            if (!jocActiv) {
                std::cout << "Jocul a fost inchis manual. La revedere!\n";
                break;
            }

            if (profitTotal < 0) {
                std::cout << "\n!!! FALIMENT !!!\n";
                std::cout << "Ati ramas fara bani. Restaurantul a fost inchis de banca.\n";
                std::cout << "Ati rezistat " << ziuaCurenta << " zile.\n";
                jocActiv = false;
            } else {
                ziuaCurenta++;
            }
        }

    } catch (const std::exception& e) {
        std::cout << "\n\n[FATAL ERROR] Programul s-a oprit neasteptat:\n";
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}