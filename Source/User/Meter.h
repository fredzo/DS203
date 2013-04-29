//#define ENABLE_MODULE_METER

#if defined(ADD_MODULE) && defined(ENABLE_MODULE_METER)
	ADD_MODULE( "Meter", CWndUserMeter )
#endif

#if !defined(__METER_H__) && defined(ENABLE_MODULE_METER)
#define __METER_H__

#include <Source/Framework/Wnd.h>

class CWndUserMeter : public CWnd
{
public:
	float m_fAverage, m_fVariance;
	bool bRefresh;
	bool bTimer;

	CWndUserMeter()
	{
		bRefresh = true;
		m_fAverage = 0;
		bTimer = false;
	}
	
	virtual void Create(CWnd *pParent, ui16 dwFlags)
	{
		CWnd::Create("CWndUserMeter", dwFlags | CWnd::WsListener | CWnd::WsNoActivate, CRect(0, 16, 400, 240), pParent);
	}

	void DrawDigit(int x, int y, int n, ui16 clrOn, ui16 clrOff );
	void DrawDigitH(int x, int y, ui16 clr );
	void DrawDigitV(int x, int y, ui16 clr );
	void DrawDot(int x, int y, ui16 clr );
	void OnWave();

	virtual void OnPaint();
	virtual void OnTimer();

	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
	{
		// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
		if (code == ToWord('L', 'D') )
		{
			bTimer = false;
			KillTimer();
			return;
		}

		if (code == ToWord('L', 'E') )
		{
			bRefresh = true;
			return;
		}

		if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
		{
			OnWave();
			return;
		}
	}

};

#endif