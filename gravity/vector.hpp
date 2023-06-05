#pragma once

#include <array>
#include "math.h"

#include "vector_exc.hpp"

// Vector Template
template<class TYPE, unsigned int DIM>
class Vector
{
public:
    // Constructors & Destructors
    Vector() : m_data(std::array<TYPE, DIM>{0}) {};
    Vector(const Vector<TYPE, DIM>& vector)
    {
        std::copy(vector.begin(), vector.end(), begin());
    }

    Vector(TYPE arg1) : m_data({arg1})
    {
    }

    Vector(TYPE arg1, TYPE arg2) : m_data({arg1, arg2})
    {
    }

    Vector(TYPE arg1, TYPE arg2, TYPE arg3) : m_data({arg1, arg2, arg3})
    {
    }

    Vector(TYPE arg1, TYPE arg2, TYPE arg3, TYPE arg4) : m_data({arg1, arg2, arg3, arg4})
    {
    }

    Vector(std::initializer_list<TYPE> data)
    {
        if (data.size() > DIM)
        {
            throw XVectorDimensionIncorrect(DIM, data.size());
        }
        std::copy(data.begin(), data.end(), begin());
    }

    ~Vector() {};

    // Basic Properties
    int Size()
    {
        return m_data.size();
    }

    // Validation
    int IsEqualLength(const Vector& vec)
    {
        return m_data.size() == vec.Size();
    }

    // Random Access Operators
    TYPE& operator[] (unsigned int n)
    {
        return m_data[n];
    }

    const TYPE& operator[] (unsigned int n) const
    {
        return m_data[n];
    }

    // Iterators
    typename std::array<TYPE, DIM>::iterator begin()
    {
        return m_data.begin();
    }

    typename std::array<TYPE, DIM>::const_iterator begin() const
    {
        return m_data.begin();
    }

    typename std::array<TYPE, DIM>::iterator end()
    {
        return m_data.end();
    }

    typename std::array<TYPE, DIM>::const_iterator end() const
    {
        return m_data.end();
    }

    // Arithmetic Operators
    friend Vector<TYPE, DIM> operator+(Vector<TYPE, DIM> lhs, const Vector<TYPE, DIM>& rhs)
    {
        return lhs += rhs;
    }

    Vector<TYPE, DIM>& operator+=(const Vector<TYPE, DIM>& rhs)
    {
        for (unsigned int i = 0; i < DIM; ++i)
        {
            m_data[i] += rhs[i];
        }
        return *this;
    }

    friend Vector<TYPE, DIM> operator-(Vector<TYPE, DIM> lhs, const Vector<TYPE, DIM>& rhs)
    {
        return lhs -= rhs;
    }

    Vector<TYPE, DIM>& operator-=(const Vector<TYPE, DIM>& rhs)
    {
        for (unsigned int i = 0; i < DIM; ++i)
        {
            m_data[i] -= rhs[i];
        }
        return *this;
    }

    // Multiplication with other types
    friend Vector<TYPE, DIM> operator*(Vector lhs, const TYPE& rhs)
    {
        return lhs *= rhs;
    }

    friend Vector<TYPE, DIM> operator*(const TYPE& lhs, Vector rhs)
    {
        return rhs *= lhs;
    }

    Vector<TYPE, DIM>& operator*=(const TYPE& rhs)
    {
        for (unsigned int i = 0; i < DIM; ++i)
        {
            m_data[i] *= rhs;
        }
        return *this;
    }

    // Component wise multiplication
    //friend Vector<TYPE, DIM> operator*(Vector<TYPE, DIM> lhs, const Vector<TYPE, DIM>& rhs)
    //{
    //   return lhs *= rhs;
    //}

    //Vector<TYPE, DIM>& operator*=(const Vector<TYPE, DIM>& rhs)
    //{
    //   for (unsigned int i = 0; i < DIM; ++i)
    //   {
    //      m_data[i] *= rhs[i];
    //   }
    //   return *this;
    //}

    // Component wise division
    //friend Vector<TYPE, DIM> operator/(Vector<TYPE, DIM> lhs, const Vector<TYPE, DIM>& rhs)
    //{
    //   return lhs /= rhs;
    //}

    //Vector<TYPE, DIM>& operator/=(const Vector<TYPE, DIM>& rhs)
    //{
    //   for (unsigned int i = 0; i < DIM; ++i)
    //   {
    //      m_data[i] /= rhs[i];
    //   }
    //   return *this;
    //}

    // TODO: Scalar multiplication

    // Assignment operator
    Vector<TYPE, DIM>& operator=(const Vector<TYPE, DIM>& other)
    {
        for (unsigned int i = 0; i < DIM; ++i)
        {
            m_data[i] = other[i];
        }
        return *this;
    }

    // Comparison operators
    // TODO: Implement ==
    // TODO: Implement !=

    // Mathematical Validation functions
    // TODO: Implement Parallel check - opposite direction but not necessarily the same magnitude
    // TODO: Implement AntiParallel check - same direction but not necessarily the same magnitude

    // Opposite check - same magnitude but opposite direction
    bool IsOpposite(Vector<TYPE, DIM> vector)
    {
        bool validateMagnitude = Norm() == vector.Norm();
        bool validateDirection = m_data[0] == -vector[0] && m_data[1] == -vector[1];
        return validateMagnitude && validateDirection;
    }

    // Opposite
    Vector<TYPE, DIM> Opposite()
    {
        return Vector<TYPE, DIM>({-m_data[0], -m_data[1]});
    }

    // Mathematical vector operations
    TYPE NormSquared()
    {
        // The euclidean norm squared - the sum of the squares of all the vector elements
        TYPE result = static_cast<TYPE>(0);
        for (auto& data : m_data)
        {
            result += data*data;
        }
        return result;
    }

    TYPE Norm()
    {
        // Also known as length or magnitude
        // The euclidean norm - defined as the square root of sum of the squares of all the vector elements
        // Physically this is the length of the vector
        return std::sqrt(NormSquared());
    }

    void Normalise()
    {
        auto magnitude = Norm();
        for (auto i=0; i<DIM; ++i)
        {
            m_data[i] /= magnitude;
        }
    }

    // Dot/Scalar Product a . b
    friend TYPE DotProduct(const Vector<TYPE, DIM>& lhs, const Vector<TYPE, DIM>& rhs)
    {
        if (!lhs.IsEqualLength(rhs))
        {
            throw XVectorDimensionMismatch(lhs.Size(), rhs.Size());
        }

        TYPE result = static_cast<TYPE>(0);
        for (int i = 0; i < lhs.Size(); ++i)
        {
            result += lhs[i] * rhs[i];
        }
        return result;
    }

    // Angle between two vectors (using dot product) - radians
    friend TYPE AngleBetween(const Vector<TYPE, DIM>& lhs, const Vector<TYPE, DIM>& rhs)
    {
        // Validates size in DotProduct
        return acos(DotProduct(lhs, rhs) / (lhs.Norm() * rhs.Norm()));
    }

    // TODO: Cross/Vector Product (2&3 dims only)
    // TODO: Scalar triple product - a dot(b x c) (2&3 dims only)

private:
    std::array<TYPE, DIM> m_data;

};

typedef Vector<double, 2> Vector2;
typedef Vector<double, 3> Vector3;
typedef Vector<double, 4> Vector4;
