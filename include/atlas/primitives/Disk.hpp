#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_DISK_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_DISK_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        class Disk : public Primitive
        {
        public:
            Disk(float radius = 1.0f, int subDivs = 20);
            Disk(Disk const& d);
            ~Disk();

        private:
            struct DiskImpl;
            std::unique_ptr<DiskImpl> mImpl;
        };
    }
}

#endif