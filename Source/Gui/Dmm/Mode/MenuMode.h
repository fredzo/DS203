#ifndef __MENUMODE_H__
#define __MENUMODE_H__

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemMode.h"
#include "ListMode.h"

class CWndMenuMode : public CWnd
{
public:
	// Menu items
	CItemMode			m_itmMeas[3];
	
	CWndListMode		m_wndListMeas;

	virtual void		Create(CWnd *pParent, ui16 dwFlags);
	virtual void		OnMessage(CWnd* pSender, ui16 code, ui32 data);

private:
	void				_UpdateAll();
};

#endif