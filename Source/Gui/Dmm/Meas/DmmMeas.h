#ifndef __DMM_MEAS_H__
#define __DMM_MEAS_H__

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemMode.h"
#include "ItemParam.h"
#include "ItemMeas.h"
#include "ListMeas.h"

class CWndDmmMeas : public CWnd
{
public:
	// Menu items
	CProviderEnum		m_proDmmModes;
	CItemMode			m_itmMode;
	CItemDmmMeas		m_itmMeas[3];
	CProviderEnum		m_proDmmRange;
	CItemParam			m_itmRange;
	CProviderEnum		m_proDmmTime;
	CItemParam			m_itmTime;
	
	CWndListDmmMeas		m_wndListMeas;
	CComboSelector		m_wndComboSelector;

	virtual void		Create(CWnd *pParent, ui16 dwFlags);
	virtual void		OnMessage(CWnd* pSender, ui16 code, ui32 data);

	void				ConfigureAdc();
	void				UpdateMode();

private:
	void				_UpdateAll();
};

#endif