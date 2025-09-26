#include "matrice_tridiag.hpp"

Matrice_trid::Matrice_trid(std::size_t size,const std::vector<double> &surdiagonale,const std::vector<double> &diagonale,const std::vector<double> &sousdiagonale) : size(size),surdiagonale(surdiagonale), diagonale(diagonale), sousdiagonale(sousdiagonale){
    if(surdiagonale.size() != size-1 || sousdiagonale.size() != size-1 || diagonale.size() != size){
        throw std::invalid_argument("Taille de la matrices et des diagonales incompatibles \n");
    }
}

Matrice_trid::~Matrice_trid(){}

Matrice_trid::Matrice_trid(const Matrice_trid &autre):size(autre.size),surdiagonale(autre.surdiagonale),diagonale(autre.diagonale), sousdiagonale(autre.sousdiagonale){
}

Matrice_trid& Matrice_trid::operator=(const Matrice_trid& autre) {
    if (this != &autre) { 
        size = autre.size;
        diagonale = autre.diagonale;
        surdiagonale = autre.surdiagonale;
        sousdiagonale = autre.sousdiagonale;   
    }
    return *this; 
}

std::vector<double> Matrice_trid::operator%(const std::vector<double> vect) const{
    std::vector<double> r;
    r.push_back((*this).diagonale[0]*vect[0] + (*this).surdiagonale[0]*vect[1]);
    for(size_t i = 1; i<(*this).size - 1; i++){
        r.push_back((*this).sousdiagonale[i-1]*vect[i-1] + (*this).diagonale[i]*vect[i] + (*this).surdiagonale[i]*vect[i+1]);
    }
    r.push_back((*this).diagonale[(*this).size-1]*vect[size-1] + (*this).sousdiagonale[(*this).size-2]*vect[(*this).size-2]);
    return r;
}


/*

std::vector<double> Matrice_trid::solve_syst_tridiag(const std::vector<double> vect) const{
    if (vect.size() != size)
        throw std::invalid_argument("Taille de la matrice et du vecteur incompatible \n");
    
    Matrice_trid Matrice_modif = Matrice_trid(*this);
    std::vector<double> vect_modif = vect;
    for (size_t i = 1; i < size; i ++){
        
        double m_i = Matrice_modif.sousdiagonale[i] / Matrice_modif.diagonale[i - 1];
        Matrice_modif.diagonale[i] -= m_i * Matrice_modif.surdiagonale[i -1] ;
        vect_modif[i] -= m_i * vect_modif[i - 1];
    }
    
    std::vector<double> result;
    result.clear();
    result[size - 1] = vect_modif[size - 1] / diagonale[ size - 1];
    for (size_t j = size - 2; j >= 0; j --)
        result[j] = (vect_modif[j] - Matrice_modif.surdiagonale[j]*result[j + 1])/Matrice_modif.diagonale[j];
    return result;
}
*/


std::vector<double> Matrice_trid::solve_syst_tridiag(const std::vector<double> &vect) const {
    std::vector<double> res(vect.size(), 0);
    std::vector<double> sur_diag_prime (vect.size() - 1);
    sur_diag_prime[0] = surdiagonale[0]/diagonale[0];
    for(int i = 1; i<sur_diag_prime.size();i++){
        sur_diag_prime[i] = surdiagonale[i]/(diagonale[i] - sousdiagonale[i-1]*sur_diag_prime[i-1]);
    } 
    std::vector<double> vect_prime(vect.size(),0);
    vect_prime[0] = vect[0]/diagonale[0];
    for(int i = 1; i< vect.size(); i++){
        vect_prime[i] = (vect[i] - sousdiagonale[i-1]*vect_prime[i-1])/(diagonale[i] -sousdiagonale[i-1]*sur_diag_prime[i-1]);
    }
    res[res.size()-1] = vect_prime[vect.size()-1];
    for(int i = vect.size() - 2; i>=0;i--){
        res[i] = vect_prime[i] - sur_diag_prime[i]*res[i+1];
    }
    return res;
}
