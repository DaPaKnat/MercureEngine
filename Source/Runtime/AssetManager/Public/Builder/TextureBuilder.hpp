#ifndef __TEXTURE_BUILDER_HPP__
#define __TEXTURE_BUILDER_HPP__

#include "CoreMinimal.hpp"

// ============================== [Forward Declaration] ============================== //

class Texture;

// =================================================================================== //

class ENGINE_API TextureBuilder : public UniqueObject
{
    public:

    // ============================== [Public Static Methods] ============================== //

        /**
         * @return Whether or not the texture's extension is supported.
         */
        static bool                     IsExtensionSupported    (std::string const& p_extension)    noexcept;

        /**
         * Creates a new texture from the specified file.
         */
        static std::shared_ptr<Texture> BuildFromFile           (std::string const& p_name,
                                                                 std::string const& p_path)         noexcept;

    private:

    // ============================== [Private Static Variables] ============================== //

        static std::array<std::string, 8> SupportedExtensions;

    // ============================== [Private Constructor and Destructor] ============================== //

        TextureBuilder  () = default;

        ~TextureBuilder () = default;

};  // !class TextureBuilder

#endif // !__TEXTURE_BUILDER_HPP__