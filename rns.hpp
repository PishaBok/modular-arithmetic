#pragma once

#include <QtCore/qfutureinterface.h>
#include <iostream>
#include <set>
#include <vector>
#include <math.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random.hpp>
#include <QThreadPool>
#include "my_task.hpp"

namespace mp = boost::multiprecision;

class RnsInt
{
public:
    RnsInt(mp::cpp_int number);

    static void initialize(const int degree, const int modCount);
    mp::cpp_int getNumber();
private:
    // Params
    static mp::cpp_int maxNumber_;
    static int modCount_;
    static std::vector<unsigned long long int> mods_;


    std::vector<unsigned long long int> residues_;

    static std::vector<unsigned long long int> moduliSet(mp::cpp_int x, int& n);
    bool initNumber(mp::cpp_int number);

    friend RnsInt operator+(const RnsInt& x, const RnsInt& y);
    friend RnsInt operator*(const RnsInt& x, const RnsInt& y);
};