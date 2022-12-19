#pragma once 

namespace SUX 
{ 
    inline static juce::Colour colourBurn(juce::Colour& source, juce::Colour& modifier) 
    { 
        std::array<float, 3> sourceHSV, modifierHSV, result;
        source.getHSB(sourceHSV[0], sourceHSV[1], sourceHSV[2]);
        modifier.getHSB(modifierHSV[0], modifierHSV[1], modifierHSV[2]);
        result[0] = modifierHSV[0];
        result[1] = sourceHSV[1];
        result[2] = modifierHSV[2];
        return juce::Colour::fromHSV(result[0], result[1], result[2], 1.0f);
    }
}