//
// Created by Syl Morrison on 26/06/2023.
//

#ifndef COLLAGE_COLOURCOMPOSITING_H
#define COLLAGE_COLOURCOMPOSITING_H
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX::PorterDuff {
    [[nodiscard]] static inline juce::Colour composite(const juce::Colour& source, const juce::Colour& dest, float f_a, float f_b) {
        const auto r_x = (source.getFloatRed() * f_a) + (dest.getFloatRed() * f_b);
        const auto g_x = (source.getFloatBlue() * f_a) + (dest.getFloatBlue() * f_b);
        const auto b_x = (source.getFloatGreen() * f_a) + (dest.getFloatBlue() * f_b);
        return juce::Colour::fromFloatRGBA(r_x, g_x, b_x, 1.0f);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour clear(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 0.0f;
        const auto f_b = 0.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour a(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f;
        const auto f_b = 0.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour b(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 0.0f;
        const auto f_b = 1.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour aOverB(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f;
        const auto f_b = 1.0f - source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour bOverA(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f - destination.getFloatAlpha();
        const auto f_b = 1.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour aInB(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = destination.getFloatAlpha();
        const auto f_b = 0.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour bInA(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 0.0f;
        const auto f_b = source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour aOutB(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f - destination.getFloatAlpha();
        const auto f_b = 0.0f;
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour bOutA(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 0.0f;
        const auto f_b = 1.0f - source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour aAtopB(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = destination.getFloatAlpha();
        const auto f_b = 1.0f - source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour bAtopA(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f - destination.getFloatAlpha();
        const auto f_b = source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }

    [[maybe_unused]] [[nodiscard]] static inline juce::Colour aXorB(const juce::Colour& source, const juce::Colour& destination) {
        const auto f_a = 1.0f - destination.getFloatAlpha();
        const auto f_b = 1.0f - source.getFloatAlpha();
        return composite(source, destination, f_a, f_b);
    }
}
#endif //COLLAGE_COLOURCOMPOSITING_H
