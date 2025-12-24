#pragma once
#include <string>
#include "Stoc.h"
#include "Meniu.h"
#include "Restaurant.h"

class FileManager {
public:
    static void IncarcaDate(stoc& S, meniu& M, restaurant& R);

private:
    static void IncarcaStoc(stoc& S, const std::string& path);
    static void IncarcaMeniu(meniu& M, const std::string& path);
    static void IncarcaRetete(meniu& M, const std::string& path);
    static void IncarcaAngajati(restaurant& R, const std::string& path);
    static void IncarcaMese(restaurant& R, const std::string& path);
};