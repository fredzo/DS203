#ifndef __MENUITEMMEAS_H__
#define __MENUITEMMEAS_H__

class CItemMeas : public CWndMenuItem
{
public:
	CSettings::Measure* m_pMeas;
	ui16	m_color;

public:
	virtual void Create(CSettings::Measure* pMeas, CWnd *pParent) 
	{
		_ASSERT( pMeas );
		m_pMeas = pMeas;
		m_color = Settings.CH1.u16Color;
		CWndMenuItem::Create( NULL, RGB565(000000), 2, pParent);
		CWndMenuItem::SetColorPtr( &m_color );
	}

	void OnPaint(bool updateBg)
	{
		bool bEnabled = m_pMeas->Enabled == CSettings::Measure::_On;
		ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);
		ui16 clr2 = RGB565(404040);

		switch ( m_pMeas->Source )
		{
		case CSettings::Measure::_CH1: m_color = Settings.CH1.u16Color; break;
		case CSettings::Measure::_CH2: m_color = Settings.CH2.u16Color; break;
		case CSettings::Measure::_Math: m_color = Settings.Math.uiColor; break;
		}
		
		int x = m_rcClient.left + 10 + MarginLeft;
		int y = m_rcClient.top;

		if(updateBg)
		{
			CWndMenuItem::OnPaint();
			BIOS::LCD::Print( x, y, clr, RGBTRANS, CSettings::Measure::ppszTextType[ (int)m_pMeas->Type ] );
		}
		else
		{
			CWndMenuItem::ClearValueBottomBg();
		}

		if ( bEnabled )
		{
			y += 16;
			char str[16]; 
			const char* suffix = CSettings::Measure::ppszTextSuffix[ (int)m_pMeas->Type ];
			float fValue = m_pMeas->fValue;
			if ( fValue < 0 )
			{
				x += BIOS::LCD::Draw( x, y, clr, RGBTRANS, CShapes::minus);
				fValue = -fValue;
			} else
				x += 6;

			BIOS::DBG::sprintf(str, "%3f", fValue);
		
			while ( strlen(str) + strlen(suffix) > 7 )
				str[strlen(str)-1] = 0;
		
			x += BIOS::LCD::Print( x, y, clr, RGBTRANS, str );
			x += 4;
			if ( suffix && *suffix )
				BIOS::LCD::Print( x, y, clr2, RGBTRANS, suffix );
		}
	}

	virtual void OnPaint()
	{
		OnPaint(true);
	}

	virtual void OnKey(ui16 nKey)
	{
		_ASSERT( m_pMeas );
		if ( nKey & BIOS::KEY::KeyEnter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pMeas);
			return;
		}
		CWnd::OnKey( nKey );
	}

};

#endif