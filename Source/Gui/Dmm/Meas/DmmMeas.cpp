#include "DmmMeas.h"

#include <Source/Gui/MainWnd.h>
#include <math.h>
#include <Source/Gui/Oscilloscope/Meas/Statistics.h>

/*virtual*/ void CWndDmmMeas::Create(CWnd *pParent, ui16 dwFlags) 
{
	CWnd::Create("CWndMenuMeas", dwFlags | CWnd::WsListener, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_proDmmModes.Create( (const char**)CSettings::DmmSettings::ppszTextDmmMode,
		(NATIVEENUM*)&Settings.Dmm.Mode, CSettings::DmmSettings::_ModeMax );
	m_itmMode.Create("Mode", RGB565(808080), &m_proDmmModes, this);

	m_itmMeas[0].Create( &Settings.DmmMeas[0], this );
	m_itmMeas[1].Create( &Settings.DmmMeas[1], this );
	m_itmMeas[2].Create( &Settings.DmmMeas[2], this );
}

/*virtual*/ void CWndDmmMeas::OnMessage(CWnd* pSender, ui16 code, ui32 data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndDmm.ShowWindow( SwHide );
		return;
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndDmm.ShowWindow( SwShow );
		return;
	}

	// new waveform acquired, update the Y values 	
	if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
	{
		_UpdateAll();
		return;
	}

	// something was changed in listbox
	if (code == ToWord('u', 'p') )
	{
		_UpdateAll();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[0].m_pMeas )
			m_itmMeas[0].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[1].m_pMeas )
			m_itmMeas[1].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[2].m_pMeas )
			m_itmMeas[2].Invalidate();
		return;
	}

	// Show listbox - cursor details
	if (code == ToWord('m', 'c') )	
	{
		m_wndListMeas.Create( (CSettings::DmmMeasure*)(NATIVEPTR)data, this );
		m_wndListMeas.StartModal( &m_wndListMeas.m_itmEnabled );
		return;
	}
	if ( (code == ToWord('o', 'k') || code == ToWord('e', 'x') || code == ToWord('l', 'e')) )
	{
		// done
		m_wndListMeas.StopModal();
		return;
	}
}

CSettings::Measure::ESource convertSource(CSettings::DmmMeasure::ESource dmmSource) {
	switch(dmmSource) 
	{
	case CSettings::DmmMeasure::_CH1 :
		return CSettings::Measure::_CH1;
	case CSettings::DmmMeasure::_CH2 :
	default :
		return CSettings::Measure::_CH2;
	}
}


void CWndDmmMeas::_UpdateAll()
{
	CMeasStatistics m_Stat;
	// TODO autorange here
	for ( int nFilter = CSettings::DmmMeasure::_CH1; nFilter <= CSettings::DmmMeasure::_CH2; nFilter++ )
	{
		int nLastRange = -1;

		for ( int i = 0; i < (int)COUNT( m_itmMeas ); i++ )
		{
			CSettings::DmmMeasure& meas = Settings.DmmMeas[i];
			if ( meas.Enabled == CSettings::DmmMeasure::_Off )
				continue;
			if ( meas.Source != nFilter )
				continue;

			if ( nLastRange != CSettings::Measure::_View )
			{
				if ( !m_Stat.Process( convertSource((CSettings::DmmMeasure::ESource)nFilter), CSettings::Measure::_View ) )
				{
					continue;
				}
				nLastRange = CSettings::Measure::_View;
			}
			
			float fPrev = meas.fValue;
			meas.fValue = -1;
			switch ( meas.Type )
			{
				case CSettings::DmmMeasure::_Min:		meas.fValue = m_Stat.GetMin(); break;
				case CSettings::DmmMeasure::_Max:		meas.fValue = m_Stat.GetMax(); break;
				case CSettings::DmmMeasure::_Avg:		meas.fValue = m_Stat.GetAvg(); break;
				case CSettings::DmmMeasure::_Rms:		meas.fValue = m_Stat.GetRms(); break;
				case CSettings::DmmMeasure::_RectAvg:	meas.fValue = m_Stat.GetRectAvg(); break;
				case CSettings::DmmMeasure::_Vpp:		meas.fValue = m_Stat.GetVpp(); break;
				case CSettings::DmmMeasure::_Freq:		meas.fValue = m_Stat.GetFreq() / 1000.0f; break; // khz
				case CSettings::DmmMeasure::_Period:	meas.fValue = m_Stat.GetPeriod() * 1000.0f; break; // ms
				case CSettings::DmmMeasure::_FormFactor:
					meas.fValue = m_Stat.GetFormFactor(); break;
				case CSettings::DmmMeasure::_Sigma:	meas.fValue = m_Stat.GetSigma(); break;
				case CSettings::DmmMeasure::_Dispersion:
					meas.fValue = m_Stat.GetDispersion(); break;
				default:
					_ASSERT( !!!"Unknown measurement type" );
			}
			if ( fPrev != meas.fValue )
				m_itmMeas[i].OnPaint(false);
		}
	}
}

