#include <string>
#include <exception>

class XVectorDimensionMismatch : public std::exception
{
private:
    std::string m_msg;

public:
    XVectorDimensionMismatch(const int dim1, const int dim2)
            : m_msg(std::string("Dimension mismatch cannot perform operation on vectors of different sizes: (1)")
                    + std::to_string(dim1) + std::string(" (2)")
                    + std::to_string(dim2))
    {
    }

    virtual const char* what() const throw()
    {
        return m_msg.c_str();
    }
};

class XVectorDimensionIncorrect : public std::exception
{
private:
    std::string m_msg;

public:
    XVectorDimensionIncorrect(const int dim, const int expectedMaxDim)
            : m_msg(std::string("Expected dimension of size less than ")
                    + std::to_string(expectedMaxDim) + std::string(" but got size ")
                    + std::to_string(dim))
    {
    }

    virtual const char* what() const throw()
    {
        return m_msg.c_str();
    }
};