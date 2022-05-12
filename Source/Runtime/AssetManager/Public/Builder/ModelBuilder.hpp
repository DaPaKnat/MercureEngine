#ifndef __MODEL_BUILDER_HPP__
#define __MODEL_BUILDER_HPP__

#include "CoreMinimal.hpp"

// ============================== [Forward Declaration] ============================== //

class Model;

// =================================================================================== //

class ENGINE_API ModelBuilder : public UniqueObject
{
    public:

    // ============================== [Public Static Methods] ============================== //

        /**
         * @return Whether or not the model's extension is supported.
         */
        static bool                     IsExtensionSupported    (std::string const& p_extension)    noexcept;

        /**
         * Creates a new model from the specified file.
         */
        static std::shared_ptr<Model>   BuildFromFile           (std::string const& p_name,
                                                                 std::string const& p_path)         noexcept;

    private:

    // ============================== [Private Static Variables] ============================== //

        static std::array<std::string, 2> SupportedExtensions;

    // ============================== [Private Static Methods] ============================== //

        static std::shared_ptr<Model>   BuildFromObj    (std::string const& p_name,
                                                         std::string const& p_path) noexcept;

        static std::shared_ptr<Model>   BuildFromFbx    (std::string const& p_name,
                                                         std::string const& p_path) noexcept;

        static std::shared_ptr<Model>   BuildFromGLTF   (std::string const& p_name,
                                                         std::string const& p_path) noexcept;

    // ============================== [Private Constructor and Destructor] ============================== //

        ModelBuilder    () = default;

        ~ModelBuilder   () = default;

};  // !class ModelBuilder

#endif // !__MODEL_BUILDER_HPP__