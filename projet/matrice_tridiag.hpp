#ifndef MATRICE_TRIDIAG_HPP
#define MATRICE_TRIDIAG_HPP
#include <iostream>
#include <vector>
#include <stdexcept>

class Matrice_trid{
    private:
    std::size_t size;
    std::vector<double> surdiagonale;
    std::vector<double> diagonale;
    std::vector<double> sousdiagonale;
    public:
    Matrice_trid(std::size_t size, const std::vector<double> &surdiagonale, const std::vector<double> &diagonale, const std::vector<double> &sousdiagonale);
    Matrice_trid(const Matrice_trid &autre);
    ~Matrice_trid();
    Matrice_trid& operator=(const Matrice_trid &autre);
    std::vector<double> operator%(const std::vector<double> vect)const;
    std::vector<double> solve_syst_tridiag(const std::vector<double> &vect) const;
    void afficher() const;

};

#endif