#pragma once
#include <JuceHeader.h>

namespace SUX 
{
    class SVGButton : public juce::Component 
    { 
    public: 
        struct Listener
        {
            virtual ~Listener() {}
            virtual void onSvgButtonClick(SVGButton* source) = 0;
        };

        SVGButton(const void* sourceData, int size, const juce::Colour& colour = juce::Colour(0xFF000000), float rotationRadians = 0) : m_rotationRadians(rotationRadians)
        {
            m_svg = juce::Drawable::createFromImageData(sourceData, size);
            m_svg->setInterceptsMouseClicks(false, false);
            //m_svg->replaceColour(juce::Colour(0xFFFFFFFF), colour);
            addAndMakeVisible(m_svg.get());
        }


        ~SVGButton()
        {

        }

        void addListener(Listener* newListener) {
            m_listener = newListener;
        }

        void mouseUp(const juce::MouseEvent& mouseEvent) override {
            if (m_listener == nullptr) return;
            m_listener->onSvgButtonClick(this);
        }

        void paint(juce::Graphics& g) override
        {
            m_svg->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
            m_rotationRadians == 0 ? m_svg->draw(g, 1.0f) : m_svg->draw(g, 1.0f, juce::AffineTransform::rotation(m_rotationRadians, getWidth() / 2, getHeight() / 2));
        }

        void resized() override
        {
            //m_svg->setBounds(getBounds());
        }

    private: 
        Listener* m_listener{ nullptr };
        std::unique_ptr<juce::Drawable> m_svg{ nullptr };
        float m_rotationRadians{ 0 };
        
    };

} 
