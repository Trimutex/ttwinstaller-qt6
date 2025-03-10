#pragma once

namespace TaleOfTwoWastelands.UI {
    // NOTE: partial class
    class frm_Main {
    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        override void dispose(bool disposing);

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System.ComponentModel.IContainer m_components = null;

    /* VESTIGIAL MACRO
     *  #region Windows Form Designer generated code
     */
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void initializeComponent();
    /* VESTIGIAL MACRO
     *  #endregion
     */

        System.Windows.Forms.TableLayoutPanel m_tbl_Main;
        System.Windows.Forms.PictureBox m_pic_Banner;
        System.Windows.Forms.RichTextBox m_txt_Progress;
        System.Windows.Forms.Button m_btn_Install;
        System.Windows.Forms.RichTextBox m_txt_Instructions;
        System.Windows.Forms.Button m_btn_FO3Browse;
        System.Windows.Forms.Button m_btn_FNVBrowse;
        System.Windows.Forms.Button m_btn_TTWBrowse;
        System.Windows.Forms.RichTextBox m_txt_FO3Location;
        System.Windows.Forms.RichTextBox m_txt_FNVLocation;
        System.Windows.Forms.RichTextBox m_txt_TTWLocation;
        System.Windows.Forms.Label m_label1;
        System.Windows.Forms.Label m_label2;
        System.Windows.Forms.Label m_label3;
        System.Windows.Forms.OpenFileDialog m_dlg_FindGame;
        System.Windows.Forms.SaveFileDialog m_dlg_SaveTTW;
        TextProgressBar m_prgCurrent;
        TextProgressBar m_prgOverall;
        System.Windows.Forms.CheckBox m_chkYou;
    }
}

