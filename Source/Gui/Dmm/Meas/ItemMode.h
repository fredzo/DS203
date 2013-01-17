#ifndef __MENUITEMMODE_H__
#define __MENUITEMMODE_H__

class CItemMode : public CWndMenuItem
{
protected:
	CValueProvider* m_pProvider;

public:
	void Create(const char* pszId, ui16 clr, CValueProvider* pProvider, CWnd* pParent)
	{
		m_pProvider = pProvider;
		CWndMenuItem::Create( pszId, clr, 1, pParent);
	}

	virtual void OnPaint()
	{
		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 2 + MarginLeft;
		int y = m_rcClient.top;
		if ( HasFocus() )
		{
			x -= 2;
		}
		x += BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_mode);
		x++;
		if ( HasFocus() )
		{
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_left);
			CRect rcRect(x, y, x + m_pProvider->GetWidth(), y + 14);
			BIOS::LCD::Bar( rcRect, RGB565(0000000) );
			m_pProvider->OnPaint( rcRect, HasFocus() );
			x = rcRect.right;
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_right);
		} else
		{
			x+=4;
			CRect rcRect(x, y, x + m_pProvider->GetWidth(), y + 14);
			m_pProvider->OnPaint( rcRect, HasFocus() );
			x = rcRect.right;
			x+=4;
		}
		switch(m_pProvider->Get()) {
			case CSettings::DmmSettings::_VAC: 
				BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_vac);
				break;
			case CSettings::DmmSettings::_VDC: 
				BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_vdc);
				break;
			case CSettings::DmmSettings::_CONT: 
				BIOS::LCD::Draw(x-2, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_cont);
				break;
		}
	}

	virtual void OnKey(ui16 nKey)
	{
		if ( nKey & BIOS::KEY::KeyLeft && (*m_pProvider)-1 == CValueProvider::Yes )
		{
			(*m_pProvider)--;
			Invalidate();
			SendMessage(m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey & BIOS::KEY::KeyRight && (*m_pProvider)+1 == CValueProvider::Yes )
		{
			(*m_pProvider)++;
			Invalidate();
			SendMessage(m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey & BIOS::KEY::KeyEnter  )
		{
			// is that provider enumerator ?
			if ( m_pProvider->Get() !=	CValueProvider::Invalid )
			{
				SendMessage(m_pParent, ToWord('l', 'e'), (ui32)(NATIVEPTR)(m_pProvider));
			}
		}
		CWndMenuItem::OnKey( nKey );
	}
};

#endif