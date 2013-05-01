#ifndef __DMM_H__
#define __DMM_H__

#include <Source/Framework/Wnd.h>
#include <Source/Gui/Dmm/Meas/DmmMeas.h>
#include <Source/Core/Settings.h>


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

	void DisplayValue(CSettings::DmmMeasure *pDmmMeas, bool isErr, int position, bool redraw, bool refreshValue);
	void DrawDigit(int x, int y, int width, int size, int space, int n, ui16 clrOn, ui16 clrOff );
	void UpdateBargraphValue(float fullRange);
	void UpdateBargraphDisplay();
	void OnTick(bool force);
	void computeFullRange(CSettings::DmmMeasure *pDmmMeas, float* pFullRange, int* pFullScale);

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

	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data);
};

#endif