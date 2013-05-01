#ifndef __DMMITEMMEAS_H__
#define __DMMITEMMEAS_H__

class CItemDmmMeas : public CWndMenuItem
{
public:
	CSettings::DmmMeasure* m_pMeas;
	ui16	m_color;
	const char*   m_pShape;

public:

	virtual void OnPaint() 
	{
		bool bEnabled = m_pMeas->Enabled == CSettings::DmmMeasure::_On;
		ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);

		switch ( m_pMeas->Source )
		{
		case CSettings::DmmMeasure::_CH1: m_color = Settings.CH1.u16Color; break;
		case CSettings::DmmMeasure::_CH2: m_color = Settings.CH2.u16Color; break;
		}
		
		int x = m_rcClient.left + 2 + MarginLeft;
		int y = m_rcClient.top;
		if ( HasFocus() )
		{
			x -= 2;
		}
		CWndMenuItem::OnPaint();
		x += BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, m_pShape);
		const char* label = CSettings::getTypeText(Settings.Dmm.Mode)[ (int)m_pMeas->Type ];
		if ( HasFocus() )
		{
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_left);
			
			CRect rcRect(x, y, x + ((ui16)strlen(label) << 3), y + 14);
			BIOS::LCD::Bar( rcRect, RGB565(0000000) );
			BIOS::LCD::Print( x, y, RGB565(FFFFFF), RGBTRANS, label );
			x = rcRect.right;
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_right);
		}
		else
		{
			x+=3;
			BIOS::LCD::Print( x, y, clr, RGBTRANS, label );
		}
	}

	virtual void Create(CSettings::DmmMeasure* pMeas, CWnd *pParent, const char* shape) 
	{
		_ASSERT( pMeas );
		m_pMeas = pMeas;
		m_color = Settings.CH1.u16Color;
		m_pShape = shape;
		CWndMenuItem::Create( NULL, RGB565(000000), 1, pParent);
		CWndMenuItem::SetColorPtr( &m_color );
	}

	void Update(CSettings::DmmMeasure* pMeas)
	{
		_ASSERT( pMeas );
		m_pMeas = pMeas;
	}

	virtual void OnKey(ui16 nKey)
	{
		_ASSERT( m_pMeas );
		if ( nKey & BIOS::KEY::KeyLeft && m_pMeas->Type > 0 )
		{
			_ASSERT(sizeof(m_pMeas->Type) == sizeof(NATIVEENUM));
			DecEnum(m_pMeas->Type);
			Invalidate();
		}
		if ( nKey & BIOS::KEY::KeyRight && m_pMeas->Type < CSettings::getTypeMaxEnum(Settings.Dmm.Mode) )
		{
			_ASSERT(sizeof(m_pMeas->Type) == sizeof(NATIVEENUM));
			IncEnum(m_pMeas->Type);
			Invalidate();
		}
		if ( nKey & BIOS::KEY::KeyEnter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pMeas);
			return;
		}
		CWnd::OnKey( nKey );
	}
};

#endif