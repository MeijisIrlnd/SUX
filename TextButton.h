#pragma once 
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX 
{ 
    class TextButton : public juce::Component 
    { 
    public: 
        struct Listener
        {
            virtual ~Listener() {}
            virtual void onTextButtonClicked(TextButton* source) = 0;
        };

        TextButton(const std::string& text, const juce::Colour& colour = juce::Colour(0x00000000), const juce::Colour& textColour = juce::Colour(0x00000000)) : m_colour(colour), m_text(text), 
            m_textColour(textColour)
        {
            if (m_textColour.getARGB() != 0) {
                m_label.setColour(juce::Label::ColourIds::textColourId, m_textColour);
            }
            m_label.setInterceptsMouseClicks(false, false);
            m_label.setJustificationType(juce::Justification::centred);
            m_label.setText(text, juce::dontSendNotification);
            addAndMakeVisible(&m_label);
        }

        TextButton(const TextButton& other) : m_listener(other.m_listener), m_colour(other.m_colour), m_text(other.m_text), m_textColour(other.m_textColour)
        {
            if (m_textColour.getARGB() != 0) {
                m_label.setColour(juce::Label::ColourIds::textColourId, m_textColour);
            }
            m_label.setInterceptsMouseClicks(false, false);
            m_label.setJustificationType(juce::Justification::centred);
            m_label.setText(m_text, juce::dontSendNotification);
            addAndMakeVisible(&m_label);
        }

        TextButton(TextButton&& other) noexcept : m_listener(other.m_listener), m_colour(other.m_colour), m_text(other.m_text), m_textColour(other.m_textColour)
        {
            if (m_textColour.getARGB() != 0) {
                m_label.setColour(juce::Label::ColourIds::textColourId, m_textColour);
            }
            m_label.setInterceptsMouseClicks(false, false);
            m_label.setJustificationType(juce::Justification::centred);
            m_label.setText(m_text, juce::dontSendNotification);
            addAndMakeVisible(&m_label);
        }

        ~TextButton() override { }

        TextButton& operator=(const TextButton& other) {
            m_listener = other.m_listener;
            m_colour = other.m_colour;
            m_textColour = other.m_textColour;
            m_text = other.m_text;
            if (m_textColour.getARGB() != 0) {
                m_label.setColour(juce::Label::ColourIds::textColourId, m_textColour);
            }
            m_label.setInterceptsMouseClicks(false, false);
            m_label.setJustificationType(juce::Justification::centred);
            m_label.setText(m_text, juce::dontSendNotification);
            addAndMakeVisible(&m_label);
            return *this;
        }

        void setFont(const juce::Font& newFont) {
            m_label.setFont(newFont);
           
        }

        void addListener(Listener* newListener) { m_listener = newListener; }

        void mouseUp(const juce::MouseEvent& mouseEvent) override {
            if (m_listener == nullptr) return;
            m_listener->onTextButtonClicked(this);
        }
        void paint(juce::Graphics& g) override {
            g.fillAll(m_colour);
        }

        void resized() override {
            m_label.setBounds(0, 0, getWidth(), getHeight());
        }

    private:
        juce::Colour m_textColour;
        std::string m_text;
        Listener* m_listener{ nullptr };
        juce::Label m_label;
        juce::Colour m_colour;
    };
}