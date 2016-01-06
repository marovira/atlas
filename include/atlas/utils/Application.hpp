#ifndef ATLAS_INCLUDE_ATLAS_UTILS_APPLICATION_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_APPLICATION_HPP

#pragma once

#include "Utils.hpp"

#include <string>
#include <memory>

#define APPLICATION atlas::utils::Application::getInstance()

namespace atlas
{
    namespace utils
    {
        class Application
        {
            Application();
            ~Application();

            Application(Application const&) = delete;
            void operator =(Application const&) = delete;

        public:
            static Application& getInstance();

            void createWindow(int width, int height, std::string const& title);
            void runApplication();

            void getCursorPosition(double* x, double *y);

            void addScene(Scene* scene);
            Scene* getCurrentScene() const;

        private:
            struct ApplicationImpl;
            std::unique_ptr<ApplicationImpl> mImpl;
        };
    }
}

#endif