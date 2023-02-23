#pragma once 
#include <juce_gui_basics/juce_gui_basics.h>

#include <utility>

namespace SUX 
{ 
    class NestedComboBox : public juce::Component
    { 
    public:
        NestedComboBox() {
            addAndMakeVisible(&m_comboBox);
        }

        ~NestedComboBox() override = default;

        void paint(juce::Graphics& g) override { }

        void resized() override {
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


    protected:
        juce::ComboBox m_comboBox;
        std::vector<std::unique_ptr<juce::ComboBox>> m_submenus;
    };

    /// Holds some useful data related to subdirs, and can be used to show nested dirs in a combobox..
    struct DirectoryData
    {
        explicit DirectoryData(juce::String path, juce::String extension) : m_directory(std::move(path)), m_extension(std::move(extension)) {
            scan(m_directory, m_extension);
        }

        [[maybe_unused]] std::optional<juce::File> lookupResult(const juce::String& toLookup) {
            auto it = m_memoryMap.find(toLookup);
            if (it == m_memoryMap.end()) return {};
            return it->second;
        }

        [[maybe_unused]] std::optional<size_t> getIndexForItem(const juce::String& toLookup) {
            auto it = std::find(m_itemList.begin(), m_itemList.end(), toLookup);
            if(it == m_itemList.end()) return {};
            return std::distance(m_itemList.begin(), it);
        }

        [[maybe_unused]] bool exists(const juce::String& toLookup) noexcept {
            return std::find(m_itemList.begin(), m_itemList.end(), toLookup) != m_itemList.end();
        }

        [[maybe_unused]] bool existsInDirectory(const juce::String& directoryName, const juce::String& toLookup) {
            // First off, does the directory exist??
            auto it = m_options.find(directoryName);
            if(it == m_options.end()) return false;
            auto items = it->second;
            auto el = std::find(items.begin(), items.end(), toLookup);
            return el != items.end();
        }

        [[maybe_unused]] void rescan() {
            scan(m_directory, m_extension);
        }

        [[maybe_unused]] std::map<juce::String, juce::StringArray>& getOptions() noexcept { return m_options; }

        [[maybe_unused]] std::unordered_map<juce::String, juce::File>& getMemoryMap() noexcept { return m_memoryMap; }

        [[maybe_unused]] std::vector<juce::String>& getItemList() noexcept { return m_itemList; }

        [[maybe_unused]] juce::File getDirectory() noexcept { return m_directory; }

        [[maybe_unused]] juce::String getExtension() noexcept { return m_extension; }

    private:
        void scan(const juce::String& path, const juce::String& extension)
        {
            m_itemList.clear();
            m_options.clear();
            m_memoryMap.clear();
            std::map<juce::String, juce::File> sortedDirs;
            for(const auto& dir : juce::RangedDirectoryIterator(path, false, "*", juce::File::findDirectories)) {
                auto current = dir.getFile();
                sortedDirs[current.getFileNameWithoutExtension()] = current;
            }

            for(auto it = sortedDirs.begin(); it != sortedDirs.end(); ++it) {
                auto name = it->first;
                auto current= it->second;
                juce::StringArray optionList;
                for(const auto& item : juce::RangedDirectoryIterator(current, false, "*" + extension, juce::File::findFiles)) {
                    auto filename = item.getFile().getFileNameWithoutExtension();
                    m_memoryMap[filename] = item.getFile();
                    optionList.add(item.getFile().getFileNameWithoutExtension());
                    m_itemList.emplace_back(filename);
                }
                optionList.sort(true);
                m_options[name] = optionList;
            }
        }

        juce::String m_directory, m_extension;
        // unsorted, but has direct access to files
        std::unordered_map<juce::String, juce::File> m_memoryMap;
        // Sorted, holds display names for each file
        std::map<juce::String, juce::StringArray> m_options;
        // Flat lookup of display names
        std::vector<juce::String> m_itemList;
    };


    class DirectoryComboBox : public NestedComboBox 
    { 
    public: 
        explicit DirectoryComboBox(DirectoryData& directoryData) : m_directoryData(directoryData) {
            populate();
        }

        ~DirectoryComboBox() override = default;

        void rescan() noexcept {
            m_directoryData.rescan();
            populate();
        }

        void populate()
        {
            m_comboBox.clear(juce::dontSendNotification);
            auto& options = m_directoryData.getOptions();
            for(auto it = options.begin(); it != options.end(); ++it)
            {
                auto name = it->first;
                auto current = it->second;
                addSubMenu(name, current);
            }
        }

        [[maybe_unused]] std::optional<juce::File> lookupResult(const juce::String& toLookup) {
            return m_directoryData.lookupResult(toLookup);
        }

        [[maybe_unused]] std::optional<size_t> getIndexForItem(const juce::String& toLookup) {
            return m_directoryData.getIndexForItem(toLookup);
        }

        [[maybe_unused]] bool exists(const juce::String& toLookup) noexcept {
            return m_directoryData.exists(toLookup);
        }

    private:
        DirectoryData& m_directoryData;
    };
}
