#pragma once

namespace TaleOfTwoWastelandsUI {
	//thanks Barry
	//http://stackoverflow.com/a/3529945
    // NOTE: sealed class
	class TextProgressBar : ProgressBar {
    public:
        string m_customText
        {
            get
            {
                return _customText;
            }
            set
            {
                if (_customText != value)
                {
                    _customText = value;
                    Invalidate();
                }
            }
        }

        TextProgressBar();

    protected:
        override void onPaint(PaintEventArgs e);

    private:
        //Property to hold the custom text
        string _m_customText;

        float m_valueF
        {
            get { return Value; }
        }


    }
}
