#pragma once
#include <string>
#include "Stoc.h"
#include "Meniu.h"
#include "Restaurant.h"

class FileManager {
private:
    FileManager() = default;
    static FileManager* instanta;
public:
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    static FileManager* getInstanta() {
        if (instanta == nullptr) {
            instanta = new FileManager();
        }
        return instanta;
    }

    void IncarcaDate(stoc& S, meniu& M, restaurant& R);

private:
    void IncarcaStoc(stoc& S, const std::string& path);
    void IncarcaMeniu(meniu& M, const std::string& path);
    void IncarcaRetete(meniu& M, const std::string& path);
    void IncarcaAngajati(restaurant& R, const std::string& path);
    void IncarcaMese(restaurant& R, const std::string& path);
};