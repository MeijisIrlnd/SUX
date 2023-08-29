#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include <memory>
namespace SUX::Quickstart
{
    enum class TYPE { SLIDER, COMBOBOX };
    struct QuickBase {
        explicit QuickBase(TYPE t, juce::Component* c) : type(t), component(c) { }
        juce::Label label;
        juce::Component* component;
        TYPE type;
    };

    struct QuickSlider : public QuickBase
    {
        QuickSlider() : QuickBase(TYPE::SLIDER, &slider) {}
        juce::Slider slider;
        std::unique_ptr<juce::SliderParameterAttachment> attachment{nullptr};
    };

    struct QuickComboBox : public QuickBase {
        QuickComboBox() : QuickBase(TYPE::COMBOBOX, &comboBox) { }
        juce::ComboBox comboBox;
        std::unique_ptr<juce::ComboBoxParameterAttachment> attachment{nullptr};
    };
    struct LayoutQuickstart 
    {
        using APVTS = juce::AudioProcessorValueTreeState;
        std::vector<QuickBase*> uiElements;
        [[maybe_unused]] void instantiateSlider(juce::Component* parent, QuickSlider& quickSlider, APVTS& tree, const juce::String& paramName)
        { 
            auto* param = tree.getParameter(paramName);
            quickSlider.label.setText(TRANS(param->getName(80)), juce::dontSendNotification);
            parent->addAndMakeVisible(&quickSlider.label);
            quickSlider.slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
            quickSlider.slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 30, 20);
            parent->addAndMakeVisible(&quickSlider.slider);
            quickSlider.attachment = std::make_unique<juce::SliderParameterAttachment>(*param, quickSlider.slider, nullptr);
            uiElements.push_back(&quickSlider);
        }

        [[maybe_unused]] void instantiateComboBox(juce::Component* parent, QuickComboBox& quickCb, APVTS& tree, const juce::String& paramName, const juce::StringArray& itemList) {
            auto* param = tree.getParameter(paramName);
            quickCb.label.setText(TRANS(param->getName(80)), juce::dontSendNotification);
            parent->addAndMakeVisible(&quickCb.label);
            quickCb.comboBox.addItemList(itemList, 1);
            parent->addAndMakeVisible(&quickCb.comboBox);
            quickCb.attachment.reset(new juce::ComboBoxParameterAttachment(*param, quickCb.comboBox, nullptr));
            uiElements.push_back(&quickCb);
        }
    };
} 
