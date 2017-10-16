#ifndef __VIRUTAL_KEY_MENAGER_H__
#define __VIRUTAL_KEY_MENAGER_H__

#include <vector>
#include <utility>


class VirtualMaster
{
protected:
	std::vector<std::pair<unsigned int, wxString> > m_vector;
public:
	VirtualMaster() : m_vector() {}
	~VirtualMaster() {}

	unsigned int GetSize();
	unsigned int GetIndexFromString (const wxString& str_mod);
	unsigned int GetIndexFromValue (const unsigned int& mod);
	unsigned int GetValueFromIndex (const unsigned int& index);
	wxString GetStringFromIndex (const unsigned int& index);

	unsigned int GetValueFromString (const wxString& str_mod);
	wxString GetStringFromValue (const unsigned int& int_mod);
};


class VirtualModifierManager: public VirtualMaster
{
public:
	VirtualModifierManager();
	~VirtualModifierManager() {}
};


class VirtualKeyManager: public VirtualMaster
{
public:
	VirtualKeyManager();
	~VirtualKeyManager() {}
};

#endif // __VIRUTAL_KEY_MENAGER_H__


