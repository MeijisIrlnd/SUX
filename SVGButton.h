#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX 
{
    class SVGButton : public juce::Component 
    { 
    public: 
        struct Listener
        {
            virtual ~Listener() = default;
            virtual void onSvgButtonClick(SVGButton* source) = 0;
        };

        SVGButton(const void* sourceData, int size, const juce::Colour& /*colour*/ = juce::Colour(0xFF000000), float rotationRadians = 0) : m_rotationRadians(rotationRadians)
        {
            m_svg = juce::Drawable::createFromImageData(sourceData, static_cast<size_t>(size));
            m_svg->setInterceptsMouseClicks(false, false);
            //m_svg->replaceColour(juce::Colour(0xFFFFFFFF), colour);
            addAndMakeVisible(m_svg.get());
        }

        ~SVGButton() override = default;

        void addListener(Listener* newListener) {
            m_listener = newListener;
        }

        void mouseUp(const juce::MouseEvent& /*mouseEvent*/) override {
            if(onClick != nullptr) {
                onClick();
            }
            if (m_listener == nullptr) return;
            m_listener->onSvgButtonClick(this);
        }

        void paint(juce::Graphics& g) override
        {
            m_svg->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
            m_rotationRadians == 0 ? m_svg->draw(g, 1.0f) : m_svg->draw(g, 1.0f, juce::AffineTransform::rotation(m_rotationRadians, static_cast<float>(getWidth()) / 2.0f, static_cast<float>(getHeight()) / 2.0f));
        }

        void resized() override
        {
            //m_svg->setBounds(getBounds());
        }

        std::function<void(void)> onClick{ nullptr };

    private: 
        Listener* m_listener{ nullptr };
        std::unique_ptr<juce::Drawable> m_svg{ nullptr };
        float m_rotationRadians{ 0 };
        
    };

    class SVGToggleButton : public juce::Component
    {
    public:
        struct Listener {
            virtual ~Listener() = default;
            virtual void onSVGToggleClicked(SVGToggleButton* /*source*/) = 0;
        };

        SVGToggleButton(const void* data, int size, const juce::Colour& offColour, const juce::Colour& onColour) :
            m_offColour(offColour), m_onColour(onColour)
        {
            m_svg = juce::Drawable::createFromImageData(data, static_cast<size_t>(size));

        }

        ~SVGToggleButton() override = default;

        void addListener(Listener* newListener) {
            m_listener = newListener;
        }

        void mouseUp(const juce::MouseEvent& /*ev*/) override {
            m_state = !m_state;
            repaint();
            if(onChange != nullptr) {
                onChange(m_state);
            }
            if(m_listener == nullptr) return;
            m_listener->onSVGToggleClicked(this);
        }

        void setState(bool newState, bool invokeCallback) {
            m_state = newState;
            if(invokeCallback) {
                if(onChange != nullptr) {
                    onChange(m_state);
                }
                if(m_listener != nullptr) {
                    m_listener->onSVGToggleClicked(this);
                }
            }
            repaint();
        }

        void paint(juce::Graphics& g) override {
            m_svg->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
            auto path = m_svg->getOutlineAsPath();
            g.reduceClipRegion(path);
            g.setColour(m_state ? m_onColour : m_offColour);
            g.fillAll();
        }

        void resized() override {

        }

        std::function<void(bool)> onChange{ nullptr };
    private:
        bool m_state{ false };
        juce::Colour m_offColour, m_onColour;
        std::unique_ptr<juce::Drawable> m_svg{ nullptr };
        Listener* m_listener{ nullptr };
    };


} 
