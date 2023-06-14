#pragma once 

namespace SUX 
{ 
    /// @brief THIS IS SUPER FUCKING INEFFICIENT, PROBABLY BECAUSE OF BOUNDS CHECKING. 
    /// Can also parallelise these fors probably...
    /// @param target 
    /// @param newWhite 
    /// @param fixSaturation 
    /// @return 
    
    [[maybe_unused]] inline static juce::Image recolourImageCopy(juce::Image& target, const juce::Colour& newWhite, bool fixSaturation = true)
    {
        juce::Image result = target.createCopy();
        float destHue, destSat, destValue;
        newWhite.getHSB(destHue, destSat, destValue);

        for (auto y = 0; y < result.getHeight(); y++) {
            for (auto x = 0; x < result.getWidth(); x++) {
                // to hsv..
                float srcHue, srcSaturation, srcValue;
                auto srcColour = result.getPixelAt(x, y);
                srcColour.getHSB(srcHue, srcSaturation, srcValue);
                result.setPixelAt(x, y, juce::Colour::fromHSV(destHue, fixSaturation ? 1 : destSat, srcValue, srcColour.getFloatAlpha()));
            }
        }
        return result;
    }

    [[maybe_unused]] inline static void recolourImageInPlace(juce::Image& target, const juce::Colour& newWhite, bool fixSaturation = true) {
        float destHue, destSat, destValue;
        newWhite.getHSB(destHue, destSat, destValue);
        for (auto y = 0; y < target.getHeight(); y++) {
            for (auto x = 0; x < target.getWidth(); x++) {
                // to hsv..
                float srcHue, srcSaturation, srcValue;
                auto srcColour = target.getPixelAt(x, y);
                srcColour.getHSB(srcHue, srcSaturation, srcValue);
                target.setPixelAt(x, y, juce::Colour::fromHSV(destHue, fixSaturation ? 1 : destSat, srcValue, srcColour.getFloatAlpha()));
            }
        }
    
    }

    [[maybe_unused]] inline static juce::Colour colourBurn(juce::Colour& source, juce::Colour& modifier)
    { 
        std::array<float, 3> sourceHSV{}, modifierHSV{}, result{};
        source.getHSB(sourceHSV[0], sourceHSV[1], sourceHSV[2]);
        modifier.getHSB(modifierHSV[0], modifierHSV[1], modifierHSV[2]);
        result[0] = modifierHSV[0];
        result[1] = sourceHSV[1];
        result[2] = modifierHSV[2];
        return juce::Colour::fromHSV(result[0], result[1], result[2], 1.0f);
    }
}