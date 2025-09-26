#ifndef PAYOFF_CALL_HPP
#define PAYOFF_CALL_HPP

#include "payoff.hpp"

class PayoffCall : public Payoff {
private:
    double strike;

public:
    /**
     * @brief Renvoie la veleur du Payoff
     * 
     * @param strike 
     */
    PayoffCall(double strike) : strike(strike) {}
    virtual double operator()(double spot) const override {
        return std::max(spot - strike, 0.0);
    }
    /**
     * @brief Renvoie le type de Payoff
     * 
     * @return std::string 
     */
    virtual std::string getType() const override {
        return "Call";
    }
};

#endif