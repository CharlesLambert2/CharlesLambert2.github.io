#include "matrice.hpp"
#include "matrice_tridiag.hpp"
#include "payoff.hpp"
#include "payoff_call.hpp"
#include "payoff_put.hpp"
#include "affichage.hpp"
#include "C:/Users/charl/Desktop/C++/Installation/SDL2-2.30.9/x86_64-w64-mingw32/include/SDL2/SDL.h"
#include <cmath>


const double T  = 1;
const double r = 0.1;
const double sigma = 0.1;
const double K = 100;
const double L = 300;
const int M = 100;
const int N = 100;
const double dt = T/M;
const double dS = L/N;
const double mu = std::pow(sigma,2)/2;
int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 600;

/**
 * @brief Résoud l'équation de Black Scholes par la méthode de Crank-Nicholson
 * 
 * @param payoff 
 * @return Matrice contenant les valeurs à tracer 
 */
std::vector<double> resolution_B_S(const Payoff& payoff){
    std::vector<double> alpha;
    std::vector<double> beta;
    std::vector<double> gamma;
    for(int i = 0; i<N-1; i++){
        alpha.push_back(dt*(r*(i+1)- std::pow(i+1,2)*std::pow(sigma,2)/2));
    }
    for(int i = 0; i<N-1;i++){
        beta.push_back(dt*(r-r*(i+1)+std::pow(i+1,2)*std::pow(sigma,2)));
        gamma.push_back(dt*std::pow(i+1,2)*std::pow(sigma,2)/2);
    }
    std::vector<double> diagA;
    std::vector<double> surdiagA;
    std::vector<double> sousdiagA;
    std::vector<double> diagB;
    std::vector<double> surdiagB;
    std::vector<double> sousdiagB;
    for(size_t i = 0; i<N-1;i++){
        diagA.push_back(2-beta[i]);
        diagB.push_back(2+beta[i]);
    }
    for(size_t i = 0; i<N-2;i++){
        surdiagA.push_back(gamma[i]);
        surdiagB.push_back(-gamma[i]);
    }
    for(size_t i = 1; i<N-1;i++){
        sousdiagA.push_back(-alpha[i]);
        sousdiagB.push_back(alpha[i]);
    }
    Matrice_trid A(N-1,surdiagA, diagA, sousdiagA);
    Matrice_trid B(N-1,surdiagB, diagB, sousdiagB);
    std::vector <double> X;
    std::vector<double> Y;
    std::vector<double> V;
    if (payoff.getType() == "Call"){
        for(int j = 0; j < M; j++){
            V.push_back(-gamma[N-2]*K*(exp(-r*(j*dt-T)) + exp(-r*((j+1)*dt-T))));
        }
    }
    else if (payoff.getType() == "Put"){
        for(int j = 0; j < M; j++){
            V.push_back(alpha[0]*K*(exp(r*(j*dt-T)) + exp(r*((j+1)*dt-T))));
        }
    }
    for (int i = 0; i < N-1; i++){
            X.push_back(payoff(i*dS));
        }
    std::vector<double> Q(N-1,0);
    for(int j = M-1; j >= 0; j--){
            if(payoff.getType() == "Call"){
                Q[N-2] = V[j];
            }
            else if(payoff.getType() == "Put"){
                Q[0] = V[j];
            }
        Y.clear();
        std::vector<double> W = A%X;
        for(int i = 0; i<N-1;i++){
            Y.push_back(W[i] - Q[i]);
        }
        X = B.solve_syst_tridiag(Y);
    }
    std::vector<double> C(N+1,0);
    if(payoff.getType() == "Call"){
        C[0] = 0;
        C[C.size()-1] = K*exp(r*T);
    }
    else if (payoff.getType() == "Put"){
        C[0] = K*exp(-r*T);
        C[C.size()-1] = 0;
    }
    for(size_t i = 0; i < X.size(); i++){
        C[i+1] = X[i];
    }
    return C;
}

/**
 * @brief Résoud l'équation réduite par la méthode d'Euler implicite
 * 
 * @param payoff 
 * @return Matrice contenant les valeurs à tracer
 */

/* B  = diag std::pow(dS,2), A = tridiag(-dt*mu, std::pow(dS,2) + 2*mu*dt, -dt*mu) */
std::vector<double> resolution_B_S_simple(const Payoff& payoff){
    std::vector<double> diagA;
    std::vector<double> surdiagA;
    std::vector<double> sousdiagA;
    std::vector<double> diagB;
    std::vector<double> surdiagB;
    std::vector<double> sousdiagB;
    for(size_t i = 0; i<N-1;i++){
        diagA.push_back(std::pow(dS,2) + 2*mu*dt);
        diagB.push_back(std::pow(dS,2));
    }
    for(size_t i = 0; i<N-2;i++){
        surdiagA.push_back(-dt*mu);
        sousdiagA.push_back(-dt*mu);
        surdiagB.push_back(0);
        sousdiagB.push_back(0);
    }
    Matrice_trid A(N-1,surdiagA, diagA, sousdiagA);
    Matrice_trid B(N-1,surdiagB, diagB, sousdiagB);
    std::vector <double> X;
    std::vector<double> Y;
    std::vector<double> V;
    if(payoff.getType() == "Call"){
        for(int j = 0; j < M; j++){
            V.push_back(dt*mu*K*exp(-r*((j)*dt-T)));
        }
    }
        else if (payoff.getType() == "Put"){
            for(int j = 0; j < M; j++){
                V.push_back(dt*mu*K*exp(r*((j)*dt-T)));
        }
    }
    for (int i = 0; i < N-1; i++){
        X.push_back(payoff(i*dS));
    }
    std::vector<double> Q(N-1,0);
    for(int j = M-1; j >= 0; j--){
            if(payoff.getType() == "Call"){
                Q[N-2] = V[j];
            }
            else if(payoff.getType() == "Put"){
                Q[0] = V[j];
            }
        Y.clear();
        std::vector<double> W = A%X;
        for(int i = 0; i<N-1;i++){
            Y.push_back(W[i] - Q[i]);
        }
        X = B.solve_syst_tridiag(Y);
    }
    std::vector<double> C(N+1,0);
    if(payoff.getType() == "Call"){
        C[0] = 0;
        C[C.size()-1] = K*exp(r*T);
    }
    else if (payoff.getType() == "Put"){
        C[0] = K*exp(-r*T);
        C[C.size()-1] = 0;
    }
    for(size_t i = 0; i < X.size(); i++){
        C[i+1] = X[i];
    }
    return C;
}

/**
 * @brief Demande à l'utilisateur le payoff souhaité puis appelle la fonction d'affichage
 * 
 * @return int (juste pour retourner quelque chose)
 */
int WinMain(){
    Matrice C(1,N-2);
    Matrice C_s(1,N-2);
    double strike = K;
    int payoffType;
    std::cout << "Choisissez le type de payoff :\n";
    std::cout << "1 - Call\n";
    std::cout << "2 - Put\n";
    std::cin >> payoffType;

    while (payoffType != 1 && payoffType != 2){
        std::cerr << "Erreur : Veuillez entrer 1 (Call) ou 2 (Put)" << std::endl;
        std::cout << "Choisissez le type de payoff :\n";
        std::cout << "1 - Call\n";
        std::cout << "2 - Put\n";
        std::cin >> payoffType;
    }
    if (payoffType == 1){
        PayoffCall payoff(strike);
        std::cout << "Vous avez choisi le Payoff de type Call.\n";
        int h = Sdl::affichage_final(resolution_B_S(payoff), resolution_B_S_simple(payoff), WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    else{
        PayoffPut payoff(strike);
        std::cout << "Vous avez choisi le Payoff de type Put.\n";
        int h = Sdl::affichage_final(resolution_B_S(payoff), resolution_B_S_simple(payoff), WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    return 0;
}


/* To do list : 
- Corriger le bug en fin de call et vérifier si l'equation de la chaleur est correcte
- Faire un graphique plus joli / plus présentable (échelle, titre, légende, etc.) (il faut SDL_ttf.h)
- Regarder la compilation demandé par M.Torri
- Corriger les Warnings
*/
