#ifndef PAYOFF_HPP
#define PAYOFF_HPP


class Payoff {
public :
    virtual double operator()(double spot) const = 0;
    virtual std::string getType() const = 0;
};

#endif