#ifndef __MENUITEMPARAM_H__
#define __MENUITEMPARAM_H__

class CItemParam : public CWndMenuItem
{
public:
	enum ParamType { _Range, _Time};

protected:
	CValueProvider* m_pProvider;
	ParamType m_paramType;

public:
	void Create(ParamType paramType, ui16 clr, CValueProvider* pProvider, CWnd* pParent)
	{
		m_pProvider = pProvider;
		m_paramType = paramType;
		CWndMenuItem::Create( NULL, clr, 1, pParent);
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
		switch(m_paramType) {
			case _Range: 
				x += BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_range);
				break;
			case _Time: 
				x += BIOS::LCD::Draw(x, y+4, RGB565(000000), RGBTRANS, CShapes::dmm_time);
				break;
		}
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
				SendMessage(m_pParent, ToWord('m', 'e'), (ui32)(NATIVEPTR)m_pProvider);
			}
		}
		CWndMenuItem::OnKey( nKey );
	}
};

#endif