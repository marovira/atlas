/**
 *	\file Shader.hpp
 *	\brief Defines a wrapper for shader operations.
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
         *	\brief Offers an encapsulation for all shader operations.
         *	
         *	This class allows the user to simply specify the shader types
         *	and source files to be created and the class handles the rest.
         *	Unlike other examples, the compilation and linkage of the shaders
         *	was separated to help isolate errors. While this introduces a small
         *	overhead with an extra function call, it is deemed worth it when
         *	trying to isolate where the shader errors are coming from.
         *	
         *	Aside from encapsulating the shader creation and linking, this 
         *	class also handles the shader program, which is created when the
         *	shaders are compiled.
         *	
         *	Once all the shaders are compiled and linked, it is possible to
         *	individually delete shaders if they are not needed anymore, or 
         *	delete all of them at once. Upon destruction, this class will
         *	automatically delete all the shaders, along with the shader
         *	program.
         *	
         *	This class also provides wrappers for typical shader operations, 
         *	such as accessing uniforms, attributes, or binding attributes.
         */
        class Shader
        {
        public:
            /**
             *	Standard constructor.
             */
            Shader();

            /**
             * Constructs a shader with the specified shader files. Notice
             * that this does not compile the shaders, it simply stores the
             * information so it can be used later on.
             * 
             * \param[in] shaders The list containing the information for all
             * the shaders.
             */
            Shader(std::vector<ShaderUnit> const& shaderUnits);

            Shader(Shader&& rhs);

            Shader& operator=(Shader&& rhs);

            /**
             *	Destructor. Notice that this will destroy not just the shaders,
             *	but the shader program as well.
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

            void setShaderIncludeDir(std::string const& dir);

            /**
             * Returns the list of shaders so they can be modified. This allows
             * on-the-fly addition or removal of shaders, as well as changing
             * source files if required. If any changes are made to this list,
             * then it should be followed by a call to reloadShaders() so the
             * shaders can be updated.
             */
            std::vector<ShaderUnit>& getShaders() const;

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
             *	\return Tre if the linking was successful, false otherwise.
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

            void hotReloadShaders();

            /**
             *	Wraps around the glBindAttribLocation function. If the
             *	shader program hasn't been created, or it is invalid, the
             *	function writes an error to the Log and returns.
             *	
             *	\warning
             *	This function does not check the OpenGL error queue for errors
             *	after attempting to bind the attribute location.
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
             *	Equivalent to glUseProgram(0). This deactivates the current
             *	shader program.
             */
            void disableShaders() const;

            /**
             *	Returns the current shader program.
             *	\return The shader program.
             */
            GLint getShaderProgram() const;

            bool shaderProgramValid() const;

            /**
             *	Gets the specified uniform variable. If the shader program is
             *	invalid or if the uniform location doesn't exist, the function
             *	returns -1 and outputs the error to the Log.
             *	
             *	\param[in] name The name of the uniform variable.
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
