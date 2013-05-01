#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define Settings (*CSettings::m_pInstance)
#include <Source/HwLayer/Types.h>
#include <Source/HwLayer/Bios.h>
#include "Serialize.h"

#define _VERSION ToDword('D', 'S', 'C', 12)

class CSettings : public CSerialize
{
public:
	static CSettings* m_pInstance;

public:
	class AnalogChannel : public CSerialize
	{
	public:
		static const char* const ppszTextEnabled[];
		static const char* const ppszTextCoupling[];
		static const char* const ppszTextResolution[];
		static const char* const ppszTextProbe[];

		enum eEnabled {_NO = 0, _YES = 1, _EnabledMax = _YES}
			Enabled;
		enum eCoupling {_AC = 0, _DC, _GND, _CouplingMax = _GND} 
			Coupling;
		enum eResolution {_50mV, _100mV, _200mV, _500mV, _1V, _2V, _5V, _10V, _ResolutionMax = _10V} 
			Resolution;
		enum eProbe { _1X = 0, _10X, _100X, _1000X, _ProbeMax = _1000X }
			Probe;

		static const float pfValueProbe[];
		static const float pfValueResolution[];
		

		ui16 u16Color;
		si16 u16Position;
		const char* pszName;
		const char* pszFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Coupling) << _E(Resolution) << _E(Probe) << u16Color << u16Position;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Coupling) >> _E(Resolution) >> _E(Probe) >> u16Color >> u16Position;
			return *this;
		}
	};
	class DigitalChannel : public CSerialize
	{
	public:
		static const char* const ppszTextPolarity[];

		enum {_NO = 0, _YES = 1, _EnabledMax = _YES}
			Enabled;
		ui16 u16Color;
		ui16 u16Position;
		enum {_POS=0, _NEG, _PolarityMax = _NEG}
			Polarity;

		const char* pszName;
		const char* pszFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Polarity) << u16Color << u16Position;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Polarity) >> u16Color >> u16Position;
			return *this;
		}
	};
	class TimeBase : public CSerialize
	{
	public:
		static const char* const ppszTextResolution[];
		enum EResolution { 
			/*_100ns,*/ _200ns, _500ns, 
			_1us, _2us, _5us,
			_10us, _20us, _50us, _100us, _200us, _500us,
			_1ms, _2ms, _5ms,
			_10ms, _20ms, _50ms, _100ms, _200ms, _500ms, 
			_1s, _ResolutionMax = _500ms }
			Resolution;
		enum {
			_1, _2, _4, _FULL}
			Range;

		static const float pfValueResolution[];
		static const int pfValueResolutionCorrection[]; // florian

		si16 Shift;
		int InvalidFirst;
		
		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Resolution) << _E(Range) << Shift; 
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Resolution) >> _E(Range) >> Shift; 
			return *this;
		}
	};

	class Trigger : public CSerialize
	{
	public:
		static const char* const ppszTextSync[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextType[];
		enum ESync { _Auto, _Norm, _Single, _Scan, _None, _SyncMax = _None }
			Sync;
		enum { _EdgeLH, _EdgeHL, _Pulse, _TypeMax = _EdgeHL }
			Type;
		enum { _CH1, _CH2, _CH3, _CH4, _Math, _SourceMax = _Math }
			Source;
		enum { _Run, _Stop, _Wait, _Unsync, _StateMax = _Unsync }
			State;
		si16 nLevel;
		si16 nTime;
		ui32 nLastChange;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Sync) << _E(Type) << _E(Source) << _E(State) << nLevel << nTime;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Sync) >> _E(Type) >> _E(Source) >> _E(State) >> nLevel >> nTime;
			return *this;
		}
	};
	class Generator : public CSerialize
	{
	public:
		static const char* const ppszTextWave[];
		enum { _Dc = 0, _SinHq, _SinLq, _Tri, _Saw, _Square, _Cardiac, _Volatile, _WaveMax = _Volatile }
			Wave;
		int nPsc;
		int nArr;
		int nCcr;
		int nScale; // 65536 - full
		int nOffset; // 0..65536

		int nDuty; // wanted duty cycle, real duty can be calculated from arr/ccr
		int nSamples;
		float nFrequency; 
		float fAmplitude; // 65536 -> full

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Wave) << nPsc << nArr << nScale;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Wave) >> nPsc >> nArr >> nScale;
			return *this;
		}
	};
	class Marker : public CSerialize
	{
	public:
		static const char* const ppszTextMode[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextDisplay[];
		static const char* const ppszTextFind[];

		enum { _Time, _Voltage }
			Type; // not necessary to serialize, cannot be changed by user
		enum { _Off, _On, _Auto, _ModeMaxTime = _On, _ModeMaxVoltage = _Auto }
			Mode;
		enum { _CH1, _CH2, _SourceMax = _CH2 }
			Source;
		enum { _Raw, _Physical, _DisplayMax = _Physical }
			Display;
		enum EFind { _MinFind, _AvgFind, _MaxFind, _FindMax = _MaxFind };

		int nValue;
		ui16 u16Color;

		PCSTR strName;
		PCSTR strFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Mode) << _E(Source) << _E(Display) << nValue;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Mode) >> _E(Source) >> _E(Display) >> nValue;
			return *this;
		}
	};
	class Measure : public CSerialize {
	public:
		static const char* const ppszTextEnabled[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextType[];
		static const char* const ppszTextSuffix[];
		static const char* const ppszTextRange[];

		enum { _Off, _On, _MaxEnabled = _On }
			Enabled;
		enum ESource { _CH1, _CH2, _Math, _MaxSource = _Math }
			Source; 
		enum { _Min, _Max, _Avg, _Rms, _RectAvg, _Vpp, _Freq, _Period, _Pwm, _FormFactor, _Sigma, _Dispersion, _Baud, _MaxType = _Baud }
			Type;
		enum ERange { _View, _Selection, _All, _MaxRange = _All }
			Range;
		
		float fValue;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Source) << _E(Type) << _E(Range);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Source) >> _E(Type) >> _E(Range);
			return *this;
		}
	};
	class DmmMeasure : public CSerialize {
	public:
		static const char* const ppszTextEnabled[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextTypeVac[];
		static const char* const ppszTextTypeVdc[];
		static const char* const ppszTextTypeCont[];
		static const char* const ppszTextTypeOhm[];
		static const char* const ppszTextTypeAac[];
		static const char* const ppszTextTypeAdc[];
		static const char* const ppszTextSuffixVac[];
		static const char* const ppszTextSuffixVdc[];
		static const char* const ppszTextSuffixCont[];
		static const char* const ppszTextSuffixOhm[];
		static const char* const ppszTextSuffixAac[];
		static const char* const ppszTextSuffixAdc[];

		enum { _Off, _On, _MaxEnabled = _On }
			Enabled;
		enum ESource { _CH1, _CH2, _MaxSource = _CH2 }
			Source; 
		enum VacType  { _VacMin, _VacMax, _VacAvg, _VacRms, _VacRectAvg, _VacVpp, _VacFreq, _VacPeriod, _VacFormFactor, _VacSigma, _VacDispersion, _VacMaxType = _VacDispersion };
		enum VdcType  { _VdcMin, _VdcMax, _VdcAvg, _VdcRms, _VdcRectAvg, _VdcVpp, _VdcFreq, _VdcPeriod, _VdcFormFactor, _VdcSigma, _VdcDispersion, _VdcMaxType = _VdcDispersion };
		enum ContType { _ContCont, _ContOhm, _ContMin, _ContMax, _ContMaxType = _ContMax };
		enum OhmType  { _OhmOhm, _OhmMin, _OhmMax, _OhmMaxType = _OhmMax };
		enum AacType  { _AacMin, _AacMax, _AacAvg, _AacRms, _AacRectAvg, _AacApp, _AacFreq, _AacPeriod, _AacMaxType = _AacPeriod };
		enum AdcType  { _AdcMin, _AdcMax, _AdcAvg, _AdcRms, _AdcRectAvg, _AdcApp, _AdcFreq, _AdcPeriod, _AdcMaxType = _AdcPeriod };

		NATIVEENUM Type;
		
		float fValue;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Source) << _E(Type);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Source) >> _E(Type);
			return *this;
		}
	};
	class DmmSettings : public CSerialize 
	{
	public:
		static const char* const ppszTextDmmMode[];
		// = {"V-AC", "V-DC, "Cont."};
		static const char* const ppszTextDmmRange[];
		static const char* const ppszTextDmmTime[];

		enum EDmmMode { _VAC = 0, _VDC, _CONT, _OHM ,_mADC, _mAAC, _ModeMax = _mAAC }
			Mode;

		enum EDmmRange { _DmmRangeAuto, _DmmRange50mV, _DmmRange100mV, _DmmRange200mV, _DmmRange500mV, _DmmRange1V, _DmmRange2V, _DmmRange5V, _DmmRange10V, _DmmRangeMax = _DmmRange10V} 
			Range;

		enum EDmmTime { 
			_DmmTimeAuto, _DmmTime200ns, _DmmTime500ns, 
			_DmmTime1us, _DmmTime2us, _DmmTime5us,
			_DmmTime10us, _DmmTime20us, _DmmTime50us, _DmmTime100us, _DmmTime200us, _DmmTime500us,
			_DmmTime1ms, _DmmTime2ms, _DmmTime5ms,
			_DmmTime10ms, _DmmTime20ms, _DmmTime50ms, _DmmTime100ms, _DmmTime200ms, _DmmTime500ms, 
			_DmmTime1s, _DmmTimeMax = _DmmTime500ms }
			Time;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Mode) << _E(Range) << _E(Time);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Mode) >> _E(Range) >> _E(Time);
			return *this;
		}
	};
	class MathOperand : public CSerialize
	{
	public:
		static const char* const ppszTextType[];
		// = {"CH1 raw", "CH1", "CH2 raw", "CH2", "Constant"}
		enum { _Ch1Raw, _CH1Corrected, _Ch2Raw, _Ch2Corrected, _Constant, _Fx, _TypeMax = _Fx }
			Type;
		int nScale;
		int nConstant;
		PCSTR strName;
	//	ui16 uiColor;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Type) << nConstant << nScale;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Type) >> nConstant >> nScale;
			return *this;
		}
	};
	class MathOperator : public CSerialize
	{
	public:
		static const char* const ppszTextType[];
		// = {"Off", "A", "B", "A+B+C", "A+B-C", "B-A+C"}
		enum { _Off, _A, _B, _C, _AplusBplusC, _AminusBplusC, _BminusAplusC, 
			_AgreaterBplusC, _AlessBplusC, _minAB, _maxAB, _FirAplusC, _FirAdivBplusC, _TypeMax = _FirAdivBplusC }
			Type;
		ui16 uiColor;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Type) << uiColor;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Type) >> uiColor;
			return *this;
		}
	};
	class Display : public CSerialize 
	{
	public:
		static const char* const ppszTextAxes[];
		// = {"T-Y", "X-Y"};
		static const char* const ppszTextDraw[];
		// = {"Points", "Lines", "Fill"};
		static const char* const ppszTextAverage[];
		// = {"No", "Yes"};
		static const char* const ppszTextPersist[];
		// = {"No", "Yes"};
		static const char* const ppszTextGrid[];
		// = {"None", "Dots", "Lines"};
		static const char* const ppszTextAxis[];
		// = {"None", "Single", "Double"};

		enum { _TY, _XY, _YX, _AxesMax = _YX }
			Axes;
		enum { _Points, _Lines, _Fill, _DrawMax = _Fill }
			Draw;
		enum { _AvgNo, _AvgCh1, _AvgCh2, _AverageMax = _AvgCh2 }
			Average;
		enum { _PerNo, _PerYes, _PersistMax = _PerYes }
			Persist;
		enum { _GridNone, _GridDots, _GridLines, _GridMax = _GridLines }
			Grid;
		enum { _AxisNone, _AxisSingle, _AxisDouble, _AxisMax = _AxisDouble }
			Axis;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Axes) << _E(Draw) << _E(Average) << _E(Persist) << _E(Grid) << _E(Axis);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Axes) >> _E(Draw) >> _E(Average) >> _E(Persist) >> _E(Grid) >> _E(Axis);
			return *this;
		}
	};
	class Spectrum : public CSerialize
	{
	public:
		static const char* const ppszTextWindow[];
		// = {"Rect", "Hann"};
		static const char* const ppszTextDisplay[];
		// = {"FFT", "FFT&Time"};
		static const char* const ppszTextScale[];
		// = {"Lin", "Log"};
		static const char* const ppszTextSource[];
		// = {"Off", "CH1", "CH2"};
		static const char* const ppszTextMode[];
		// = {"Manual", "Find max"};

		enum { _Rectangular, _Hann, _WindowMax = _Hann }
			Window;
		enum { _Fft, _FftTime, _Spectrograph, _DisplayMax = _Spectrograph }
			Display;
		enum { _Lin, _Log, _ScaleMax = _Log }
			YScale;
		enum { _Off, _SrcCh1, _SrcCh2, _SourceMax = _SrcCh2 }
			MarkerSource;
		enum { _Manual, _FindMax, _ModeMax = _FindMax }
			MarkerMode;
	
		int nWindowLength;
		int		nMarkerX;

		float	fMarkerY;
		float fMarkerX;
		
		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Window) << _E(Display) << _E(YScale) << _E(MarkerSource) << nMarkerX << _E(MarkerMode);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Window) >> _E(Display) >> _E(YScale) >> _E(MarkerSource) >> nMarkerX >> _E(MarkerMode);
			return *this;
		}
	};
	class CRuntime : public CSerialize
	{
	public:
		static const char* const ppszTextBeepOnOff[];
		// = {"On", "Off"};
		static const char* const ppszTextShortcut[];
		// = {"Start/Stop Acquisition", "Screenshot"};

		enum { _On, _Off, _BeepMax = _Off }
			m_Beep;

		enum EShortcut { None = -1, StartStop = -2, Toolbox = -3, WaveManager = -4, Screenshot = -5, _ShortcutMax = Screenshot };

		int m_nShortcutCircle;
		int m_nShortcutTriangle;
		int m_nShortcutS1;
		int m_nShortcutS2;

		int m_nMenuItem;
		int m_nUptime;
		int m_nBacklight;
		int m_nVolume;
		int m_nStandby;
		
		FLOAT m_fTimeRes;
		FLOAT m_fCH1Res;
		FLOAT m_fCH2Res;

		int m_bUartTest;
		int m_bUartEcho;	// not saved in settings
		int m_bUartSdk;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << m_nMenuItem << m_nUptime << m_nBacklight << m_nVolume << m_nStandby
				<< _E(m_Beep) << m_nShortcutTriangle << m_nShortcutS1 << m_nShortcutS2
				<< m_bUartTest;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> m_nMenuItem >> m_nUptime >> m_nBacklight >> m_nVolume >> m_nStandby
				>> _E(m_Beep) >> m_nShortcutTriangle >> m_nShortcutS1 >> m_nShortcutS2
				>> m_bUartTest;
			return *this;
		}
	};

public:
	#include "Calibration.h"
	
public:
	CRuntime Runtime;
	AnalogChannel CH1;
	AnalogChannel CH2;
	DigitalChannel CH3;
	DigitalChannel CH4;
	TimeBase Time;
	Trigger Trig;
	Generator Gen;
	Marker		MarkT1;
	Marker		MarkT2;
	Marker		MarkY1;
	Marker		MarkY2;
	Measure		Meas[6];
	DmmMeasure 	DmmMeas[3][6];
	DmmSettings	Dmm;

	MathOperand MathA;
	MathOperand MathB;
	MathOperand MathC;
	MathOperator Math;

	Display	Disp;
	
	Spectrum Spec;

	Calibrator	CH1Calib;
	Calibrator	CH2Calib;
	LinApprox	DacCalib;
	ui32		m_lLastChange;

	virtual CSerialize& operator <<( CStream& stream )
	{
		ui32 dwId = _VERSION;
		ui32 dwEnd = ToDword('E', 'N', 'D', 27);
		stream << dwId << Runtime << CH1 << CH2 << CH3 << CH4 << Time << Trig << Gen
			<< MarkT1 << MarkT2 << MarkY1 << MarkY2
			<< Meas[0] << Meas[1] << Meas[2] << Meas[3] << Meas[4] << Meas[5]
			<< DmmMeas[0][0] << DmmMeas[1][0] << DmmMeas[2][0]
			<< DmmMeas[0][1] << DmmMeas[1][1] << DmmMeas[2][1]
			<< DmmMeas[0][2] << DmmMeas[1][2] << DmmMeas[2][2]
			<< DmmMeas[0][3] << DmmMeas[1][3] << DmmMeas[2][3]
			<< DmmMeas[0][4] << DmmMeas[1][4] << DmmMeas[2][4]
			<< DmmMeas[0][5] << DmmMeas[1][5] << DmmMeas[2][5]
			<< MathA << MathB << MathC << Math
			<< Spec
			<< dwEnd;

		return *this;
	}

	virtual CSerialize& operator >>( CStream& stream )
	{
		ui32 dwId = 0, dwEnd = 0;
		
		stream >> dwId;
		//_ASSERT( dwId == _VERSION );
		if ( dwId == _VERSION )
		{
			stream >> Runtime >> CH1 >> CH2 >> CH3 >> CH4 >> Time >> Trig >> Gen
				>> MarkT1 >> MarkT2 >> MarkY1 >> MarkY2
				>> Meas[0] >> Meas[1] >> Meas[2] >> Meas[3] >> Meas[4] >> Meas[5]
				>> DmmMeas[0][0] >> DmmMeas[1][0] >> DmmMeas[2][0] 
				>> DmmMeas[0][1] >> DmmMeas[1][1] >> DmmMeas[2][1]
				>> DmmMeas[0][2] >> DmmMeas[1][2] >> DmmMeas[2][2]
				>> DmmMeas[0][3] >> DmmMeas[1][3] >> DmmMeas[2][3]
				>> DmmMeas[0][4] >> DmmMeas[1][4] >> DmmMeas[2][4]
				>> DmmMeas[0][5] >> DmmMeas[1][5] >> DmmMeas[2][5]
				>> MathA >> MathB >> MathC >> Math
				>> Spec
				>> dwEnd;
			// Dont forget to change version when the serialization order was changed!
			_ASSERT( dwEnd == ToDword('E', 'N', 'D', 27) );
			if ( dwEnd != ToDword('E', 'N', 'D', 27) )
			{
				Reset();
			}
		} else
			Reset();
		return *this;
	}

	CSettings();
	ui32 GetChecksum();
	ui32 GetStaticChecksum();
	void Save();
	void Kick();
	void Load();
	void Reset();

	void ResetCalibration();
	void SaveCalibration();
	bool LoadCalibration();

	static const char* const* getTypeText(CSettings::DmmSettings::EDmmMode mode);
	static const char* const* getTypeSuffix(CSettings::DmmSettings::EDmmMode mode);
	static NATIVEENUM getTypeMaxEnum(CSettings::DmmSettings::EDmmMode mode);
};
#endif
