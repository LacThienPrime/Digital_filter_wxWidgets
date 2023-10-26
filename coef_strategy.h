#pragma once

#include <vector>
#include <tuple>

class CoefStrategy
{
public:
    virtual ~CoefStrategy() = default;
    virtual std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq) = 0;
};

class Context {
private:
    CoefStrategy* strategy;

public:
    Context(CoefStrategy* strategy);
    std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq);
};

<<<<<<< HEAD
class lowpass : public CoefStrategy
=======
class lowpass : public CoefStrategy 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
public:
    std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

<<<<<<< HEAD
class highpass : public CoefStrategy
=======
class highpass : public CoefStrategy 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
public:
    std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

class bandpass : public CoefStrategy
{
public:
    std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

<<<<<<< HEAD
class bandstop : public CoefStrategy
=======
class bandstop : public CoefStrategy 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
public:
    std::tuple<std::vector<double>, std::vector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};