#ifndef __DMM_H__
#define __DMM_H__

#include <Source/Framework/Wnd.h>
#include <Source/Gui/Dmm/Mode/MenuMode.h>


class CWndDmm : public CWnd
{
public:
	static const ui16 cLabel = RGB565(bbbbbb);
	static const ui16 cOn = RGB565(11bbff);
	static const ui16 cOff = RGB565(050505);
	static const ui16 cClr = RGB565(000000);
	static const int  valuesNumber = 3;

	float m_fAverage, m_fVariance;
	bool bRefresh;
	bool bTimer;
	int values[valuesNumber][6];

	CWndDmm()
	{
		bRefresh = true;
		m_fAverage = 0;
		bTimer = false;
	}
	
	virtual void Create(CWnd *pParent, ui16 dwFlags)
	{
		CWnd::Create("CWndUserDmm", dwFlags | CWnd::WsListener | CWnd::WsNoActivate, CRect(0, 16, 320-CWndMenuItem::MarginLeft, 240), pParent);
	}

	void DisplayValue(float value, bool isErr, int position, int type, bool redraw);
	void DrawDigit(int x, int y, int width, int size, int space, int n, ui16 clrOn, ui16 clrOff );

	virtual void OnPaint();

	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
	{
		// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
		if ((code == ToWord('L', 'D') )||(code == ToWord('S', 'H') && data == 0))
		{	// TODO restore channel settings
			bTimer = false;
			KillTimer();
			return;
		}

		if ((code == ToWord('L', 'E') )||(code == ToWord('S', 'H') && data == 1 ))
		{	// TODO backup channel settings
			bRefresh = true;
			return;
		}

		if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
		{
			Invalidate();
			return;
		}
	}

};

#endif