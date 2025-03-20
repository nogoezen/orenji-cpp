#pragma once

#include <string>

namespace tmx
{
    /**
     * @brief Stub class for tmxlite's Property class
     * This is only for compilation purposes and should be replaced with the actual tmxlite library
     */
    class Property
    {
    public:
        enum class Type
        {
            Boolean,
            Float,
            Int,
            String,
            Colour
        };

        Type getType() const { return Type::String; }

        std::string getName() const { return ""; }
        bool getBoolValue() const { return false; }
        float getFloatValue() const { return 0.f; }
        int getIntValue() const { return 0; }
        std::string getStringValue() const { return ""; }

        struct Colour
        {
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            uint8_t a = 255;
        };

        Colour getColourValue() const { return {}; }
    };
}