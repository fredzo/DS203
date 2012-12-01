class CMIBacklight : public CMIProgress
{
public:
	virtual int GetValue()
	{
		return Settings.Runtime.m_nBacklight / 4;
	}
	virtual void SetValue(int nValue)
	{
		Settings.Runtime.m_nBacklight = nValue * 4;
		CCoreSettings::Update();
		Settings.Kick();
	}
	virtual int GetMin()
	{
		return 0;
	}
	virtual int GetMax()
	{
		return 25;
	}
};
