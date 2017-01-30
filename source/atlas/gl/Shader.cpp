#include "atlas/gl/Shader.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/core/Exception.hpp"
#include "atlas/gl/ErrorCheck.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <set>

#if defined(ATLAS_PLATFORM_WINDOWS)
#define stat _stat
#define stat_t struct _stat
#else
#define stat_t struct stat
#endif


namespace atlas
{
    namespace gl
    {
        struct Shader::ShaderImpl
        {
            ShaderImpl() :
                shaderProgram(0),
                isHotReloadAvailable(true)
            { }

            ShaderImpl(ShaderImpl const& impl) = default;
            ~ShaderImpl() = default;

            time_t getFileTimeStamp(std::string const& filename)
            {
                // Check to see if hot reloading is available. If it isn't, 
                // then don't bother with getting the timestamp.
                if (!isHotReloadAvailable)
                {
                    return time_t();
                }

                stat_t buffer;
                auto result = stat(filename.c_str(), &buffer);
                if (result != 0)
                {
                    // We couldn't obtain the information for the file, so 
                    // disable hot reloading.
                    isHotReloadAvailable = false;
                    WARN_LOG("Could not obtain file timestamp. Hot reloading" +
                        std::string(" will be disabled"));
                    return time_t();
                }

                return buffer.st_mtime;
            }

            std::string readShaderSource(std::string const& filename, ShaderUnit& unit)
            {
                std::ifstream inFile(filename);
                std::stringstream outString;

                if (!inFile)
                {
                    throw core::RuntimeException("Could not open file: " +
                        filename);
                }

                // Check to see if this is the first time that we are adding
                // something. If it is, then we add the root file (this is to 
                // help us build a hierarchy of includes).
                if (unit.includedFiles.empty())
                {
                    auto timestamp = getFileTimeStamp(filename);
                    unit.includedFiles.push_back({ filename, -1 , timestamp });
                }

                int fileNum = (int)unit.includedFiles.size() - 1;
                int lineNum = 1;
                std::string line;
                while (std::getline(inFile, line))
                {
                    // Check to see if the line is a #version. If it is, then 
                    // we must skip it while increasing the line counter.
                    if (line.find("#version") != std::string::npos)
                    {
                        outString << line + "\n";
                        lineNum++;
                        continue;
                    }

                    // Check to see if the line is an #include
                    if (line.find("#include") != std::string::npos)
                    {
                        // We have an include, so extract the path of the 
                        // included file.
                        const std::string includeStr = "#include ";
                        std::size_t pathSize = 
                            line.size() - includeStr.size() - 2;
                        std::string path = 
                            line.substr(includeStr.size() + 1, pathSize);

                        std::string absolutePath = includeDir + path;

                        // Add the file to the list of includes.
                        auto timestamp = getFileTimeStamp(absolutePath);
                        ShaderFile f = { absolutePath, fileNum, timestamp };
                        unit.includedFiles.push_back(f);

                        // Now recurse on the included file.
                        auto parsedFile = readShaderSource(absolutePath, unit);

                        // Now insert it.
                        outString << parsedFile;
                        continue;
                    }

                    // Create the #line directive.
                    std::string lineInfo = "#line " + std::to_string(lineNum) +
                        " " + std::to_string(fileNum) + "\n";
                    outString << lineInfo;
                    outString << line + "\n";
                    lineNum++;
                }

                return outString.str();
            }

            std::string parseErrorLog(ShaderUnit const& unit, 
                std::string const& log)
            {
                // We first check if the log is empty. If it is, return.
                if (log.empty())
                {
                    return std::string();
                }

                std::stringstream logStream(log);
                std::stringstream outLog;
                std::string line;

                while (std::getline(logStream, line, '\n'))
                {
                    std::smatch match;
                    std::regex pattern("\\d+\\(\\d*\\)");

                    // We are only interested in the first match.
                    std::regex_search(line, match, pattern);

                    if (match.size() != 1)
                    {
                        // We cannot do any parsing on this, so we just return 
                        // the string as is and emit a message to the log.
                        WARN_LOG("The current implementation of OpenGL has a" +
                            std::string(" different syntax for GLSL error ") +
                            std::string("logs. The log is printed as is"));

                        return log;
                    }

                    std::string info = match[0];
                    std::string error = match.suffix();
                    error = error.substr(1, error.size() - 1);

                    // Now extract the numbers by locating ().
                    std::size_t start = info.find("(");
                    std::size_t end = info.find(")");
                    std::size_t delta = end - start;

                    // Grab the numbers.
                    int fileNum = std::stoi(info.substr(0, start));
                    int lineNum = std::stoi(info.substr(start + 1, delta));

                    // Now assemble the include hierarchy for the file.
                    int parent = unit.includedFiles[fileNum].parent;
                    std::vector<int> hierarchy;
                    hierarchy.push_back(fileNum);
                    while (parent != -1)
                    {
                        hierarchy.push_back(parent);
                        parent = unit.includedFiles[parent].parent;
                    }

                    //Check if the hierarchy has size 1. If it does, then
                    // the error was generated in the top file, and we just
                    // need to print out the error.
                    if (hierarchy.size() == 1)
                    {
                        outLog << "In file " +
                            unit.includedFiles[hierarchy[0]].name + "(" +
                            std::to_string(lineNum) + ")" + error + "\n";
                        continue;
                    }

                    // Otherwise, we have to generate the include hierarchy
                    // messages. To do this, we traverse the list in reverse.
                    for (auto i = hierarchy.size() - 1; i > 0; --i)
                    {
                        outLog << "In file included from " +
                            unit.includedFiles[hierarchy[i]].name + ":\n";
                    }

                    // Now print the actual error message.
                    outLog << unit.includedFiles[hierarchy[0]].name +
                        "(" + std::to_string(lineNum) + ")" + error + "\n";
                }

                return outLog.str();
            }

            bool compileShader(ShaderUnit& unit)
            {
                if (unit.handle)
                {
                    glDetachShader(shaderProgram, unit.handle);
                    glDeleteShader(unit.handle);
                }

                if (unit.isSourceString)
                {
                    createTempFile(unit);
                }

                auto handle = glCreateShader(unit.type);
                auto sourceStr = readShaderSource(unit.filename, unit);

                if (sourceStr.empty())
                {
                    ERROR_LOG_V("File: \"%s\" is empty or cannot be read.",
                        unit.filename.c_str());
                    glDeleteShader(handle);

                    return false;
                }

                auto source = static_cast<const GLchar*>(sourceStr.c_str());


                glShaderSource(handle, 1, &source, NULL);
                source = nullptr;

                glCompileShader(handle);

                GLint compiled;
                glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
                if (!compiled)
                {
                    GLsizei len;
                    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

                    GLchar* log = new GLchar[len + 1];
                    glGetShaderInfoLog(handle, len, &len, log);

                    auto message = parseErrorLog(unit, std::string(log));

                    ERROR_LOG(message);

                    glDeleteShader(handle);
                    delete[] log;
                    return false;
                }

                glAttachShader(shaderProgram, handle);
                unit.handle = handle;
                return true;
            }

            void createTempFile(ShaderUnit& unit)
            {
                // First, we need to find the file extension that we are going
                // to sue.
                std::string extension;
                switch (unit.type)
                {
                case GL_VERTEX_SHADER:
                    extension = ".vs.glsl";
                    break;

                case GL_FRAGMENT_SHADER:
                    extension = ".fs.glsl";
                    break;

                case GL_TESS_CONTROL_SHADER:
                    extension = ".tcs.glsl";
                    break;

                case GL_TESS_EVALUATION_SHADER:
                    extension = ".tes.glsl";
                    break;

                case GL_GEOMETRY_SHADER:
                    extension = ".gs.glsl";
                    break;

                case GL_COMPUTE_SHADER:
                    extension = ".cs.glsl";
                    break;

                default:
                    break;
                }

                // Next we need to figure out the name of the temp file.
                std::string name = "tmp";
                int num = 0;
                while (true)
                {
                    std:: ifstream file(name + std::to_string(num) + extension);
                    if (!file)
                    {
                        break;
                    }
                    num++;
                }

                // Now create the file and dump the contents of the source
                // string to it.
                std::fstream file(name + std::to_string(num) + extension,
                    std::fstream::out);
                file << unit.filename;
                file.close();

                // Now save the name of the temp file as the actual filename.
                unit.filename = name + std::to_string(num) + extension;

                // Store the name of the created file so we can delete it 
                // when we exit the program.
                tmpFiles.push_back(unit.filename);
            }

            bool checkShaderProgram() const
            {
                if (shaderProgram == 0)
                {
                    return false;
                }

#ifdef ATLAS_DEBUG
                if (!glIsProgram(shaderProgram))
                {
                    return false;
                }
#endif

                return true;
            }

            GLint shaderProgram;
            std::vector<ShaderUnit> shaderUnits;
            std::vector<std::string> tmpFiles;
            bool isHotReloadAvailable;
            std::string includeDir;
        };

        Shader::Shader() :
            mImpl(std::make_unique<ShaderImpl>())
        { }

        Shader::Shader(std::vector<ShaderUnit> const& shaderUnits) :
            mImpl(std::make_unique<ShaderImpl>())
        {
            mImpl->shaderUnits = shaderUnits;
        }

        Shader::Shader(Shader&& rhs) :
            mImpl(std::make_unique<ShaderImpl>(*rhs.mImpl))
        {
            rhs.mImpl->shaderProgram = 0;
            rhs.mImpl->shaderUnits.clear();
        }

        Shader& Shader::operator=(Shader&& rhs)
        {
            *mImpl = *rhs.mImpl;
            rhs.mImpl->shaderProgram = 0;
            rhs.mImpl->shaderUnits.clear();

            return *this;
        }

        Shader::~Shader()
        {
            deleteShaders();

            mImpl->shaderUnits.clear();

            if (mImpl->shaderProgram)
            {
                glDeleteProgram(mImpl->shaderProgram);
                mImpl->shaderProgram = 0;
            }

            for (auto& file : mImpl->tmpFiles)
            {
                std::remove(file.c_str());
            }
        }

        void Shader::setShaders(std::vector<ShaderUnit> const& shaders)
        {
            mImpl->shaderUnits = shaders;
        }
        
        void Shader::setShaderIncludeDir(std::string const& dir)
        {
            mImpl->includeDir = dir;
        }

        bool Shader::compileShaders(int idx)
        {
            if (mImpl->shaderUnits.empty())
            {
                WARN_LOG("Received empty shader list.");
                return false;
            }

            if (!mImpl->checkShaderProgram())
            {
                mImpl->shaderProgram = glCreateProgram();
            }

            if (idx == -1)
            {
                for (auto& shader : mImpl->shaderUnits)
                {
                    if (!mImpl->compileShader(shader))
                    {
                        // Shader compilation failed, so set the program to 0,
                        // and return false.
                        glDeleteProgram(mImpl->shaderProgram);
                        mImpl->shaderProgram = 0;
                        return false;
                    }
                }
            }
            else
            {
                if (0 <= idx && idx < mImpl->shaderUnits.size())
                {
                    if (!mImpl->compileShader(mImpl->shaderUnits[idx]))
                    {
                        glDeleteProgram(mImpl->shaderProgram);
                        mImpl->shaderProgram = 0;
                        return false;
                    }
                }
            }

            return true;
        }

        bool Shader::linkShaders()
        {
            if (!mImpl->shaderProgram)
            {
                ERROR_LOG("Cannot link with an empty program.");
                return false;
            }

            glLinkProgram(mImpl->shaderProgram);

            GLint linked;
            glGetProgramiv(mImpl->shaderProgram, GL_LINK_STATUS, &linked);

            if (!linked)
            {
                GLsizei len;
                glGetProgramiv(mImpl->shaderProgram, GL_INFO_LOG_LENGTH, &len);

                GLchar* log = new GLchar[len + 1];
                glGetProgramInfoLog(mImpl->shaderProgram, len, &len, log);

                ERROR_LOG("Shader linking failed: " + std::string(log));

                deleteShaders();
                delete[] log;
                return false;
            }

            return true;
        }

        void Shader::deleteShaders(int idx)
        {
            if (idx == -1)
            {
                for (auto& shader : mImpl->shaderUnits)
                {
                    if (shader.handle == 0)
                    {
                        continue;
                    }

                    glDetachShader(mImpl->shaderProgram, shader.handle);
                    glDeleteShader(shader.handle);
                }
            }
            else
            {
                if (0 <= idx && idx < mImpl->shaderUnits.size())
                {
                    if (mImpl->shaderUnits[idx].handle == 0)
                    {
                        return;
                    }

                    glDetachShader(mImpl->shaderProgram, 
                        mImpl->shaderUnits[idx].handle);
                    glDeleteShader(mImpl->shaderUnits[idx].handle);
                }
            }

        }

        bool Shader::reloadShaders(int idx)
        {
            bool ret = true;
            ret = compileShaders(idx);
            ret = linkShaders();
            return ret;
        }

        bool Shader::hotReloadShaders()
        {
            // Check if hot reloading is available.
            if (!mImpl->isHotReloadAvailable)
            {
                return false;
            }

            int i = 0;
            std::vector<int> changedShaders;
            for (auto& unit : mImpl->shaderUnits)
            {
                // Now check every single included file. If the timestamps
                // are different, then queue it to be reloaded.
                for (auto& file : unit.includedFiles)
                {
                    time_t stamp = mImpl->getFileTimeStamp(file.name);

                    double secs = std::difftime(stamp, file.timeStamp);
                    if (secs > 0)
                    {
                        // Add the unit to the queue and update the timestamp
                        // on the file.
                        file.timeStamp = stamp;
                        changedShaders.push_back(i);
                    }
                }

                ++i;
            }

            // If there's nothing to reload, return.
            if (changedShaders.empty())
            {
                return false;
            }

            // Before we do anything else, cull the list of shaders to
            // ensure that we don't reload the same thing multiple times.
            std::set<int> set;
            for (auto& idx : changedShaders)
            {
                set.insert(idx);
            }


            changedShaders.assign(set.begin(), set.end());

            // Now loop through the units that need to be recompiled and
            // reload them.
            bool ret = true;
            for (auto& idx : changedShaders)
            {
                ret = reloadShaders(idx);
            }

            return ret;
        }

        void Shader::bindAttribute(GLuint location, 
            std::string const& name) const
        {
            if (mImpl->checkShaderProgram())
            {
                glBindAttribLocation(mImpl->shaderProgram, location,
                    name.c_str());
            }
        }

        void Shader::enableShaders() const
        {
            if (mImpl->checkShaderProgram())
            {
                glUseProgram(mImpl->shaderProgram);
            }
        }

        void Shader::disableShaders() const
        {
            glUseProgram(0);
        }

        GLint Shader::getShaderProgram() const
        {
            return mImpl->shaderProgram;
        }

        bool Shader::shaderProgramValid() const
        {
            return mImpl->checkShaderProgram();
        }

        GLint Shader::getUniformVariable(std::string const& name) const
        {
            GLint ret = -1;
            if (mImpl->checkShaderProgram())
            {
                ret = glGetUniformLocation(mImpl->shaderProgram, 
                    name.c_str());
                if (ret == -1)
                {
                    ERROR_LOG(std::string("The uniform location \"") + name +
                        std::string("\" is invalid."));
                }
            }

            return ret;
        }

        GLint Shader::getAttributeVariable(std::string const& name) const
        {
            GLint ret = -1;
            if (mImpl->checkShaderProgram())
            {
                ret = glGetAttribLocation(mImpl->shaderProgram,
                    name.c_str());
                if (ret == -1)
                {
                    ERROR_LOG(std::string("The attribute location \"") + name +
                        std::string("\" is invalid."));
                }
            }

            return ret;
        }

        GLuint Shader::getUniformBlockIndex(std::string const& name) const
        {
            GLuint ret = GL_INVALID_INDEX;
            if (mImpl->checkShaderProgram())
            {
                ret = glGetUniformBlockIndex(mImpl->shaderProgram, 
                    name.c_str());
                if (ret == GL_INVALID_INDEX)
                {
                    ERROR_LOG_V("The block index %s is invalied.", 
                        name.c_str());
                }
            }

            return ret;
        }

        void Shader::uniformBlockBinding(GLuint uniformBlockIndex,
            GLuint uniformBlockBinding) const
        {
            glUniformBlockBinding(mImpl->shaderProgram,
                uniformBlockIndex, uniformBlockBinding);
        }
    }
}