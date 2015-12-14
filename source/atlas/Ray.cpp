#include "atlas/Ray.hpp"

namespace atlas
{
    Ray::Ray() :
        o(0.0f),
        d(0.0f, 1.0f, 0.0f)
    { }

    Ray::Ray(atlas::Point const& origin, atlas::Vector const& dir) :
        o(origin),
        d(dir)
    { }

    Ray::~Ray()
    { }

    atlas::Point Ray::operator()(float t) const
    {
        return o + (t * d);
    }

    bool Ray::operator==(Ray const& rhs)
    {
        return (o == rhs.o) && (d == rhs.d);
    }

    bool Ray::operator!=(Ray const& rhs)
    {
        return ((*this) != rhs);
    }
}