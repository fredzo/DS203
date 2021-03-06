#include "Application.h"
#include <Source/Gui/MainWnd.h>
/*
CApplicationProto::CApplicationProto() {}
CApplicationProto::~CApplicationProto() {}
void CApplicationProto::Create() {}
void CApplicationProto::Destroy() {}
bool CApplicationProto::operator ()() { return false; }
*/

/*static*/ CApplication* CApplication::m_pInstance = NULL;

class CKeyProcessor
{
	ui16 keys;
	ui32 last;
	ui16 delay;
public:
	CKeyProcessor()
	{
		last = BIOS::SYS::GetTick();
		delay = 301;
	}
	void operator <<(const ui16& in)
	{
		if (keys == 0)
			delay = 301;
		keys = in;
	}
	void operator >>(ui16& out)
	{
		ui32 now = BIOS::SYS::GetTick();
		if (now-last > delay && keys)
		{
			out = keys;
			last = now;
			if ( delay == 301 )
				delay = 300;
			else if ( delay == 300 )
				delay = 100;
			else if (delay > 10)
				delay -= 10;
		}
		else
			out = 0;
	}
};

/* workaround class for the issue of global classes constructors being not called */

#define GLOBAL (*CInit::getInstance())

class CInit
{
public:
	CMainWnd m_wndMain;
	CSettings m_Settings;
	CKeyProcessor m_kp;

	static CInit* getInstance()
	{
		static CInit init;
		return &init;
	}
};

CApplication::CApplication()
{
	m_pInstance = this;
}

CApplication::~CApplication()
{
}

void CApplication::Create()
{
	BIOS::SYS::Init();
#ifdef _VERSION2
	BIOS::FAT::Init();
#endif
	GLOBAL.m_wndMain.Create();
	GLOBAL.m_wndMain.WindowMessage( CWnd::WmPaint );

	CCoreOscilloscope::ConfigureAdc();
	CCoreOscilloscope::ConfigureTrigger();
	CCoreGenerator::Update();

	BIOS::ADC::Restart();
}

void CApplication::Destroy()
{
}

bool CApplication::operator ()()
{
	static ui32 lLastTick = (ui32)-1;
	static ui16 lLastKeys = (ui16)-1;
	ui32 lCurTick = BIOS::SYS::GetTick();
	if ( lLastTick == (ui32)-1 )
		lLastTick = lCurTick;
	if ( lCurTick - lLastTick > 1000 )
	{
		int nSeconds = (lCurTick - lLastTick) / 1000;
		lLastTick += nSeconds * 1000;
		GLOBAL.m_Settings.Runtime.m_nUptime += nSeconds;
	}

	ui16 nKeys = BIOS::KEY::GetKeys();
	if(!nKeys)
	{
		lLastKeys = 0;
	}

	GLOBAL.m_kp << nKeys;
	GLOBAL.m_kp >> nKeys;

	if ( nKeys )
	{
		if ( nKeys != lLastKeys && Settings.Runtime.m_Beep == CSettings::CRuntime::_On ) 
			BIOS::SYS::Beep(50);

		GLOBAL.m_wndMain.WindowMessage( CWnd::WmKey, nKeys );
		lLastKeys = nKeys;
	}

	GLOBAL.m_wndMain.WindowMessage( CWnd::WmTick, 0 );

#ifdef _WIN32
	// Sleep 1 ms is too long => divide this value by 10 to have something that approaches the quad's behaviour
	static int count = 0;
    if (count >= 10) { // Kind of like sleeping for 0.1 milliseconds
        Sleep(1);
        count = 0;
    }
    count++;
#endif
	return true;
}

/*static*/ CApplication* CApplication::getInstance()
{
	return m_pInstance;	
}
