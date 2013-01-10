#ifndef __DMMLISTMEAS_H__
#define __DMMLISTMEAS_H__

class CWndListDmmMeas : public CListBox
{
public:
	CSettings::DmmMeasure* m_pMeas;

	CProviderEnum	m_proEnabled;
	CProviderEnum	m_proSource;
	CProviderEnum	m_proType;

	CLPItem			m_itmEnabled;
	CLPItem			m_itmSource;
	CLPItem			m_itmType;

public:
	void Create( CSettings::DmmMeasure* pMeas, CWnd* pParent )
	{
		m_pMeas = pMeas;
		CListBox::Create( "Measure", WsVisible | WsModal, CRect(100, 30, 316, 160), RGB565(8080b0), pParent );

		m_proEnabled.Create( (const char**)CSettings::DmmMeasure::ppszTextEnabled,
			(NATIVEENUM*)&pMeas->Enabled, CSettings::DmmMeasure::_MaxEnabled );

		m_proSource.Create( (const char**)CSettings::DmmMeasure::ppszTextSource,
			(NATIVEENUM*)&pMeas->Source, CSettings::DmmMeasure::_MaxSource );

		m_proType.Create( (const char**)CSettings::DmmMeasure::ppszTextType,
			(NATIVEENUM*)&pMeas->Type, CSettings::DmmMeasure::_MaxType );

		m_itmEnabled.Create( "Enable", CWnd::WsVisible, &m_proEnabled, this );
		m_itmSource.Create( "Source", CWnd::WsVisible, &m_proSource, this );
		m_itmType.Create( "Type", CWnd::WsVisible, &m_proType, this );
	}
};

#endif