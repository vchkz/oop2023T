#ifndef ARRAYD_H
#define ARRAYD_H

#include <cstddef>
#include <iostream>
#include <algorithm>


class ArrayD {
public:

    explicit ArrayD(const std::ptrdiff_t size = 0);
    ~ArrayD();

    ArrayD(const ArrayD&);
    ArrayD& operator=(const ArrayD&);
    [[nodiscard]] double& operator[](const std::ptrdiff_t i);
    [[nodiscard]] const double& operator[](const std::ptrdiff_t i) const;
    [[nodiscard]] std::ptrdiff_t ssize() const noexcept;
    void resize(const std::ptrdiff_t new_size);
    void insert(const std::ptrdiff_t i, const double value);
    void remove(const std::ptrdiff_t i);

private:
    std::ptrdiff_t size_{0};
    std::ptrdiff_t capacity_{0};
    double* data_{nullptr};
};

#endif