#include "atlas/utils/Film.hpp"
#include "atlas/core/Float.hpp"
#include "atlas/core/Log.hpp"

#include <iostream>
#include <fstream>

namespace atlas
{
    namespace utils
    {
        struct Film::FilmImpl
        {
            FilmImpl() :
                hRes(400),
                vRes(400),
                aspectRatio(1.0f),
                gamma(1.0f),
                invGamma(1.0f),
                showOutOfGamut(false)
            {
                frame.resize(hRes * vRes);
            }

            FilmImpl(int h, int v) :
                hRes(h),
                vRes(v),
                aspectRatio(float(h) / v),
                gamma(1.0f),
                invGamma(1.0f),
                showOutOfGamut(false)
            {
                frame.resize(hRes * vRes);
            }


            int hRes;
            int vRes;
            float aspectRatio;
            float gamma;
            float invGamma;
            bool showOutOfGamut;

            FilmFrame frame;
        };

        Film::Film() :
            mImpl(new FilmImpl)
        { }

        Film::Film(int hRes, int vRes) :
            mImpl(new FilmImpl(hRes, vRes))
        { }

        Film::~Film()
        { }

        void Film::setShowOutOfGamut(bool showOutOfGamut)
        {
            mImpl->showOutOfGamut = showOutOfGamut;
        }

        void Film::setGamma(float gamma)
        {
            mImpl->gamma = gamma;
        }

        int Film::getHRes() const
        {
            return mImpl->hRes;
        }

        int Film::getVRes() const
        {
            return mImpl->vRes;
        }

        float Film::getAspectRatio() const
        {
            return mImpl->aspectRatio;
        }

        const FilmFrame& Film::getCurrentFrame() const
        {
            return mImpl->frame;
        }

        void Film::drawPixel(int x, int y, RGBColour const& pixel)
        {
            USING_ATLAS_CORE_NS;

            RGBColour mappedColour = (mImpl->showOutOfGamut) ?
                clampToColour(pixel) :
                maxToOne(pixel);

            if (!areEqual(1.0f, mImpl->gamma))
            {
                mappedColour = powc(mappedColour, mImpl->invGamma);
            }

            int index = x * mImpl->hRes + y;
            mImpl->frame[index] = mappedColour;
        }

        void Film::saveToPPM(std::string const& frameName) const
        {
            typedef char RawPixel;

            std::ofstream fileStream(frameName, std::ios::binary | std::ios::out);
            if (fileStream.is_open())
            {
                fileStream << "P6\n" << mImpl->hRes << " " << mImpl->vRes <<
                    "\n255\n";
                RawPixel* pixel = new RawPixel[3];
                for (auto &pix : mImpl->frame)
                {
                    pixel[0] = RawPixel(pix.r * 255);
                    pixel[1] = RawPixel(pix.g * 255);
                    pixel[2] = RawPixel(pix.b * 255);

                    fileStream.write(pixel, 3);
                }
                fileStream.close();
                delete[] pixel;
            }
            else
            {
                ERROR_LOG("Could not open file " + frameName);
            }
        }

        RGBColour& Film::operator()(int x, int y)
        {
            int index = x * mImpl->hRes + y;
            return mImpl->frame[index];
        }
    }
}