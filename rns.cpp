#include "rns.hpp"
#include <QtCore/qpromise.h>
#include <algorithm>
#include <thread>
#include <future>
#include <QList>
#include <QFuture>
#include <vector>




mp::cpp_int RnsInt::maxNumber_{};
int RnsInt::modCount_{};
std::vector<unsigned long long int> RnsInt::mods_{};
 

void RnsInt::initialize(const int degree, const int n)
{
    maxNumber_ = static_cast<mp::cpp_int>(mp::pow(mp::cpp_dec_float_100{2}, degree));
    modCount_ = n;
    mods_ = moduliSet(maxNumber_, modCount_);

    // mp::cpp_int check{1};
    // for (int i{0}; i < RnsInt::modCount_; ++i)
    // {
    //     std::cout << "Mod #" << i + 1 << ": " << boost::lexical_cast<std::string>(RnsInt::mods_[i]).size() << "\n";
    //     check *= RnsInt::mods_[i];
    // }
    // std::cout << "M: " << boost::lexical_cast<std::string>(RnsInt::maxNumber_).size() << "\n";
    // std::cout << "Check: " << boost::lexical_cast<std::string>(check).size() << "\n";
}

mp::cpp_dec_float_100 nthRoot(const mp::cpp_dec_float_100 x, const int n) 
{
    // Используем метод Ньютона для вычисления корня
    mp::cpp_dec_float_100 guess = x / n;  // начальное приближение
    mp::cpp_dec_float_100 eps = 1;
    for (int i = 0, maxIter = boost::lexical_cast<std::string>(static_cast<mp::cpp_int>(x)).size() * 20; i < maxIter; ++i) {
        mp::cpp_dec_float_100 guessNext = (1.0 / n) * ((n - 1) * guess + x / mp::pow(guess, n - 1));
        if (mp::abs(guessNext - guess) < eps) {
            return guessNext;
        }
        guess = guessNext;
    }
    return guess;
}

unsigned long long generateNDigitNumber(int numDigits) {
    // if (numDigits <= 0 || numDigits > 19) {
    //     throw std::invalid_argument("Количество знаков должно быть от 1 до 19");
    // }
    
    unsigned long long lowerBound = static_cast<unsigned long long int>(std::pow(10, numDigits - 1));
    unsigned long long upperBound = static_cast<unsigned long long int>(std::pow(10, numDigits)) - 1;
    
    std::random_device rd;  // Инициализация случайного генератора
    std::mt19937_64 gen(rd()); // Генератор Mersenne Twister 64-бит
    std::uniform_int_distribution<unsigned long long> dis(lowerBound, upperBound);
    
    return dis(gen);
}

unsigned long long int rootBySigns(const mp::cpp_int x, const int n)
{
    return generateNDigitNumber(20);
}

bool isPrime(const mp::cpp_int n, const unsigned trials = 25)
{
    boost::random::independent_bits_engine<boost::random::mt19937, 512, mp::cpp_int> gen;
    return boost::multiprecision::miller_rabin_test(n, trials, gen);
}

std::set<unsigned long long int> locateHigherModuli(const int n, const unsigned long long int nrRoot)
{
    std::set<unsigned long long int> res{};
    unsigned long long int flag{nrRoot};

    for (int i{0}; i < n; ++i)
    {
        unsigned long long int modul{flag + 1};

        while (!isPrime(modul))
        {
            ++modul;
            //std::cout << "High: " << modul << "\n";
        }
        res.insert(modul);
        flag = modul;
    }

    return res;
}

std::set<unsigned long long int> locateLowerModuli(const int n, const unsigned long long int nrRoot)
{
    std::set<unsigned long long int> res{};
    unsigned long long int flag{nrRoot};

    for (int i{0}; i < n; ++i)
    {
        unsigned long long int modul{flag - 1};

        while (!isPrime(modul))
        {
            if (modul == 1) {break;}
            --modul;
            //std::cout << "Low: " << modul << "\n";
        }
        res.insert(modul);
        flag = modul;
    }

    return res;
}

std::vector<unsigned long long int> RnsInt::moduliSet(mp::cpp_int x, int& n)
{
    std::vector<unsigned long long int> res{};

    int iterCount{0};
    mp::cpp_int check{};
    do 
    {
        if (iterCount == 100) {++n; iterCount = 0;}

        //std::cout << "Mod Count: " << n << "\n";
        //mp::cpp_int nrRoot{mp::round(nthRoot(static_cast<mp::cpp_dec_float_100>(x), n))};
        unsigned long long int nrRoot{rootBySigns(x, n)};
        unsigned long long int temp{nrRoot};
        while (!isPrime(nrRoot))
        {
            ++nrRoot;
            //std::cout << "Root: " << nrRoot << "\n";
        }

        res.clear();

        std::shared_future<std::set<unsigned long long int>> lowMods = std::async(std::launch::async, locateLowerModuli, floor(n / 2), temp);
        std::shared_future<std::set<unsigned long long int>> highMods = std::async(std::launch::async, locateHigherModuli, ceil(n / 2), nrRoot);
        
        res.insert(res.begin(), lowMods.get().begin(), lowMods.get().end());
        res.insert(res.end(), highMods.get().begin(), highMods.get().end());

        check = 1;
        for (auto& mod: res)
        {
            check *= mod;
        }
        //std::cout << "M: " << boost::lexical_cast<std::string>(RnsInt::maxNumber_).size() << "\n" << "Check: " << boost::lexical_cast<std::string>(check).size() << "\n";
        nrRoot *= 10;
        ++iterCount;
    } while (RnsInt::maxNumber_ >= check);
    

    return res;
}

// Функция для вычисления НОД (расширенный алгоритм Евклида)
mp::cpp_int gcdExtended(const mp::cpp_int& a, const mp::cpp_int& b, mp::cpp_int &x, mp::cpp_int &y) 
{
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    mp::cpp_int x1, y1;
    mp::cpp_int gcd = gcdExtended(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

// Функция для вычисления обратного элемента по модулю
mp::cpp_int modInverse(const mp::cpp_int& a, const mp::cpp_int& m) 
{
    mp::cpp_int x, y;
    mp::cpp_int g = gcdExtended(a, m, x, y);
    if (g != 1) 
    {
        return -1;
    }
    return (x % m + m) % m;
}

RnsInt::RnsInt(mp::cpp_int number) : residues_{}
{
    initNumber(number);
}

bool RnsInt::initNumber(mp::cpp_int number)
{
    if (number > maxNumber_) {return false;}

    for (const auto& mod: mods_)
    {
        residues_.push_back((number % mod).convert_to<unsigned long long int>());
    }

    return true;
}

RnsInt operator+(const RnsInt& x, const RnsInt& y)
{
    RnsInt res{0};

    for (int i{0}; i < RnsInt::modCount_; ++i)
    {
        res.residues_[i] = (x.residues_[i] + y.residues_[i]) % RnsInt::mods_[i];
    }

    return res;
}

RnsInt operator*(const RnsInt& x, const RnsInt& y)
{
    RnsInt res{0};

    for (int i{0}; i < RnsInt::modCount_; ++i)
    {
        res.residues_[i] = (x.residues_[i] * y.residues_[i]) % RnsInt::mods_[i];
    }

    return res;
}

mp::cpp_int RnsInt::getNumber()
{
    mp::cpp_int res{};

    for (int i{0}; i < RnsInt::modCount_; ++i)
    {
        mp::cpp_int modI = RnsInt::maxNumber_ / RnsInt::mods_[i];
        res += (residues_[i] * modI * modInverse(modI, RnsInt::mods_[i]));
    }
    res = res % RnsInt::maxNumber_;

    return res;
}