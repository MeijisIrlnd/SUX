#pragma once

#include "Utils/BlendModes.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <utility>
namespace SUX {
    class ImageButton : public juce::Component {
    public:
        struct Listener {
            virtual ~Listener() = default;
            virtual void onImageButtonClick(ImageButton* source) = 0;
        };

        ImageButton(const void* data, int size, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) : m_colour(colour),
                                                                                                                                                                     m_reduction(reduction) {
            m_image = juce::ImageCache::getFromMemory(data, size);
        }

        explicit ImageButton(juce::Image img, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) : m_image(std::move(img)), m_colour(colour), m_reduction(reduction) {
        }

        ImageButton(const ImageButton& other) : m_image(other.m_image), m_colour(other.m_colour), m_reduction(other.m_reduction) {}

        ImageButton(ImageButton&& other) noexcept : m_image(other.m_image), m_colour(other.m_colour), m_reduction(other.m_reduction) {}

        ImageButton& operator=(const ImageButton& other) {
            if (&other != this) {
                m_image = other.m_image;
                m_colour = other.m_colour;
                m_reduction = other.m_reduction;
            }
            return *this;
        }

        virtual ~ImageButton() override = default;

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
            int imgWidth = static_cast<int>(getWidth() - (startX * 2));
            int imgHeight = static_cast<int>(getHeight() - (startY * 2));
            auto toDraw = m_image.rescaled(imgWidth, imgHeight, juce::Graphics::highResamplingQuality);
            g.setColour(m_colour);
            g.drawImageAt(toDraw, static_cast<int>(startX), static_cast<int>(startY), true);
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

    class ImageButtonWithTooltip : public ImageButton, public juce::TooltipClient {
    public:
        ImageButtonWithTooltip(const void* data, int size, const juce::String& tooltipText, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) : ImageButton(data, size, colour, reduction), m_tooltipText(tooltipText) {
        }

        ImageButtonWithTooltip(juce::Image img, const juce::String& tooltipText, const juce::Colour& colour = juce::Colour(0xFFFFFFFF), std::pair<double, double> reduction = std::make_pair(0, 0)) : ImageButton(std::move(img), colour, reduction) {
        }

        ImageButtonWithTooltip(const ImageButtonWithTooltip& other) : ImageButton(other), m_tooltipText(other.m_tooltipText) {}

        ImageButtonWithTooltip(ImageButtonWithTooltip&& other) noexcept : ImageButton(other), m_tooltipText(std::move(other.m_tooltipText)) {}

        ImageButtonWithTooltip& operator=(const ImageButtonWithTooltip& other) {
            ImageButtonWithTooltip::operator=(other);
            if (&other != this) {
                m_tooltipText = other.m_tooltipText;
            }
            return *this;
        }

        [[nodiscard]] juce::String getTooltip() override {
            return m_tooltipText;
        }

    private:
        juce::String m_tooltipText;
    };

    class ImageToggleButton : public juce::Component {
    public:
        struct Listener {
            virtual ~Listener() = default;
            virtual void onToggleButtonStateChanged(ImageToggleButton* source, bool newState) = 0;
        };

        ImageToggleButton(const void* data, int size, const juce::Colour& offColour, const juce::Colour& onColour) : m_offColour(offColour), m_onColour(onColour) {
            m_image = juce::ImageCache::getFromMemory(data, size);
        }

        ~ImageToggleButton() override = default;

        void addListener(Listener* newListener) {
            m_listener = newListener;
        }

        void mouseUp(const juce::MouseEvent& /*mouseEvent*/) override {
            triggerClick(true);
        }

        void triggerClick(bool triggerCallback = true) {
            m_internalState = !m_internalState;
            repaint();
            if (triggerCallback) {
                if (m_listener == nullptr) return;
                m_listener->onToggleButtonStateChanged(this, m_internalState);
            }
        }

        void setState(bool newState, bool triggerCallback = true) {
            m_internalState = newState;
            repaint();
            if (triggerCallback) {
                if (m_listener == nullptr) return;
                m_listener->onToggleButtonStateChanged(this, m_internalState);
            }
        }

        [[nodiscard]] bool getInternalState() const { return m_internalState; }
        void paint(juce::Graphics& g) override {
            auto& colour = m_internalState ? m_onColour : m_offColour;
            g.setColour(colour);
            auto rescaled = m_image.rescaled(getWidth(), getHeight(), juce::Graphics::highResamplingQuality);
            g.drawImage(rescaled, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit, true);
        }

        void resized() override {
        }

    private:
        juce::Colour m_offColour, m_onColour;
        juce::Image m_image;
        bool m_internalState{ false };
        Listener* m_listener{ nullptr };
    };
} // namespace SUX