//
// Created by Syl on 02/08/2023.
//

#ifndef COLLAGE_VERBOSECOMBOBOX_H
#define COLLAGE_VERBOSECOMBOBOX_H
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX {
    class VerboseComboBox : public juce::ComboBox {
    public:
        ~VerboseComboBox() override = default;
        void showPopup() override {
            DBG("HERE");
            juce::ComboBox::showPopup();
        }
    };
}
#endif //COLLAGE_VERBOSECOMBOBOX_H
