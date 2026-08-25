%{
#include "math/vec3.h"
%}

%nspace math::vec3;

%inline %{
namespace math {

class vec3
{
public:
    vec3(double x, double y, double z)
    {
        this->m_vec.x = x;
        this->m_vec.y = y;
        this->m_vec.z = z;
    }

    ~vec3() {}

    st_vec3_float64_t *get() { return &m_vec; }

private:
    st_vec3_float64_t m_vec;

    vec3(const vec3&) = delete;
    vec3 &operator=(const vec3 &) = delete;
};

}
%}
