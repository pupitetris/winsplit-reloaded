#ifndef __HOTKEYS_CONFIGURE_H__
#define __HOTKEYS_CONFIGURE_H__

#include <vector>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <windows.h>

#include "hotkeys_manager.h"
#include "virtual_key_manager.h"

class ChooseHotkeyDialog:public wxDialog
{
private:
    wxComboBox* p_cmbMod1;
    wxComboBox* p_cmbMod2;
    wxComboBox* p_cmbVirtKey;
    wxButton* p_btnOk;
    wxButton* p_btnCancel;
    std::vector<HotkeyStruct> vec_hk;
    int n_item;
    VirtualModifierManager m_mod_manager;
	VirtualKeyManager m_keys_manager;

public:

    ChooseHotkeyDialog(wxWindow* parent,
                        wxWindowID id = wxID_ANY,
                        const wxString& title = wxEmptyString);

    ~ChooseHotkeyDialog();

    void set_hotkeys(unsigned int modifier_1,unsigned int modifier_2,unsigned int virtual_key);
    HotkeyStruct GetHotkeys();
    void InitCombo();
    void CreateControls();
    void SetHotkeys(HotkeyStruct hk,int num_item);
    void OnButOk(wxCommandEvent &event);
    void SetVecHK(const std::vector<HotkeyStruct>& vec);
};


class HotkeyConfigureDialog:public wxDialog
{
private:
	wxListCtrl* p_listbox;
	wxButton* p_btnEdit;
	wxButton* p_btnActiv;
	ChooseHotkeyDialog dlgChooseHK;

	HotkeysManager* p_hkManager;

	int m_item_selected;
	bool m_modified;

	std::vector<HotkeyStruct> vec_tab_tmp;

	VirtualModifierManager m_mod_manager;
	VirtualKeyManager m_keys_manager;

	void OnButEdit(wxCommandEvent &event);
    void OnButActiv(wxCommandEvent &event);
    void OnTaskChangeSelection(wxListEvent& event);
    void OnTaskDoubleClick(wxListEvent& event);
    void InitList();
    void CreateControls();
    void CreateConnexions();
    void ShowDialogHK();

    int GetIndexVal(int* tab,int size,int val);
    wxString GetIndexStr(int val);
    void SetRegisterHK(HotkeysManager *p);

public:
	HotkeyConfigureDialog(HotkeysManager*);
    ~HotkeyConfigureDialog();

    std::vector<HotkeyStruct> GetVecTmpHK();
    bool IsModified();

};

#endif // __HOTKEYS_CONFIGURE_H__
