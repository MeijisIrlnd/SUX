#pragma once 

#include <juce_gui_basics/juce_gui_basics.h>

#include <utility>
namespace SUX
{ 
    class ImageButton : public juce::Component 
    { 
    public:
        struct Listener
        {
            virtual ~Listener() = default;
            virtual void onImageButtonClick(ImageButton* source) = 0;
        };

        ImageButton(const void* data, int size, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) :
            m_colour(colour),
            m_reduction(reduction)
        { 
            m_image = juce::ImageCache::getFromMemory(data, size);
        }

        explicit ImageButton(juce::Image img, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) :
            m_image(std::move(img)), m_colour(colour), m_reduction(reduction)
        { 
        }

        ImageButton(const ImageButton& other) : m_image(other.m_image), m_colour(other.m_colour), m_reduction(other.m_reduction) { }

        ImageButton(ImageButton&& other) noexcept : m_image(other.m_image), m_colour(other.m_colour), m_reduction(other.m_reduction) { }

        ImageButton& operator=(const ImageButton& other)
        {
            if(&other != this) {
                m_image = other.m_image;
                m_colour = other.m_colour;
                m_reduction = other.m_reduction;
            }
            return *this;
        }

        ~ImageButton() override = default;

        void addListener(Listener* newListener) {
            m_listener = newListener;
        }

        void setColour(const juce::Colour& newColour) {
            m_colour = newColour;
            repaint();
        }

        void mouseUp(const juce::MouseEvent& /*mouseEvent*/) override {
            if (m_listener == nullptr) return;
            m_listener->onImageButtonClick(this);
        }

        void paint(juce::Graphics& g) override { 
            auto startX = getWidth() * m_reduction.first;
            auto startY = getHeight() * m_reduction.second;
            g.setColour(m_colour);
            g.drawImageAt(m_renderImage, static_cast<int>(startX), static_cast<int>(startY), true);
        }

        void reduction(double xOffsetMultiplier, double yOffsetMultiplier) {
            m_reduction.first = xOffsetMultiplier;
            m_reduction.second = yOffsetMultiplier;
            repaint();
        }


        void resized() override { 
            auto startX = getWidth() * m_reduction.first;
            auto startY = getHeight() * m_reduction.second;
            int imgWidth = static_cast<int>(getWidth() - (startX * 2));
            int imgHeight = static_cast<int>(getHeight() - (startY * 2));
            m_renderImage = m_image.rescaled(imgWidth, imgHeight, juce::Graphics::ResamplingQuality::highResamplingQuality);
        }
        
    private:    
        std::pair<double, double> m_reduction = { 0, 0 };
        juce::Image m_image, m_renderImage;
        juce::Colour m_colour;
        Listener* m_listener{ nullptr };
    };
}