/**
 *	\file RandomGenerator.hpp
 *	\brief Defines a simple, templated random number generator.
 */

#ifndef ATLAS_INCLUDE_ATLAS_MATH_RANDOM_GENERATOR_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_RANDOM_GENERATOR_HPP

#include "Math.hpp"

#include <random>
#include <limits>

namespace atlas
{
    namespace math
    {
        /**
         *	\class RandomGenerator
         *	\brief Defines a simple random number generator.
         *	
         *	This class wraps around the new STD <tt> \<random\> </tt>
         *	functionality by providing a simple way of getting a random
         *	number generator running quickly. This is based on the 
         *	mt19937 generator, which should be sufficient for most
         *	applications. The class is templated to allow switching
         *	between floats and doubles.
         *	
         *	@tparam GenType The precision for the generator.
         */
        template <typename GenType>
        class RandomGenerator
        {
        public:
            /**
             *	Standard constructor initializes everything to default.
             */
            RandomGenerator() = default;

            /**
             *	Constructor with a seed initialization.
             *	
             *	@param[in] seed The seed for the random number generation.
             */
            RandomGenerator(std::mt19937::result_type seed) :
                mEngine(seed)
            { }

            /**
             *	Returns a random real number \f$x\f$ in the range \f$ 
             *	[min, max] \f$. 
             *	
             *	@param[in] min The minimum range for the random real.
             *	@param[in] max The maximum range for the random real.
             *	@return A real number between min and max.
             */
            inline GenType getRandomReal(GenType min, GenType max)
            {
                std::uniform_real_distribution<GenType> dist(min, max);
                return dist(mEngine);
            }

            /**
             *	Returns a random real number \f$x\f$ in the range 
             *	\f$ [ 0, \infty] \f$ where \f$ \infty\f$ is understood as
             *	the maximum value that the specified type can hold.
             *	
             *	@return A real number bewtween 0 and infinity.
             */
            inline GenType getRandomRealMax()
            {
                return getRandomReal(GenType(0.0), 
                    std::numeric_limits<GenType>::max());
            }

            /**
             *	Returns a random real number \f$x\f$ between 0 and 1.
             *	
             *	@return A real number between 0 and 1.
             */
            inline GenType getRandomRealOne()
            {
                return getRandomReal(GenType(0.0), GenType(1.0));
            }

            /**
             *	Returns a random integer in the range \f$ [min, max]\f$.
             *	
             *	@param[in] min The minimum range for the random integer.
             *	@param[in] max The maximum range for the random integer.
             *	@return A 
             */
            inline int getRandomInt(int min, int max)
            {
                std::uniform_int_distribution<> dist(min, max);
                return dist(mEngine);
            }

            /**
             *	Returns a random integer in the range \f$ [0, \infty] \f$
             *	where \f$ \infty \f$ is understood as the maximum value an
             *	integer can hold.
             *	
             *	@return A random integer in the range 0 to infinity.
             */
            inline int getRandomInt()
            {
                return getRandomInt(0, std::numeric_limits<int>::max());
            }

        private:
            std::mt19937 mEngine{ std::random_device{} () };
        };
    }
}

#endif
