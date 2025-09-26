#ifndef PAYOFF_PUT_HPP
#define PAYOFF_PUT_HPP

#include "payoff.hpp"

class PayoffPut : public Payoff {
private:
    double strike;

public:
    /**
     * @brief Renvoie la valeur du Payoff
     * 
     * @param strike 
     */
    PayoffPut(double strike) : strike(strike) {}
    virtual double operator()(double spot) const override {
        return std::max(strike - spot, 0.0);
    }
    /**
     * @brief Renvoie le type de Payoff
     * 
     * @return std::string 
     */
    virtual std::string getType() const override {
        return "Put";
    }
};

#endif