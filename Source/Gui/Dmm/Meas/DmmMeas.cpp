#include "DmmMeas.h"

#include <Source/Gui/MainWnd.h>
#include <math.h>
#include <Source/Gui/Oscilloscope/Meas/Statistics.h>

/*virtual*/ void CWndDmmMeas::Create(CWnd *pParent, ui16 dwFlags) 
{
	CWnd::Create("CWndMenuMeas", dwFlags | CWnd::WsListener, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_proDmmModes.Create( (const char**)CSettings::DmmSettings::ppszTextDmmMode,
		(NATIVEENUM*)&Settings.Dmm.Mode, CSettings::DmmSettings::_ModeMax );
	m_itmMode.Create(NULL, RGB565(808080), &m_proDmmModes, this);

	m_itmMeas[0].Create( &Settings.DmmMeas[0][Settings.Dmm.Mode], this, CShapes::m );
	m_itmMeas[1].Create( &Settings.DmmMeas[1][Settings.Dmm.Mode], this, CShapes::one );
	m_itmMeas[2].Create( &Settings.DmmMeas[2][Settings.Dmm.Mode], this, CShapes::two );

	m_proDmmRange.Create( (const char**)CSettings::DmmSettings::ppszTextDmmRange,
		(NATIVEENUM*)&Settings.Dmm.Range, CSettings::DmmSettings::_DmmRangeMax );
	m_itmRange.Create(CItemParam::_Range, RGB565(808080), &m_proDmmRange, this);
	m_proDmmTime.Create( (const char**)CSettings::DmmSettings::ppszTextDmmTime,
		(NATIVEENUM*)&Settings.Dmm.Time, CSettings::DmmSettings::_DmmTimeMax );
	m_itmTime.Create(CItemParam::_Time, RGB565(808080), &m_proDmmTime, this);
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
		ConfigureAdc();
		_UpdateAll();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[0].m_pMeas )
			m_itmMeas[0].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[1].m_pMeas )
			m_itmMeas[1].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[2].m_pMeas )
			m_itmMeas[2].Invalidate();
		return;
	}

	// Mode changed
	if (code == ToWord('u', 'm') )
	{
		UpdateMode();
		ConfigureAdc();
		_UpdateAll();
		return;
	}

	// Show listbox - measure details
	if (code == ToWord('m', 'c') )	
	{
		m_wndListMeas.Create( (CSettings::DmmMeasure*)(NATIVEPTR)data, this );
		m_wndListMeas.StartModal( &m_wndListMeas.m_itmEnabled );
		return;
	}
	// Mode / timebase / range
	if (code == ToWord('m', 'e') )	// more timebase
	{
		m_wndComboSelector.Create( "Select a value", WsVisible | WsModal, 
			CRect( 30, 100, 370, 140), RGB565(ffffff), (CValueProvider*)(NATIVEPTR)data, this);
		m_wndComboSelector.StartModal();
	}


	if ( (code == ToWord('o', 'k') || code == ToWord('e', 'x') || code == ToWord('l', 'e')) )
	{
		// done
		if(pSender == &m_wndListMeas || pSender->m_pParent == &m_wndListMeas) {
			m_wndListMeas.StopModal();
		} else if(pSender == &m_wndComboSelector || pSender->m_pParent == &m_wndComboSelector) {
			m_wndComboSelector.StopModal();
			UpdateMode();
		}
		ConfigureAdc();
		_UpdateAll();
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

void CWndDmmMeas::UpdateMode()
{
	m_itmMeas[0].Update( &Settings.DmmMeas[0][Settings.Dmm.Mode]);
	m_itmMeas[1].Update( &Settings.DmmMeas[1][Settings.Dmm.Mode]);
	m_itmMeas[2].Update( &Settings.DmmMeas[2][Settings.Dmm.Mode]);
	m_itmMeas[0].Invalidate();
	m_itmMeas[1].Invalidate();
	m_itmMeas[2].Invalidate();
}

void CWndDmmMeas::ConfigureAdc()
{
	// Load coupling
	switch(Settings.Dmm.Mode) {
		case CSettings::DmmSettings::_VAC: 
		case CSettings::DmmSettings::_mAAC: 
			Settings.CH1.Coupling = CSettings::AnalogChannel::_AC;
			break;
		case CSettings::DmmSettings::_VDC: 
		case CSettings::DmmSettings::_mADC: 
		case CSettings::DmmSettings::_CONT:
		default :
			Settings.CH1.Coupling = CSettings::AnalogChannel::_DC;
			break;
	}
	Settings.CH2.Coupling = Settings.CH1.Coupling;
	// Load range
	switch(Settings.Dmm.Range) {
		case CSettings::DmmSettings::_DmmRange50mV:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_50mV;
			break;
		case CSettings::DmmSettings::_DmmRange100mV:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_100mV;
			break;
		case CSettings::DmmSettings::_DmmRange200mV:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_200mV;
			break;
		case CSettings::DmmSettings::_DmmRange500mV:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_500mV;
			break;
		case CSettings::DmmSettings::_DmmRange1V:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_1V;
			break;
		case CSettings::DmmSettings::_DmmRange2V:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_2V;
			break;
		case CSettings::DmmSettings::_DmmRangeAuto:
		case CSettings::DmmSettings::_DmmRange5V:
			Settings.CH1.Resolution = CSettings::AnalogChannel::_5V;
			break;
		case CSettings::DmmSettings::_DmmRange10V:
		default :
			Settings.CH1.Resolution = CSettings::AnalogChannel::_10V;
			break;
	}
	Settings.CH2.Resolution = Settings.CH1.Resolution;
	// Load timebase
	switch(Settings.Dmm.Time) {
		case CSettings::DmmSettings::_DmmTime200ns:
			Settings.Time.Resolution = CSettings::TimeBase::_200ns;
			break;
		case CSettings::DmmSettings::_DmmTime500ns:
			Settings.Time.Resolution = CSettings::TimeBase::_500ns;
			break;
		case CSettings::DmmSettings::_DmmTime1us:
			Settings.Time.Resolution = CSettings::TimeBase::_1us;
			break;
		case CSettings::DmmSettings::_DmmTime2us:
			Settings.Time.Resolution = CSettings::TimeBase::_2us;
			break;
		case CSettings::DmmSettings::_DmmTime5us:
			Settings.Time.Resolution = CSettings::TimeBase::_5us;
			break;
		case CSettings::DmmSettings::_DmmTime20us:
			Settings.Time.Resolution = CSettings::TimeBase::_20us;
			break;
		case CSettings::DmmSettings::_DmmTime50us:
			Settings.Time.Resolution = CSettings::TimeBase::_50us;
			break;
		case CSettings::DmmSettings::_DmmTime100us:
			Settings.Time.Resolution = CSettings::TimeBase::_100us;
			break;
		case CSettings::DmmSettings::_DmmTime200us:
			Settings.Time.Resolution = CSettings::TimeBase::_200us;
			break;
		case CSettings::DmmSettings::_DmmTime500us:
			Settings.Time.Resolution = CSettings::TimeBase::_500us;
			break;
		case CSettings::DmmSettings::_DmmTime1ms:
			Settings.Time.Resolution = CSettings::TimeBase::_1ms;
			break;
		case CSettings::DmmSettings::_DmmTime2ms:
			Settings.Time.Resolution = CSettings::TimeBase::_2ms;
			break;
		case CSettings::DmmSettings::_DmmTime5ms:
			Settings.Time.Resolution = CSettings::TimeBase::_5ms;
			break;
		case CSettings::DmmSettings::_DmmTimeAuto:
		case CSettings::DmmSettings::_DmmTime10ms:
			Settings.Time.Resolution = CSettings::TimeBase::_10ms;
			break;
		case CSettings::DmmSettings::_DmmTime20ms:
			Settings.Time.Resolution = CSettings::TimeBase::_20ms;
			break;
		case CSettings::DmmSettings::_DmmTime50ms:
			Settings.Time.Resolution = CSettings::TimeBase::_50ms;
			break;
		case CSettings::DmmSettings::_DmmTime100ms:
			Settings.Time.Resolution = CSettings::TimeBase::_100ms;
			break;
		case CSettings::DmmSettings::_DmmTime200ms:
			Settings.Time.Resolution = CSettings::TimeBase::_200ms;
			break;
		case CSettings::DmmSettings::_DmmTime500ms:
			Settings.Time.Resolution = CSettings::TimeBase::_500ms;
			break;
		case CSettings::DmmSettings::_DmmTime1s:
		default :
			Settings.Time.Resolution = CSettings::TimeBase::_1s;
			break;
	}
	// Reconfigure ADC
	CCoreOscilloscope::ConfigureAdc();
	// Update DMM display
	MainWnd.m_wndDmm.Invalidate();
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
			CSettings::DmmMeasure& meas = Settings.DmmMeas[i][Settings.Dmm.Mode];
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
			
			meas.fValue = -1;
			switch ( meas.Type )
			{
				case CSettings::DmmMeasure::_VacMin:		
				//case CSettings::DmmMeasure::_VdcMin:		
				//case CSettings::DmmMeasure::_AacMin:		
				//case CSettings::DmmMeasure::_AdcMin:		
					meas.fValue = m_Stat.GetMin(); 
					break;
				case CSettings::DmmMeasure::_VacMax:		
				//case CSettings::DmmMeasure::_VdcMax:		
				//case CSettings::DmmMeasure::_AacMax:		
				//case CSettings::DmmMeasure::_AdcMax:		
					meas.fValue = m_Stat.GetMax(); 
					break;
				case CSettings::DmmMeasure::_VacAvg:		
					meas.fValue = m_Stat.GetAvg(); 
					break;
				case CSettings::DmmMeasure::_VacRms:		
					meas.fValue = m_Stat.GetRms(); 
					break;
				case CSettings::DmmMeasure::_VacRectAvg:	
					meas.fValue = m_Stat.GetRectAvg(); 
					break;
				case CSettings::DmmMeasure::_VacVpp:		
					meas.fValue = m_Stat.GetVpp(); 
					break;
				case CSettings::DmmMeasure::_VacFreq:		
					meas.fValue = m_Stat.GetFreq() / 1000.0f; 
					break; // khz
				case CSettings::DmmMeasure::_VacPeriod:	
					meas.fValue = m_Stat.GetPeriod() * 1000.0f; 
					break; // ms
				case CSettings::DmmMeasure::_VacFormFactor:
					meas.fValue = m_Stat.GetFormFactor(); 
					break;
				case CSettings::DmmMeasure::_VacSigma:	
					meas.fValue = m_Stat.GetSigma(); 
					break;
				case CSettings::DmmMeasure::_VacDispersion:
					meas.fValue = m_Stat.GetDispersion(); 
					break;
				default:
					_ASSERT( !!!"Unknown measurement type" );
			}
		}
	}
}

