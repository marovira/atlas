#ifndef ATLAS_INCLUDE_ATLAS_FILM_HPP
#define ATLAS_INCLUDE_ATLAS_FILM_HPP

#pragma once

#include "RGBColour.hpp"

#include <vector>
#include <string>
#include <memory>

namespace atlas
{
    typedef std::vector<RGBColour> FilmFrame;

    class Film
    {
    public:
        Film();
        Film(int hRes, int vRes);
        ~Film();

        void setShowOutOfGamut(bool showOutOfGamut);
        void setGamma(float gamma);

        int getHRes() const;
        int getVRes() const;
        float getAspectRatio() const;
        const FilmFrame& getCurrentFrame() const;

        void drawPixel(int x, int y, RGBColour const& pixel);
        void saveToPPM(std::string const& frameName) const;

        RGBColour& operator()(int x, int y);

    private:
        struct FilmImpl;
        std::unique_ptr<FilmImpl> mImpl;
    };
}

#endif