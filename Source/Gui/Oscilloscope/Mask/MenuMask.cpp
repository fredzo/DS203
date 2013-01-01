#include "MenuMask.h"

#include <Source/Gui/MainWnd.h>

PCSTR m_strStart = "Start\ncollect";
PCSTR m_strStop = "Stop\ncollect";

int BlurFilter( int a, int b, int c )
{
	if ( a < b && c < b )
		return (a + b + c)/3;
	if ( a < b )
		return (a + b)/2;
	if ( c < b )
		return (c + b)/2;
	return b;
}

#define BlurFilterInv(a, b, c) (255-BlurFilter(255-(a), 255-(b), 255-(c)))

/*static*/ const char* const CWndMenuMask::m_ppszTextAction[] =
	{"None", "Beep", "BeepStp", "Stop"};

/*static*/ const char* const CWndMenuMask::m_ppszDispAction[] =
	{"No", "Yes"};

/*virtual*/ void CWndMenuMask::Create(CWnd *pParent, ui16 dwFlags) 
{
	m_Action = ActionNone;
	m_Display = DisplayNo;

	CWnd::Create("CWndMenuMask", dwFlags | CWnd::WsListener, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);

	m_btnSource.Create( "Source\nCH1", RGB565(ffff00), 2, this );
	m_btnReset.Create( "Reset\nmask", RGB565(8080ff), 2, this );
	m_btnCollect.Create( m_strStart, RGB565(8080ff), 2, this );

	m_itmAction.Create( "Action", RGB565(ffffff), &m_proAction, this );
	m_proAction.Create( (const char**)m_ppszTextAction, (NATIVEENUM*)&m_Action, ActionMax );

	m_btnExpand.Create( "\x11" "Expand\x10", RGB565(8080ff), 1, this );
	m_btnBlur.Create( "\x11 Blur \x10", RGB565(8080ff), 1, this );

	m_itmDisplay.Create( "Display", RGB565(ffffff), &m_proDisplay, this );
	m_proDisplay.Create( (const char**)m_ppszDispAction, (NATIVEENUM*)&m_Display, DisplayMax );
}

/*virtual*/ void CWndMenuMask::OnMessage(CWnd* pSender, ui16 code, ui32 data)
{
	if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
	{
		if ( m_btnCollect.m_pszId != m_strStart )
		{
			CSettings::Calibrator::FastCalc Ch1fast;
			Settings.CH1Calib.Prepare( &Settings.CH1, Ch1fast );

			for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
			{
				ui8* bLow = NULL;
				ui8* bHigh = NULL;
				CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
		
				BIOS::ADC::SSample Sample;
				Sample.nValue = BIOS::ADC::GetAt( Settings.Time.Shift + i );

				si16 ch1 = Sample.CH1;
				ch1 = Settings.CH1Calib.Correct( Ch1fast, ch1 );
				UTILS.Clamp<si16>( ch1, 0, 255 );
			
				*bLow = min(*bLow, (ui8)ch1);
				*bHigh = max(*bHigh, (ui8)ch1);
			}
		}
		if ( m_Action != ActionNone || m_Display != DisplayNo )
		{
			bool bFailure = false;
			CSettings::Calibrator::FastCalc Ch1fast;
			Settings.CH1Calib.Prepare( &Settings.CH1, Ch1fast );

			for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
			{
				ui8* bLow = NULL;
				ui8* bHigh = NULL;
				CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
		
				BIOS::ADC::SSample Sample;
				Sample.nValue = BIOS::ADC::GetAt( Settings.Time.Shift + i );

				si16 ch1 = Sample.CH1;
				ch1 = Settings.CH1Calib.Correct( Ch1fast, ch1 );
				UTILS.Clamp<si16>( ch1, 0, 255 );

				if ( ch1 < *bLow || ch1 > *bHigh )
				{
					bFailure = true;
					break;
				}
			}

			int* nPass = NULL;
			int* nFail = NULL;
			CCoreOscilloscope::GetMaskStats(&nPass, &nFail);

			if ( !bFailure )
				(*nPass)++;
			else
			{
				(*nFail)++;
				switch ( m_Action )
				{
				case ActionNone:
					break;

				case ActionBeep:
					BIOS::SYS::Beep(100);
					break;

				case ActionBeepStop:
					BIOS::SYS::Beep(100);
					Settings.Trig.State = CSettings::Trigger::_Stop;
					BIOS::ADC::Enable( false );
					MainWnd.m_wndMessage.Show(&MainWnd, "Information", "Trigger was paused", RGB565(ffff00));
					break;

				case ActionStop:
					Settings.Trig.State = CSettings::Trigger::_Stop;
					BIOS::ADC::Enable( false );
					MainWnd.m_wndMessage.Show(&MainWnd, "Information", "Trigger was paused", RGB565(ffff00));
					break;
				}
			}
		}
		return;
	}

	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow( SwHide );
		MainWnd.m_wndInfoBar.ShowWindow( SwHide );
	}

	if (code == ToWord('L', 'E') )
	{
		m_Action = ActionNone;
		MainWnd.m_wndGraph.ShowWindow( SwShow );
		MainWnd.m_wndInfoBar.ShowWindow( SwShow );
		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
			*bLow = 32;
			*bHigh = 255-32;
		}
	}

	if ( pSender == &m_btnReset && code == CWnd::WmKey && data == BIOS::KEY::KeyEnter )
	{
		CSettings::Calibrator::FastCalc Ch1fast;
		Settings.CH1Calib.Prepare( &Settings.CH1, Ch1fast );

		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
		
			BIOS::ADC::SSample Sample;
			Sample.nValue = BIOS::ADC::GetAt( Settings.Time.Shift + i );

			si16 ch1 = Sample.CH1;
			ch1 = Settings.CH1Calib.Correct( Ch1fast, ch1 );
			UTILS.Clamp<si16>( ch1, 0, 255 );
			*bLow = (ui8)ch1;
			*bHigh = (ui8)ch1;
		}
	}

	if ( pSender == &m_btnExpand && code == CWnd::WmKey && data == BIOS::KEY::KeyRight )
	{
		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
			
			if ( *bLow > 0 ) 
				(*bLow)--;
			if ( *bHigh < 255 ) 
				(*bHigh)++;
		}
	}

	if ( pSender == &m_btnExpand && code == CWnd::WmKey && data == BIOS::KEY::KeyLeft )
	{
		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
			
			if ( *bLow < 255 && *bLow < *bHigh ) 
				(*bLow)++;
			if ( *bHigh > 0 && *bHigh > *bLow ) 
				(*bHigh)--;
		}
	}

	if ( pSender == &m_btnCollect && code == CWnd::WmKey && data == BIOS::KEY::KeyEnter )
	{
		if ( m_btnCollect.m_pszId == m_strStart )
		{
			//m_dwFlags |= CWnd::WsListener;
			m_btnCollect.m_pszId = m_strStop;
			m_btnCollect.Invalidate();
		} else
		if ( m_btnCollect.m_pszId == m_strStop )
		{
			//m_dwFlags &= ~CWnd::WsListener;
			m_btnCollect.m_pszId = m_strStart;
			m_btnCollect.Invalidate();
		}
	}

	if ( pSender == &m_btnBlur && code == CWnd::WmKey && data == BIOS::KEY::KeyRight )
	{
		ui8 arrLow[3] = {0, 0, 0};
		ui8 arrHigh[3] = {255, 255, 25};
		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
			arrLow[0] = arrLow[1];
			arrLow[1] = arrLow[2];
			arrLow[2] = *bLow;

			arrHigh[0] = arrHigh[1];
			arrHigh[1] = arrHigh[2];
			arrHigh[2] = *bHigh;

			if (i >= 2)
			{
				CCoreOscilloscope::GetMaskAt( i-1, &bLow, &bHigh );
				*bLow = BlurFilter( arrLow[0], arrLow[1], arrLow[2] );
				*bHigh = BlurFilterInv( arrHigh[0], arrHigh[1], arrHigh[2] );
			}
		}
	}

	if ( pSender == &m_btnBlur && code == CWnd::WmKey && data == BIOS::KEY::KeyLeft )
	{
		ui8 arrLow[3] = {0, 0, 0};
		ui8 arrHigh[3] = {255, 255, 25};
		for ( int i = 0; i < CWndGraph::DivsX*CWndGraph::BlkX; i++ )
		{
			ui8* bLow = NULL;
			ui8* bHigh = NULL;
			
			CCoreOscilloscope::GetMaskAt( i, &bLow, &bHigh );
			arrLow[0] = arrLow[1];
			arrLow[1] = arrLow[2];
			arrLow[2] = *bLow;

			arrHigh[0] = arrHigh[1];
			arrHigh[1] = arrHigh[2];
			arrHigh[2] = *bHigh;

			if (i >= 2)
			{
				CCoreOscilloscope::GetMaskAt( i-1, &bLow, &bHigh );
				*bLow = BlurFilterInv( arrLow[0], arrLow[1], arrLow[2] );
				*bHigh = BlurFilter( arrHigh[0], arrHigh[1], arrHigh[2] );
			}
		}
	}
	if ( pSender == &m_itmDisplay && code == ToWord('l', 'e') )
	{
		int* nPass = NULL;
		int* nFail = NULL;
		CCoreOscilloscope::GetMaskStats( &nPass, &nFail );
		*nPass = 0;
		*nFail = 0;
	}

}


