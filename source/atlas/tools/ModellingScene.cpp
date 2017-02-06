#include "atlas/tools/ModellingScene.hpp"

#include "atlas/gl/GL.hpp"
#include "atlas/utils/GUI.hpp"

namespace atlas
{
    namespace tools
    {
        ModellingScene::ModellingScene() :
            mShowGrid(true),
            mUniformMatrixBuffer(GL_UNIFORM_BUFFER)
        {
            mUniformMatrixBuffer.bindBuffer();
            mUniformMatrixBuffer.bufferData(gl::size<math::Matrix4>(2),
                nullptr, GL_DYNAMIC_DRAW);
            mUniformMatrixBuffer.unBindBuffer();

            mUniformMatrixBuffer.bindBufferRange(0, 0, 
                gl::size<math::Matrix4>(2));
        }

        void ModellingScene::mousePressEvent(int button, int action,
            int modifiers, double xPos, double yPos)
        {
            utils::Gui::getInstance().mousePressed(button, action, modifiers);

            if (action == GLFW_PRESS)
            {
                math::Point2 point(xPos, yPos);

                if (button == GLFW_MOUSE_BUTTON_LEFT &&
                    modifiers == GLFW_MOD_ALT)
                {
                    mCamera.setMovement(MayaMovements::Tumble);
                    mCamera.mouseDown(point);
                }
                else if (button == GLFW_MOUSE_BUTTON_MIDDLE &&
                    modifiers == GLFW_MOD_ALT)
                {
                    mCamera.setMovement(MayaMovements::Track);
                    mCamera.mouseDown(point);
                }
                else if (button == GLFW_MOUSE_BUTTON_RIGHT &&
                    modifiers == GLFW_MOD_ALT)
                {
                    mCamera.setMovement(MayaMovements::Dolly);
                    mCamera.mouseDown(point);
                }
            }
            else
            {
                mCamera.mouseUp();
            }
        }

        void ModellingScene::mouseMoveEvent(double xPos, double yPos)
        {
            utils::Gui::getInstance().mouseMoved(xPos, yPos);
            mCamera.mouseMove(math::Point2(xPos, yPos));
        }

        void ModellingScene::mouseScrollEvent(double xOffset, double yOffset)
        {
            atlas::utils::Gui::getInstance().mouseScroll(xOffset, yOffset);
            mCamera.mouseScroll(atlas::math::Point2(xOffset, yOffset));
        }

        void ModellingScene::keyPressEvent(int key, int scancode, int action,
            int mods)
        {
            utils::Gui::getInstance().keyPress(key, scancode, action, mods);
        }

        void ModellingScene::screenResizeEvent(int width, int height)
        {
            glViewport(0, 0, width, height);

            mProjection = glm::perspective(glm::radians(mCamera.getCameraFOV()),
                (float)width / height, 1.0f, 1000000.0f);
            
            mUniformMatrixBuffer.bindBuffer();
            mUniformMatrixBuffer.bufferSubData(0, sizeof(math::Matrix4),
                &mProjection[0][0]);
            mUniformMatrixBuffer.unBindBuffer();

            utils::Gui::getInstance().screenResize(width, height);

            mWidth = (float)width;
            mHeight = (float)height;
        }

        void ModellingScene::onSceneEnter()
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void ModellingScene::onSceneExit()
        {
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
        }

        void ModellingScene::updateScene(double time)
        {
            mTime.deltaTime = (float)time - mTime.currentTime;
            mTime.currentTime = (float)time;
            mTime.totalTime += (float)time;

            utils::Gui::getInstance().update(mTime);
        }

        void ModellingScene::renderScene()
        {
            utils::Gui::getInstance().newFrame();
            const float grey = 161.0f / 255.0f;
            glClearColor(grey, grey, grey, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mProjection = glm::perspective(
                glm::radians(mCamera.getCameraFOV()),
                (float)mWidth / mHeight, 1.0f, 1000000.0f);

            mUniformMatrixBuffer.bindBuffer();
            mUniformMatrixBuffer.bufferSubData(0, sizeof(atlas::math::Matrix4),
                &mProjection[0][0]);
            mUniformMatrixBuffer.unBindBuffer();

            mView = mCamera.getCameraMatrix();
            mUniformMatrixBuffer.bindBuffer();
            mUniformMatrixBuffer.bufferSubData(sizeof(atlas::math::Matrix4),
                sizeof(atlas::math::Matrix4), &mView[0][0]);

            if (mShowGrid)
            {
                mGrid.renderGeometry(mProjection, mView);
            }

            ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Global HUD");
            ImGui::Checkbox("Show grid", &mShowGrid);
            if (ImGui::Button("Reset Camera"))
            {
                mCamera.resetCamera();
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
                ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
            ImGui::Render();
        }
    }
}