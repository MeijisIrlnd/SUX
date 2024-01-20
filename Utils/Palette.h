#pragma once 
#include <juce_core/juce_core.h>
namespace SUX
{
	struct Palette
	{
		Palette() = default;
		
		explicit Palette(const std::vector<juce::Colour>& colours_) : colours(colours_) { }
		

		Palette& operator=(const Palette& other) {
            if(&other != this) {
                colours = other.colours;
            }
            return *this;
		}

		Palette& operator=(Palette&& other) noexcept {
            if(&other != this) {
                colours = other.colours;
            }
            return *this;
		}

		static inline Palette fromString(const juce::String& data)
		{
			try {
				std::vector<juce::Colour> loaded;
				auto j = juce::JSON::fromString(data);
				auto arr = j.getProperty("colours", 0).getArray();
				for (auto i = 0; i < arr->size(); i++) {
					juce::String hexColour = "0xFF" + arr->operator[](i).toString();
					loaded.push_back(juce::Colour::fromString(hexColour));
				}
				return Palette(loaded);
			}
			catch (std::exception& /*e*/) {
				return {};
			}
		}

		//this sucks, but I prefer it to having to include different json.hs in different compilation units, this should do for now.. 
		static inline Palette fromFile(const juce::File& toLoad)
		{
			return fromString(juce::FileInputStream(toLoad).readEntireStreamAsString());
		}

		static inline Palette fromMemory(const void* data, int size)
		{
			return fromString(juce::String::fromUTF8((const char*)data, size));
		}


		inline juce::Colour getRandom() {
			juce::Random rand;
			return colours[static_cast<size_t>(rand.nextInt(juce::Range<int>(0, static_cast<int>(colours.size()))))];
		}

		std::vector<juce::Colour>::iterator begin() { return colours.begin(); }
		std::vector<juce::Colour>::iterator end() { return colours.end(); }

		juce::Colour operator[](size_t index) { return colours[index]; }
		std::vector<juce::Colour> colours;
	};
}