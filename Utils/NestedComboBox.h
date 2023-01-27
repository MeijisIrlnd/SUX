#pragma once 
#include <juce_gui_basics/juce_gui_basics.h>

namespace SUX 
{ 
    class NestedComboBox : public juce::Component
    { 
    public:
        NestedComboBox() 
        { 
            addAndMakeVisible(&m_comboBox);
        }

        virtual ~NestedComboBox() override 
        { 

        }

        void paint(juce::Graphics& g) override 
        { 

        }

        void resized() override
        { 
            m_comboBox.setBounds(getLocalBounds());
        }

        void addSubMenu(const juce::String& name, const juce::StringArray& opts) 
        { 
            std::unique_ptr<juce::ComboBox> current(new juce::ComboBox());
            m_submenus.push_back(std::move(current));
            m_submenus.back()->addItemList(opts, 1);
            addToTree(name, m_submenus.back().get());
        }

        juce::ComboBox* getComboBox() { return &m_comboBox; }
    private:
        void addToTree(const juce::String& name, juce::ComboBox* toAdd) 
        { 
            auto root = m_comboBox.getRootMenu();
            root->addSubMenu(name, *toAdd->getRootMenu());
        }
        juce::ComboBox m_comboBox;
        std::vector<std::unique_ptr<juce::ComboBox>> m_submenus;
    };

    class DirectoryComboBox : public NestedComboBox 
    { 
    public: 
        /// @brief Expects wildcard for extension, eg "*.wav" or "*.mp3;*.wav"
        /// @param extension 
        DirectoryComboBox(const juce::String& extension) : NestedComboBox(), m_extension(extension)
        { 
        }

        DirectoryComboBox(const juce::File& root, const juce::String& extension) : NestedComboBox(), m_extension(extension) 
        { 
            setDirectory(root);
        }

        ~DirectoryComboBox() override 
        { 

        }

        void setDirectory(const juce::File& root) 
        { 
            for(auto dir : juce::RangedDirectoryIterator(root, false, "*", juce::File::findDirectories)) 
            { 
                auto current = dir.getFile();
                auto name = current.getFileNameWithoutExtension();
                juce::StringArray optionList;
                for(auto item : juce::RangedDirectoryIterator(current, false, m_extension, juce::File::findFiles)) { 
                    auto filename = item.getFile().getFileNameWithoutExtension();
                    m_memoryMap[filename] = item.getFile();
                    optionList.add(item.getFile().getFileNameWithoutExtension());
                }
                addSubMenu(name, optionList);
            }
        }

        juce::File lookupResult(const juce::String& toLookup)
        { 
            auto it = m_memoryMap.find(toLookup);
            if(it == m_memoryMap.end()) return juce::File();
            return it->second;
        }


    private:
        juce::String m_extension;
        // item -> subfolder
        std::unordered_map<juce::String, juce::File> m_memoryMap;
    };
}