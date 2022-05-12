#ifndef __SHADER_BUILDER_HPP__
#define __SHADER_BUILDER_HPP__

#include "CoreMinimal.hpp"

// ============================== [Forward Declaration] ============================== //

class Shader;

// =================================================================================== //

class ENGINE_API ShaderBuilder : public UniqueObject
{
    public:

    // ============================== [Public Static Methods] ============================== //

        /**
         * @return Whether or not the shader's extension is supported.
         */
        static bool                     IsExtensionSupported    (std::string const& p_extension)    noexcept;

        /**
         * Creates a new shader from the specified file.
         */
        static std::shared_ptr<Shader>  BuildFromFile           (std::string const& p_name,
                                                                 std::string const& p_path)         noexcept;

    private:

    // ============================== [Private Static Variables] ============================== //

        static std::array<std::string, 1> SupportedExtensions;

    // ============================== [Private Constructor and Destructor] ============================== //

        ShaderBuilder   () = default;

        ~ShaderBuilder  () = default;

};  // !class TextureBuilder

#endif // !__SHADER_BUILDER_HPP__