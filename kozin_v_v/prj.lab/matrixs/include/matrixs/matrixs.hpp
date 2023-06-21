//
// Created by kozin on 15.03.2023.
//

#ifndef MATRIXS_H
#define MATRIXS_H

#include <stdexcept>
#include <cstddef>
#include <vector>
#include <utility>
#include <tuple>

class MatrixS {
public:
    using SizeType = std::tuple<std::ptrdiff_t, std::ptrdiff_t>;

public:
    explicit MatrixS(const SizeType& size = {0, 0});

    MatrixS(const std::ptrdiff_t m, const std::ptrdiff_t n);

    ~MatrixS();

    MatrixS(const MatrixS &other);

    MatrixS &operator=(const MatrixS &other);

    [[nodiscard]] int &at(const SizeType &elem);

    [[nodiscard]] const int &at(const SizeType &elem) const;

    [[nodiscard]] int &at(const std::ptrdiff_t i, const std::ptrdiff_t j);

    [[nodiscard]] const int &at(const std::ptrdiff_t i, const std::ptrdiff_t j) const;

    void resize(const SizeType &new_size);

    void resize(const std::ptrdiff_t i, const std::ptrdiff_t j);

    [[nodiscard]] const SizeType &ssize() const noexcept;

    /**
     * \return количество строк в матрице (m)
     */
    [[nodiscard]] std::ptrdiff_t nRows() const noexcept;

    /**
     * \return количество столбцов в матрице (n)
     */
    [[nodiscard]] std::ptrdiff_t nCols() const noexcept;

private:
    std::ptrdiff_t m_{};
    std::ptrdiff_t n_{};
    std::vector<int> data_{};
};

#endif
