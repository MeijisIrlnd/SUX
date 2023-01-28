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
            auto offset = m_comboBox.getNumItems() + 1;
            juce::PopupMenu current;
            for(auto opt = 0; opt < opts.size(); opt++) { 
                current.addItem(opt + offset, opts[opt], true, false);
            }
            m_comboBox.getRootMenu()->addSubMenu(name, current);
        }
        juce::ComboBox* getComboBox() { return &m_comboBox; }


    private:
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