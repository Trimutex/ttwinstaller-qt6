#pragma once

#include <string>

namespace TaleOfTwoWastelandsUI {
	//thanks Barry
	//http://stackoverflow.com/a/3529945
    // NOTE: sealed class
	class TextProgressBar : ProgressBar {
    public:
        std::string getCustomText(void);
        std::string setCustomText(void);

        TextProgressBar();

    protected:
        override void onPaint(PaintEventArgs e);

    private:
        // Property to hold the custom text
        std::string _m_customText;

        float m_valueF;
    }
}
