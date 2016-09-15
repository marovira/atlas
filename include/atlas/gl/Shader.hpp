/**
 *	\file Shader.hpp
 *	\brief Defines a wrapper for shader programs.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_SHADER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_SHADER_HPP

#pragma once

#include "GL.hpp"
#include "ShaderUnit.hpp"

#include <vector>
#include <string>
#include <memory>

namespace atlas
{
    namespace gl
    {
        /**
         *	\class Shader
         *	\brief Offers an encapsulation for shader programs.
         *	
         *	This class is designed to both simplify and extend the interactions
         *	with GLSL. The class extends GLSL by adding the include directive
         *	to shader code, allowing the sharing of files between shaders (and
         *	even C++ code). It also allows live reloading of shaders whenever
         *	they are edited, allowing for a faster development cycle.
         *	
         *	In addition, it also provides wrappers for typical shader 
         *	operations, such as accessing uniforms, attributes, or 
         *	binding attributes.
         */
        class Shader
        {
        public:
            /**
             *	Standard constructor.
             */
            Shader();

            /**
             * Sets the files that will be used later for compilation/linking
             * and for execution. This function does not initialize or create
             * any OpenGL objects. It merey stores the supplied list for later
             * use.
             * 
             * \param[in] shaderUnits The list containing the information for
             * all the shaders.
             */
            Shader(std::vector<ShaderUnit> const& shaderUnits);

            /**
             * Move constructor. In order to prevent shaders being accidentally
             * deleted by copying, only move semantics are offered. It is
             * worth noting that the shader that is left behind after the move
             * has a null program as well as no unit data.
             * 
             * \param[in] rhs The shader to move.
             */
            Shader(Shader&& rhs);

            /**
             * Move assignment operator. 
             * 
             * \param[in] rhs The shader to move.
             * 
             * \return The moved shader.
             */
            Shader& operator=(Shader&& rhs);

            /**
             * The destructor does two things: first it detaches and deletes
             * all of the shader units, and then proceeds to delete the
             * shader program itself.
             */
            ~Shader();

            /**
             * Sets the shader list that will be used to compile/link the 
             * shaders into the shader program.
             * 
             * \param[in] shaders The list containing the information for all
             * the shaders.
             */
            void setShaders(std::vector<ShaderUnit> const& shaders);

            /**
             * This is used to specify where the preprocessing system will look
             * for include files. 
             * 
             * \warning
             * Currently, relative paths of the form 
             * \verbatim
             #include "../foo.glsl"
             * \endverbatim
             * are not supported. All that the preprocessor does when an 
             * include is encountered is to paste the provided path in front
             * of the path for the included file. This does imply that this
             * \verbatim
             #include "include/foo/foo.glsl"
             * \endverbatim
             * is supported.
             */
            void setShaderIncludeDir(std::string const& dir);

            /**
             * This function does two things: first it creates the 
             * shader program (if it hasn't already) and then it compiles
             * the shaders from source, checking each one for errors. If an
             * error is encountered, that particular shader is destroyed and 
             * the process continues.
             * 
             * The supplied index can be used to specify a particular shader
             * to be re-compiled (or compiled as the case may be). If no
             * arguments are given, it will compile all the shaders currently
             * stored (passed by constructor or setShaders).
             * 
             * As part of the compilation process, this function will expand 
             * any include directives that are used in the program. If an error
             * occurs, the output will be printed to the log.
             * 
             * It is worth noting that if a shader fails to compile, the
             * program will not be destroyed and set to 0. It is important
             * to check whether or not the compilation failed to avoid
             * unnecessary error messages.
             * 
             * \param[in] idx The index of the shader on the list to compile. 
             * If none is provided or -1, then all shaders are compiled.
             * 
             * \return True if the compilation was successful, false otherwise.
             */
            bool compileShaders(int idx = -1);

            /**
             *	As explained before, the linkage of shaders was separated to
             *	help in isolating errors. As a result, after calling
             *	compileShaders, it is necessary to follow that with a call
             *	to linkShaders. After this, the shaders are fully linked and
             *	ready to be used. If an error occurs, everything is destroyed
             *	and an error is written to the Log.
             *	
             *	\return True if the linking was successful, false otherwise.
             */
            bool linkShaders();

            /**
             * Deletes the shader specified by the argument or all
             * the shaders if -1 (or no argument) is provided. This is 
             * also called by the destructor.
             */
            void deleteShaders(int idx = -1);

            /**
             * Combines calls to compileShaders and linkShaders into a single
             * function. This can be used whenever the shaders have been 
             * changed in some way. 
             * 
             * As with compileShaders, the index allows the compilation of a 
             * specific shader, while no argument (or -1) compiles everything.
             * 
             * \param[in] idx The index of the shader on the list to compile.
             * If none or -1 is provided, then all shaders are compiled.
             * 
             * \return True if the compilation/linking was successful, false
             * otherwise.
             */
            bool reloadShaders(int idx = -1);

            /**
             * This will check the timestamp of the shaders. If a shader file
             * (or any dependencies of that file) has been modified, then the
             * shader will be recompiled and re-linked.
             * 
             * \return True if the shaders were reloaded, false otherwise.
             * 
             * \warning
             * The timestamps are retrieved using the stat function. If the
             * timestamps cannot be retrieved, then this function will
             * be disabled and will do nothing.
             */
            bool hotReloadShaders();

            /**
             *	Wraps around the glBindAttribLocation function. If the
             *	shader program hasn't been created, or it is invalid, the
             *	function writes an error to the Log and returns.
             *	
             *	\param[in] location The attribute location.
             *	\param[in] name The name of the attribute to bind.
             */
            void bindAttribute(GLuint location, std::string const& name) const;

            /**
             *	Wraps around the glUseProgram function call. If the
             *	shader program hasn't been created, or it is invalid, the
             *	function writes an error to the log and returns.
             */
            void enableShaders() const;

            /**
             *	This deactivates the current shader program.
             */
            void disableShaders() const;

            /**
             *	Returns the current shader program.
             *	\return The shader program.
             */
            GLint getShaderProgram() const;

            /**
             * Checks to see if the current shader program is valid. This 
             * can be invoked each frame to ensure that rendering will
             * actually happen and prevent unnecessary errors.
             * 
             * \return True if the current program is valid, false otherwise.
             */
            bool shaderProgramValid() const;

            /**
             *	Gets the specified uniform variable. If the shader program is
             *	invalid or if the uniform location doesn't exist, the function
             *	returns -1 and outputs the error to the Log.
             *	
             *	\param[in] name The name of the uniform variable.
             *	
             *	\return The uniform location if it exists, -1 otherwise.
             */
            GLint getUniformVariable(std::string const& name) const;

            /**
             *	Gets the specified attribute variable. If the shade program
             *	is invalid or if the uniform location doesn't exist, the
             *	function returns -1 and outputs the error to the Log.
             *	
             *	\param[in] name The name of the attribute variable.
             *	\return The attribute location if it exists, -1 otherwise.
             */
            GLint getAttributeVariable(std::string const& name) const;

            /**
             * Retrieves the index of a specified uniform block. If the
             * shader program is invalid or if the block doesn't exist, the
             * function returns -1 and outputs the error to the Log.
             * 
             * \param[in] name The name of the uniform block.
             * \return The uniform block index if it exists, -1 otherwise.
             */
            GLuint getUniformBlockIndex(std::string const& name) const;

            /**
             * Assign an active point to a uniform block. This function
             * has an included error check.
             * 
             * \param[in] uniformBlockIndex The index of the active uniform
             * block whose binding to assign.
             * \param[in] uniformBlockBinding The binding point to which the
             * block index will be bound.
             */
            void uniformBlockBinding(GLuint uniformBlockIndex,
                GLuint uniformBlockBinding) const;

        private:
            struct ShaderImpl;
            std::unique_ptr<ShaderImpl> mImpl;
        };
    }
}

#endif
