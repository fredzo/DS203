#ifndef __DMM_H__
#define __DMM_H__

#include <Source/Framework/Wnd.h>
#include <Source/Gui/Dmm/Meas/DmmMeas.h>


class CWndDmm : public CWnd
{
public:
	static const ui16 cLabel = RGB565(bbbbbb);
	static const ui16 cOn = RGB565(11bbff);
	static const ui16 cOff = RGB565(050505);
	static const ui16 cClr = RGB565(000000);
	// Positions
	static const int cBargraphLeft = 34;
	static const int cBargraphTop = 200;

	int m_currentBargraph;
	bool m_bargraphPositive;
	int m_targetBargraph;
	int m_tickCounter;
	bool bRefresh;
	bool bTimer;
	int values[3][6];

	CWndDmm()
	{
		bRefresh = true;
		m_currentBargraph = 0;
		m_bargraphPositive = true;
		m_targetBargraph = 0;
		m_tickCounter = 0;
		bTimer = false;
	}
	
	virtual void Create(CWnd *pParent, ui16 dwFlags)
	{
		CWnd::Create("CWndUserDmm", dwFlags | CWnd::WsTick | CWnd::WsListener | CWnd::WsNoActivate, CRect(0, 16, 320-CWndMenuItem::MarginLeft, 240), pParent);
	}

	void DisplayValue(float value, bool isErr, int position, int type, bool redraw, bool refreshValue);
	void DrawDigit(int x, int y, int width, int size, int space, int n, ui16 clrOn, ui16 clrOff );
	void UpdateBargraphValue();
	void UpdateBargraphDisplay();
	void OnTick(bool force);

	void OnPaint(bool updateBg);

	virtual void OnPaint()
	{
		OnPaint(true);
	}

	virtual void OnTimer()
	{
		bTimer = false;
		KillTimer();
	}

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
			OnPaint(false);
			return;
		}

		if (code == ToWord('t', 'i') )
		{
			// OnTick
			OnTick(false);
			return;
		}
	}
};

#endif