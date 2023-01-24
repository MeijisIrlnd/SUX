#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace SUX::Quickstart
{
    struct QuickSlider 
    { 
        juce::Slider slider;
        juce::Label label;
        std::unique_ptr<juce::SliderParameterAttachment> attachment{nullptr};
    };

    struct LayoutQuickstart 
    {
        using APVTS = juce::AudioProcessorValueTreeState;
        std::vector<QuickSlider*> uiElements;
        void instantiateSlider(juce::Component* parent, QuickSlider& quickSlider, APVTS& tree, const juce::String& paramName)
        { 
            auto* param = tree.getParameter(paramName);
            quickSlider.label.setText(param->getName(80), juce::dontSendNotification);
            parent->addAndMakeVisible(&quickSlider.label);
            quickSlider.slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
            quickSlider.slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 30, 20);
            parent->addAndMakeVisible(&quickSlider.slider);
            quickSlider.attachment.reset(new juce::SliderParameterAttachment(*param, quickSlider.slider, nullptr));
            uiElements.push_back(&quickSlider);
        }
    };
} 
