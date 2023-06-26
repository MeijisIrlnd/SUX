#pragma once 
#if defined(PERFETTO)
    #if PERFETTO
        #include <melatonin_perfetto/melatonin_perfetto.h>
    #endif
#endif

#include "ColourCompositing.h"
namespace SUX 
{ 
    /// @brief THIS IS SUPER FUCKING INEFFICIENT, PROBABLY BECAUSE OF BOUNDS CHECKING. 
    /// Can also parallelise these fors probably...
    /// @param target 
    /// @param newWhite 
    /// @param fixSaturation 
    /// @return 

    [[maybe_unused]] [[nodiscard]] inline static bool isGreyscale(const juce::Colour& x) noexcept {
        return x.getRed() == x.getGreen() && x.getRed() == x.getBlue();
    }


    [[maybe_unused]] [[nodiscard]] inline static float blendChannel(float c_0, float a_0, float c_1, float a_1, float a_x) {
        return ((c_0 * a_0) + (c_1 * a_1 * (1 - a_0))) / a_x;
    }

    [[maybe_unused]] inline static juce::Colour mixColours(const juce::Colour& c0, const juce::Colour& c1) {
        auto a_x = c0.getFloatAlpha() + c1.getFloatAlpha() * (1 - c0.getFloatAlpha());
        auto r_x = blendChannel(c0.getFloatRed(), c0.getFloatAlpha(), c1.getFloatRed(), c1.getFloatAlpha(), a_x);
        auto g_x = blendChannel(c0.getFloatGreen(), c0.getFloatAlpha(), c1.getFloatGreen(), c1.getFloatAlpha(), a_x);
        auto b_x = blendChannel(c0.getFloatBlue(), c0.getFloatAlpha(), c1.getFloatBlue(), c1.getFloatAlpha(), a_x);
        return juce::Colour::fromFloatRGBA(r_x, g_x, b_x, a_x);
    }

    [[maybe_unused]] inline static juce::Image recolourImageCopy(juce::Image& target, const juce::Colour& newWhite, bool fixSaturation = true)
    {

        juce::Image result = target.createCopy();
        if(isGreyscale(newWhite)) {
            for(auto y = 0; y < target.getHeight(); ++y) {
                for(auto x = 0; x < target.getWidth(); ++x) {
                    if(target.getPixelAt(x,y).getFloatAlpha() == 0) continue;
                    auto blended = PorterDuff::bAtopA(target.getPixelAt(x, y), newWhite);
                    result.setPixelAt(x, y, blended);
                }
            }
        }
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
#if defined(PERFETTO)
#if PERFETTO
        TRACE_COMPONENT();
#endif
#endif
        if(isGreyscale(newWhite)) {
            for(auto y = 0; y < target.getHeight(); ++y) {
                for(auto x = 0; x < target.getWidth(); ++x) {
                    if(target.getPixelAt(x,y).getFloatAlpha() == 0) continue;
                    auto blended = PorterDuff::bAtopA(target.getPixelAt(x, y), newWhite);
                    target.setPixelAt(x, y, blended);
                }
            }
        }
        else {
            float destHue, destSat, destValue;
            newWhite.getHSB(destHue, destSat, destValue);
            for (auto y = 0; y < target.getHeight(); y++) {
                for (auto x = 0; x < target.getWidth(); x++) {
                    // to hsv..
                    float srcHue, srcSaturation, srcValue;
                    auto srcColour = target.getPixelAt(x, y);
                    srcColour.getHSB(srcHue, srcSaturation, srcValue);
                    target.setPixelAt(x, y, juce::Colour::fromHSV(destHue, fixSaturation ? 1 : destSat, srcValue,
                                                                  srcColour.getFloatAlpha()));
                }
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