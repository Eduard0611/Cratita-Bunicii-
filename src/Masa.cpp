#include "Masa.h"

masa::masa(int id, int capacitate) : id(id), capacitate(capacitate), ocupata(false), timpRamas(0) {}

int masa::getId() const {
    return id;
}

int masa::getCapacitate() const { return capacitate; }

bool masa::isOcupata() const { return ocupata; }

int masa::getTimpRamas() const { return timpRamas; }

void masa::setOcupata(bool status, int timp) {
    ocupata = status;
    timpRamas = (status ? timp : 0);
}

void masa::scadeTimp() {
    if (ocupata && timpRamas > 0) {
        timpRamas--;
    }
}

std::ostream& operator<<(std::ostream& out, const masa& m) {
    out << "Masa " << m.id << " | Capacitate: " << m.capacitate << " | Status: " << (m.ocupata ? "OCUPATA" : "LIBERA");
    if(m.ocupata) {
        out << " (Se elibereaza in " << m.timpRamas << " comenzi)";
    }
    return out;
}