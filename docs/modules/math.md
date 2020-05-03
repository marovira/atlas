# Math Module

The math module contains several utility functions and classes for common
operations. The module relies on [GLM](https://github.com/g-truc/glm) for all of
the basic math types such as vectors, matrices, and all related functions. The
module is split across 5 headers, which we will cover next.

## Coordinates (`coordinates.hpp`)

Contains a series of functions to convert to and from several coordinate spaces.
The functions all follow the same naming convention: `<space-1>_to_<space-2>`.
The currently implemented conversions are:

* Cartesian to Spherical: `cartesian_to_spherical`,
* Spherical to Cartesian: `spherical_to_cartesian`,
* Cartesian to Cylindrical: `cartesian_to_cylindrical`,
* Cylindrical to Cartesian: `cylindrical_to_cartesian`,
* Cartesian ($\mathbb{R}^2$) to Polar: `cartesian_to_polar`,
* Polar to Cartesian ($\mathbb{R}^2$): `polar_to_cartesian`.

The functions all rely on the types defined by the main `math.hpp` header. To
use them, do the following:

```c++
#include <atlas/math/coordinates.hpp>

atlas::math::Point pt;
auto pt_spherical = atlas::math::cartesian_to_spherical(pt);

auto pt_cartesian = atlas::math::spherical_to_cartesian(pt_spherical);
```

## Math (`math.hpp`)

This is the main math header that includes the following headers from GLM:

* `glm/glm.hpp`,
* `glm/gtc/constants.hpp`
* `glm/gtc/matrix_inverse.hpp`
* `glm/gtc/matrix_transform.hpp`
* `glm/gtc/quaternion.hpp`
* `glm/gtc/type_ptr.hpp`
* `glm/gtx/norm.hpp`
* `glm/gtx/projection.hpp`

These headers provide the functionality required by most graphics applications.
In addition to this, it also defines the following GLM macros:

* `GLM_FORCE_RADIANS`,
* `GLM_FORCE_SWIZZLE`,
* `GLM_ENABLE_EXPERIMENTAL`.

For more information regarding the exact contents of these headers and the
behaviour of the macros, please see the corresponding documentation from GLM. In
addition, the header also provides aliases for most common types used in
graphics applications. By default, the types `Vector`, `Point` and `Normal` are
all in $\mathbb{R}^3$.

* `Vector2`, `Vector`, `Vector4`
* `Point2`, `Point`, `Point4`
* `Normal2`, `Normal`, `Normal4`
* `Matrix2` ($2 \times 2$ matrix), `Matrix3` ($3 \times 3$ matrix), `Matrix4`
  ($4 \times 4$ matrix)
* `Quaternion`

All headers within the math module rely on these types unless otherwise
indicated.

## Random (`random.hpp`)

Defines a class for a simple random number generator. It is by default based on
`std::mt19937`, which should provide relatively good random numbers for most
applications. That being said, the class is fully templated to allow the user to
change the random number generator and the type of number it creates. The only
requirement is that the type must satisfy `std::is_arithmetic`. The class
provides 3 functions to retrieve a random number:

* `get_random_in_range`: given two numbers $a, b$, this function will return a
  random number in the range $[a, b]$.
* `get_random_zero_to_max`: returns a random number in the range $[0, \infty]$.
* `get_random_zero_to_one`: returns a random number in the range $[0, 1]$.

To use the class, do the following:

```c++
#include <atlas/math/random.hpp>

atlas::math::Random<float> engine;
float r1 = engine.get_random_in_range(1.0f, 2.0f);
```

## Ray (`ray.hpp`)

Defines a class representing a ray. This can be used for ray tracing, ray
casting, or any other type of application that relies on a ray structure. The
class is templated to allow for any dimension for the origin and direction of
the ray. The class overloads the evaluation operator `operator()` as a way of
evaluating the following equation:

$$
r(t) = o + t \cdot \mathbf{d}
$$

Where $o$ is the origin point of the ray, $t$ is a scalar (`float`) and
$\mathbf{d}$ is the direction of the ray. The class also provides equality
operators and can be used as follows:

```c++
#include <atlas/math/ray.hpp>
#include <atlas/math/math.hpp>              // Included here for the Vector alias.

atlas::math::Ray<atlas::math::Vector> r1;   // o and d default to 0.

atlas::math::Vector o{0, 1, 0};
atlas::math::Vector d{0, 0, -1};
atlas::math::Ray<atlas::math::Vector> r2{o, d};

float t = r2(1.0f);                         // Evaluate the ray equation at t = 1.0f
```

## Solvers (`solvers.hpp`)

Provides functions to solve 3 types of polynomial equations: quadratic, cubic,
and quartic. For all equations, they must be in the following form:

$$
\begin{align*}
    Ax^2 + Bx + C &= 0 \\
    Ax^3 + Bx^2 + Cx + D &= 0 \\
    Ax^4 + Bx^3 + Cx^2 + Dx + E &= 0
\end{align*}
$$

The functions all have the same signature, and take two parameters:

* An `std::vector` with the coefficients *in reverse order*
* An `std::vector` where the roots will be placed. Note that if the vector is
  empty it will automatically be resized to the correct number of roots.

The functions will return a number with the number of roots that were found.

> **Note:** The quartic solver is currently not very precise due to rounding
> errors and as a result may return roots that are incorrect. Other
> implementations are being explored.

The functions are all templated to allow for any precision, though they will
only accept floating point types. To use them, do the following:

```c++
#include <atlas/math/solvers.hpp>

// Solve 16x^2 + 24x + 12 = 0
std::vector<float> coeffs{12.0f, 24.0f, 16.0f};
std::vector<float> roots;
std::size_t num_roots = atlas::math::solve_quadratic(coeffs, roots);
```
