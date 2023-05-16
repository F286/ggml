#pragma once
#include <string>
#include <span>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

enum class StorageOrder {
    RowMajor,
    ColumnMajor
};

template <typename T, StorageOrder Order = StorageOrder::ColumnMajor>
struct MatrixHash {
    static constexpr std::size_t DisplayItemCount = 64; // You can change this value to control the number of items in the hash string

    MatrixHash(std::size_t rows, std::size_t columns, std::span<T> data) {
        std::stringstream ss;

        int index = 0;
        for (std::size_t r = 0; r < rows; ++r) {
            ss << "[ ";
            for (std::size_t c = 0; c < columns; ++c) {
                std::size_t dataIndex = (Order == StorageOrder::RowMajor) ? (r * columns + c) : (c * rows + r);
                ss << std::setw(6) << std::fixed << std::setprecision(3) << static_cast<double>(data[dataIndex]) << ' ';
                if (index++ > DisplayItemCount)
                {
                    break;
                }
            }
            if (index++ > DisplayItemCount)
            {
                break;
            }
            ss << "]";
            if (r != rows - 1) {
                ss << std::endl;
            }
        }

        hash = ss.str();
    }

    MatrixHash(std::size_t rows, std::size_t columns, void* data)
        : MatrixHash(rows, columns, std::span<T>(static_cast<T*>(data), rows* columns))
    {
    }

    template <typename TMatrix>
    explicit MatrixHash(TMatrix matrix)
        : MatrixHash(matrix.rows, matrix.cols, std::span<T>(matrix.Data()))
    {
    }

    bool operator==(std::string str) const {
        return hash == str;
    }

    bool operator!=(std::string str) const {
        return hash != str;
    }

    bool operator==(MatrixHash other) const {
        return hash == other.hash;
    }

    bool operator!=(MatrixHash other) const {
        return hash != other.hash;
    }

    friend std::ostream& operator<<(std::ostream& os, const MatrixHash& matrixHash) {
        os << matrixHash.hash;
        return os;
    }

    std::string hash;
};