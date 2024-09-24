#include <QRunnable>
#include <QPromise>
#include <QtCore/qrunnable.h>
#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;

class MyTask : public QRunnable {
public:
    MyTask(const mp::cpp_int* param1, const mp::cpp_int* param2, const mp::cpp_int* mod, QPromise<mp::cpp_int> promise)
        : param1_(param1), param2_(param2), mod_(mod), promise_(std::move(promise)) {}

    void run() override {
        mp::cpp_int result = (*param1_ * *param2_) % *mod_; // Например, результат вычисления
        promise_.addResult(result); // Возвращаем результат
        promise_.finish(); // Завершаем выполнение задачи
    }

private:
    const mp::cpp_int* param1_;
    const mp::cpp_int* param2_;
    const mp::cpp_int* mod_;
    QPromise<mp::cpp_int> promise_;
};