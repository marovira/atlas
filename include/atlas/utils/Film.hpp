#ifndef ATLAS_INCLUDE_ATLAS_UTILS_FILM_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_FILM_HPP

#pragma once

#include "Utils.hpp"
#include "RGBColour.hpp"

#include <vector>
#include <string>
#include <memory>

namespace atlas
{
    namespace utils
    {
        typedef std::vector<RGBColour> FilmFrame;

        /**
         *	\class Film
         *	\brief Defines a simple array of pixels.
         *	
         *	The Film class is used basically whenever individual pixels need
         *	to be written at any one time (such as ray tracing). It also 
         *	provides a way of writing the file to ppm.
         *	
         *	The way to display a Film on the screen is to essentially bind
         *	the array to a texture and then have the texture drawn on top of
         *	a quad. The image can then be updated and sent to the GPU as
         *	required. 
         *	
         *	\note
         *	The method described above introduces a substantial amount of 
         *	overhead, since the pixels would have to be computed individually
         *	and then sent to the GPU each time the image needs to be updated.
         *	This is <b> not </b> recommended for applications that want to
         *	use OpenGL directly, and is better suited for ray tracing.
         */
        class Film
        {
        public:
            /**
             *	Standard constructor.
             */
            Film();

            /**
             *	Standard constructor that defines the dimensions of the film.
             *	
             *	\param[in] hRes The horizontal size of the film.
             *	\param[in] vRes The vertical size of the film.
             */
            Film(int hRes, int vRes);

            /**
             *	Standard empty destructor.
             */
            ~Film();

            /**
             * Tells the Film whether colour values that are outside of the
             * gamut should be displayed or not. This should generally be
             * set to false, unless there is a good reason to see colours that
             * fall outside of the gamut (like debugging for example).
             * 
             * \param[in] showOutOfGamut Whether to show out of gamut colours
             * or not.
             */
            void setShowOutOfGamut(bool showOutOfGamut);

            /**
             *	Set the gamma value for all the pixels in the image.
             *	
             *	\param[in] gamma The gamma value for all pixels.
             */
            void setGamma(float gamma);

            /**
             *	Returns the horizontal resolution of the image.
             *	
             *	\return The horizontal dimension of the image.
             */
            int getHRes() const;

            /**
             *	Returns the vertical resolution of the image.
             *	
             *	\return The vertical dimension of the image.
             */
            int getVRes() const;

            /**
             *	Returns the aspect ratio of the image. This is computed
             *	by the following equation: \f$ h / v \f$ where \f$ h \f$ and
             *	\f$ v \f$ are the horizontal and vertical dimensions
             *	respectively.
             *	
             *	\return The aspect ratio of the image.
             */
            float getAspectRatio() const;

            /**
             *	Returns the current frame. Use this when binding to a texture.
             *	\warning
             *	This function returns a const reference for a reason. Do not
             *	edit the frame outside of the Film class! Please use the 
             *	provided drawPixel function or <tt>()</tt> operator instead.
             *	
             *	\return Returns the current frame.
             */
            const FilmFrame& getCurrentFrame() const;

            /**
             *	Sets the pixel at the given coordinates to the given 
             *	colour. Note that this will perform clamping if 
             *	showOutOfGamut is set to false, as well as set gamma if it is
             *	set to anything other than 1.
             *	
             *	\param[in] x The x coordinate of the pixel.
             *	\param[in] y The y coordinate of the pixel.
             *	\param[in] pixel The colour of the pixel.
             */
            void drawPixel(int x, int y, RGBColour const& pixel);

            /**
             *	Saves the current frame to ppm with the provided filename.
             *	
             *	\param[in] frameName The name of the file to which the image
             *	will be saved.
             */
            void saveToPPM(std::string const& frameName) const;

            /**
             *	An alternative way of editing specific pixels. This allows
             *	the user to directly acces and manipulate individual pixels
             *	based on their position. 
             *	\note
             *	This operator performs no clamping or gamma computations if
             *	the values are modified.
             *	
             *	\param[in] x The x coordinate of the pixel.
             *	\param[in] y The y coordinate of the pixel.
             */
            RGBColour& operator()(int x, int y);

        private:
            struct FilmImpl;
            std::unique_ptr<FilmImpl> mImpl;
        };
    }
}

#endif