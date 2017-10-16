#include "virtual_key_manager.h"

using namespace std;


wxString VirtualMaster::GetStringFromIndex (const unsigned int& index)
{
	return m_vector[index].second;
}

unsigned int VirtualMaster::GetSize()
{
	return m_vector.size();
}

unsigned int VirtualMaster::GetIndexFromValue (const unsigned int& mod)
{
	unsigned int count = 0;
	vector<pair<unsigned int, wxString> >::iterator it = m_vector.begin();

	while (it->first != mod)
	{
		++count;
		++it;
	}

	if (it == m_vector.end() )
		count = 0;

	return count;
}

unsigned int VirtualMaster::GetValueFromIndex (const unsigned int& index)
{
	return m_vector[index].first;
}

unsigned int VirtualMaster::GetIndexFromString (const wxString& str_mod)
{
	unsigned int count = 0;
	vector<pair<unsigned int, wxString> >::iterator it = m_vector.begin();

	while (it->second != str_mod)
	{
		++count;
		++it;
	}

	if (it == m_vector.end() )
		count = 0;

	return count;
}

unsigned int VirtualMaster::GetValueFromString (const wxString& str_mod)
{
	unsigned int ret_value = 0;
	vector<pair<unsigned int, wxString> >::iterator it = m_vector.begin();

	while (it != m_vector.end() )
	{
		if (it->second == str_mod)
		{
			ret_value = it->first;
			break;
		}
		++it;
	}

	return ret_value;
}

wxString VirtualMaster::GetStringFromValue (const unsigned int& int_mod)
{
	wxString ret_str (_T ("Error modifier") );
	vector<pair<unsigned int, wxString> >::iterator it = m_vector.begin();

	while (it != m_vector.end() )
	{
		if (it->first == int_mod)
		{
			ret_str = it->second;
			break;
		}
		++it;
	}

	return ret_str;
}


VirtualModifierManager::VirtualModifierManager()
{
	m_vector.push_back (make_pair (0, _T ("-none-") ) );
	m_vector.push_back (make_pair (MOD_ALT, _T ("Alt") ) );
	m_vector.push_back (make_pair (MOD_CONTROL, _T ("Ctrl") ) );
	m_vector.push_back (make_pair (MOD_SHIFT, _T ("Shift") ) );
	m_vector.push_back (make_pair (MOD_WIN, _T ("Win") ) );
	m_vector.push_back (make_pair (10, _T ("Middle clic") ) );
	m_vector.push_back (make_pair (11, _T ("Right clic") ) );
}


VirtualKeyManager::VirtualKeyManager()
{
	m_vector.push_back (make_pair (0x25, _T ("Left") ) ); //VK_LEFT
	m_vector.push_back (make_pair (0x26, _T ("up") ) ); //VK_UP
	m_vector.push_back (make_pair (0x27, _T ("Right") ) ); //VK_RIGHT
	m_vector.push_back (make_pair (0x28, _T ("Down") ) ); //VK_DOWN
	m_vector.push_back (make_pair (0x60, _T ("Num Pad 0") ) );
	m_vector.push_back (make_pair (0x61, _T ("Num Pad 1") ) );
	m_vector.push_back (make_pair (0x62, _T ("Num Pad 2") ) );
	m_vector.push_back (make_pair (0x63, _T ("Num Pad 3") ) );
	m_vector.push_back (make_pair (0x64, _T ("Num Pad 4") ) );
	m_vector.push_back (make_pair (0x65, _T ("Num Pad 5") ) );
	m_vector.push_back (make_pair (0x66, _T ("Num Pad 6") ) );
	m_vector.push_back (make_pair (0x67, _T ("Num Pad 7") ) );
	m_vector.push_back (make_pair (0x68, _T ("Num Pad 8") ) );
	m_vector.push_back (make_pair (0x69, _T ("Num Pad 9") ) );
	m_vector.push_back (make_pair (0x41, _T ("A") ) );
	m_vector.push_back (make_pair (0x42, _T ("B") ) );
	m_vector.push_back (make_pair (0x43, _T ("C") ) );
	m_vector.push_back (make_pair (0x44, _T ("D") ) );
	m_vector.push_back (make_pair (0x45, _T ("E") ) );
	m_vector.push_back (make_pair (0x46, _T ("F") ) );
	m_vector.push_back (make_pair (0x47, _T ("G") ) );
	m_vector.push_back (make_pair (0x48, _T ("H") ) );
	m_vector.push_back (make_pair (0x49, _T ("I") ) );
	m_vector.push_back (make_pair (0x4A, _T ("J") ) );
	m_vector.push_back (make_pair (0x4B, _T ("K") ) );
	m_vector.push_back (make_pair (0x4C, _T ("L") ) );
	m_vector.push_back (make_pair (0x4D, _T ("M") ) );
	m_vector.push_back (make_pair (0x4E, _T ("N") ) );
	m_vector.push_back (make_pair (0x4F, _T ("O") ) );
	m_vector.push_back (make_pair (0x50, _T ("P") ) );
	m_vector.push_back (make_pair (0x51, _T ("Q") ) );
	m_vector.push_back (make_pair (0x52, _T ("R") ) );
	m_vector.push_back (make_pair (0x53, _T ("S") ) );
	m_vector.push_back (make_pair (0x54, _T ("T") ) );
	m_vector.push_back (make_pair (0x55, _T ("U") ) );
	m_vector.push_back (make_pair (0x56, _T ("V") ) );
	m_vector.push_back (make_pair (0x57, _T ("W") ) );
	m_vector.push_back (make_pair (0x58, _T ("X") ) );
	m_vector.push_back (make_pair (0x59, _T ("Y") ) );
	m_vector.push_back (make_pair (0x5A, _T ("Z") ) );
	m_vector.push_back (make_pair (0x30, _T ("0") ) );
	m_vector.push_back (make_pair (0x31, _T ("1") ) );
	m_vector.push_back (make_pair (0x32, _T ("2") ) );
	m_vector.push_back (make_pair (0x33, _T ("3") ) );
	m_vector.push_back (make_pair (0x34, _T ("4") ) );
	m_vector.push_back (make_pair (0x35, _T ("5") ) );
	m_vector.push_back (make_pair (0x36, _T ("6") ) );
	m_vector.push_back (make_pair (0x37, _T ("7") ) );
	m_vector.push_back (make_pair (0x38, _T ("8") ) );
	m_vector.push_back (make_pair (0x39, _T ("9") ) );
	m_vector.push_back (make_pair (0x21, _T ("Page Up") ) );
	m_vector.push_back (make_pair (0x22, _T ("Page Down") ) );
	m_vector.push_back (make_pair (0x2C, _T ("Prnt Src") ) );
}




