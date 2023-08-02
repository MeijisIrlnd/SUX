#pragma once 
#include "VerboseComboBox.h"
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

        void paint(juce::Graphics& /*g*/) override { }

        void resized() override {
            m_comboBox.setBounds(getLocalBounds());
        }


        void addSubMenu(const juce::String& name, const juce::StringArray& opts) 
        { 
            auto offset = m_comboBox.getNumItems() + 1;
            juce::PopupMenu current;
            for(auto opt = 0; opt < opts.size(); opt++) { 
                current.addItem(opt + offset, opts[opt], true, false);
                m_idLookup[opt + offset] = { name, opts[opt] };
            }
            m_comboBox.getRootMenu()->addSubMenu(name, current);
        }
        juce::ComboBox* getComboBox() { return &m_comboBox; }


        [[nodiscard]] std::optional<std::tuple<juce::String, juce::String> > operator[](int id) {
            if(m_idLookup.find(id) == m_idLookup.end()) {
                return {};
            }
            return m_idLookup[id];
        }

        [[nodiscard]] std::optional<int> operator[](const std::tuple<juce::String, juce::String>& item) {
            const auto& bankToFind = std::get<0>(item);
            const auto& presetToFind = std::get<1>(item);
            const auto it = std::find_if(m_idLookup.begin(), m_idLookup.end(), [&] (const std::pair<int, std::tuple<juce::String, juce::String> >& el) {
                const auto& [currentBank, currentPreset] = el.second;
                return currentBank == bankToFind && currentPreset == presetToFind;
            });
            if(it == m_idLookup.end()) return { };
            return it->first;
        }

    protected:
        VerboseComboBox m_comboBox;
        /*juce::ComboBox m_comboBox;*/
        std::vector<std::unique_ptr<juce::ComboBox>> m_submenus;
        std::unordered_map<int, std::tuple<juce::String, juce::String> > m_idLookup;
    };

    // TODO: move this to a different header
    /// Holds some useful data related to subdirs, and can be used to show nested dirs in a combobox..
    struct DirectoryData
    {
        struct TargetDirectory {
            juce::String path;
            bool childDirs;
        };
        explicit DirectoryData(juce::String path, std::vector<juce::String> extension) : m_directory(std::move(path)), m_extension(std::move(extension)), m_multiDirMode(false) {
            scan(m_directory, m_extension);
        }

        explicit DirectoryData(std::vector<TargetDirectory> paths, std::vector<juce::String> extension) : m_directories(std::move(paths)), m_extension(std::move(extension)), m_multiDirMode(true) {
            scan(m_directories, m_extension);
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
            if(m_directories.empty()){ scan(m_directory, m_extension); }
            else { scan(m_directories, m_extension); }
        }

        [[maybe_unused]] std::map<juce::String, juce::StringArray>& getOptions() noexcept { return m_options; }

        [[maybe_unused]] std::unordered_map<juce::String, juce::File>& getMemoryMap() noexcept { return m_memoryMap; }

        [[maybe_unused]] std::vector<juce::String>& getItemList() noexcept { return m_itemList; }

        [[maybe_unused]] juce::File getDirectory() noexcept { return m_directory; }

        [[maybe_unused]] juce::File getDirectory(size_t index) noexcept { return m_directories[index].path; }

        [[nodiscard]] [[maybe_unused]] std::vector<TargetDirectory> getDirectories() noexcept { return m_directories; }

        [[maybe_unused]] std::vector<juce::String> getExtensions() noexcept { return m_extension; }

        [[nodiscard]] [[maybe_unused]] bool getMultiDirMode() const noexcept { return m_multiDirMode; }
    private:
        // for handling a single path
        void scan(const juce::String& path, const std::vector<juce::String>& extensions)
        {
            m_itemList.clear();
            m_options.clear();
            m_memoryMap.clear();
            auto extensionString = formatExtensionString(extensions);
            std::map<juce::String, juce::File> sortedDirs;
            for(const auto& dir : juce::RangedDirectoryIterator(path, false, "*", juce::File::findDirectories)) {
                auto current = dir.getFile();
                sortedDirs[current.getFileNameWithoutExtension()] = current;
            }

            for(auto it = sortedDirs.begin(); it != sortedDirs.end(); ++it) {
                auto name = it->first;
                auto current= it->second;
                juce::StringArray optionList;
                for(const auto& item : juce::RangedDirectoryIterator(current, false, extensionString, juce::File::findFiles)) {
                    auto filename = item.getFile().getFileNameWithoutExtension();
                    m_memoryMap[filename] = item.getFile();
                    optionList.add(item.getFile().getFileNameWithoutExtension());
                    m_itemList.emplace_back(filename);
                }
                optionList.sort(true);
                m_options[name] = optionList;
            }
        }

        // For handling multiple paths
        void scan(std::vector<TargetDirectory>& paths, const std::vector<juce::String>& extensions) {
            m_itemList.clear();
            m_options.clear();
            m_memoryMap.clear();
            juce::String extensionString = formatExtensionString(extensions);

            std::map<juce::String, juce::File> sortedDirs;
            for(auto& p : paths) {
                if(p.childDirs) {
                    for (const auto &dir: juce::RangedDirectoryIterator(p.path, false, "*", juce::File::findDirectories)) {
                        auto current = dir.getFile();
                        sortedDirs[current.getFileNameWithoutExtension()] = current;
                    }
                    for (auto it = sortedDirs.begin(); it != sortedDirs.end(); ++it) {
                        auto name = it->first;
                        auto current = it->second;
                        juce::StringArray optionList;
                        for (const auto &item: juce::RangedDirectoryIterator(current, false, extensionString,
                                                                             juce::File::findFiles)) {
                            auto filename = item.getFile().getFileNameWithoutExtension();
                            m_memoryMap[filename] = item.getFile();
                            optionList.add(item.getFile().getFileNameWithoutExtension());
                            m_itemList.emplace_back(filename);
                        }
                        optionList.sort(true);
                        m_options[name] = optionList;
                    }
                }
                else {
                    juce::StringArray optionList;
                    for(auto& f : juce::RangedDirectoryIterator(juce::File{p.path}, false, extensionString, juce::File::findFiles)) {
                        auto name = f.getFile().getFileNameWithoutExtension();
                        if(m_memoryMap.find(name) == m_memoryMap.end()) {
                            m_memoryMap[name] = f.getFile();
                        }
                        optionList.add(f.getFile().getFileNameWithoutExtension());
                        m_itemList.emplace_back(name);
                    }
                    optionList.sort(true);
                    m_options[juce::File{p.path}.getFileNameWithoutExtension()] = optionList;
                }
            }
        }


        [[nodiscard]] juce::String formatExtensionString(const std::vector<juce::String>& extensions) {
            juce::String extensionString;
            for(size_t i = 0; i < extensions.size(); ++i) {
                extensionString += "*" + extensions[i];
                if(i != extensions.size() - 1) {
                    extensionString += ";";
                }
            }
            return extensionString;
        }
        bool m_multiDirMode{ false };

        std::vector<TargetDirectory> m_directories;
        juce::String m_directory;
        std::vector<juce::String> m_extension;
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
