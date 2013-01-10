#include "Dmm.h"
#include <Source/Core/Settings.h>
#include <string.h>
#include <Source/Core/Utils.h>
#include <math.h>

void DrawTriangle(int x, int y, int size, bool half, ui8 quadrant, ui16 clr)
{
	if(half)
	{
		for(int xPos = 0 ; xPos < size ; xPos++)
		{
			int yStart;
			if(quadrant == 0 || quadrant == 3)
			{
				yStart = 0;
			}
			else if (quadrant == 1)
			{
				yStart = size - xPos - 1;
			}
			else // if(quadrant == 2)
			{
				yStart = xPos;
			}
			int yMax;
			if(quadrant == 1 || quadrant == 2)
			{
				yMax = size;
			}
			else if(quadrant == 0)
			{
				yMax = xPos;
			}
			else // if(quadrant == 3)
			{
				yMax = size - xPos - 1;
			}
			for(int yPos = yStart ; yPos < yMax ; yPos++)
			{
				BIOS::LCD::PutPixel(x+xPos,y+yPos,clr);
			}
		}
	}
	else
	{
		int halfSize = size / 2;
		if(quadrant == 0 || quadrant == 2)
		{
			for(int xPos = 0 ; xPos < halfSize ; xPos++)
			{
				int yStart = (quadrant == 0) ? 0 : halfSize-xPos;
				int yMax = (quadrant == 0) ? xPos : halfSize;
				for(int yPos = yStart ; yPos < yMax ; yPos++)
				{
					BIOS::LCD::PutPixel(x+xPos,y+yPos,clr);
				}
			}
			for(int xPos = 0 ; xPos < halfSize ; xPos++)
			{
				int yStart = (quadrant == 0) ? 0 : xPos;
				int yMax = (quadrant == 0) ? halfSize-xPos : halfSize;
				for(int yPos = yStart ; yPos < yMax ; yPos++)
				{
					BIOS::LCD::PutPixel(x+halfSize+xPos,y+yPos,clr);
				}
			}
		}
		else // if(quadrant == 1 || quadrant == 3)
		{
			for(int yPos = 0 ; yPos < halfSize ; yPos++)
			{
				int xStart = (quadrant == 1) ? halfSize-yPos : 0;
				int xMax = (quadrant == 1) ? halfSize : yPos;
				for(int xPos = xStart ; xPos < xMax ; xPos++)
				{
					BIOS::LCD::PutPixel(x+xPos,y+yPos,clr);
				}
			}
			for(int yPos = 0 ; yPos < halfSize ; yPos++)
			{
				int xStart = (quadrant == 1) ? yPos : 0;
				int xMax = (quadrant == 1) ? halfSize : halfSize-yPos;
				for(int xPos = xStart ; xPos < xMax ; xPos++)
				{
					BIOS::LCD::PutPixel(x+xPos,y+halfSize+yPos,clr);
				}
			}
		}
	}
}

void _DrawMinus(int x, int y, int width, int size, ui16 clr)
{
	if(width > 4)
	{
		BIOS::LCD::RoundRect( x, y, x+size, y+width, clr ); 
	}
	else
	{
		BIOS::LCD::Bar( x, y, x+size, y+width, clr ); 
	}
}

void _DrawDigitHT(int x, int y, int width, int size, ui16 clr)
{
	DrawTriangle(x,y,width,true,0,clr);
	BIOS::LCD::Bar( x+width, y, x+size, y+width, clr ); 
	DrawTriangle(x+size,y,width,true,3,clr);
}

void _DrawDigitHC(int x, int y, int width, int size, ui16 clr)
{
	DrawTriangle(x,y,width,false,1,clr);
	BIOS::LCD::Bar( x+width/2, y, x+size+width/2, y+width, clr ); 
	DrawTriangle(x+size+width/2,y,width,false,3,clr);
}

void _DrawDigitHB(int x, int y, int width, int size, ui16 clr)
{
	DrawTriangle(x,y,width,true,1,clr);
	BIOS::LCD::Bar( x+width, y, x+size, y+width, clr ); 
	DrawTriangle(x+size,y,width,true,2,clr);
}

void _DrawDigitVL(int x, int y, int width, int size, ui16 clr)
{
	DrawTriangle(x,y,width,true,2,clr);
	BIOS::LCD::Bar( x, y+width, x+width, y+size, clr ); 
	DrawTriangle(x,y+size,width,true,3,clr);
}

void _DrawDigitVR(int x, int y, int width, int size, ui16 clr)
{
	DrawTriangle(x,y,width,true,1,clr);
	BIOS::LCD::Bar( x, y+width, x+width, y+size, clr ); 
	DrawTriangle(x,y+size,width,true,0,clr);
}

void _DrawDot0(int x, int y, int width, int size, int space, ui16 clr)
{
	BIOS::LCD::Bar( x, y, x+(width/4)*3, y+(width/4)*3, clr );
}

void CWndDmm::OnPaint(bool updateBg)
{
	if ( CWnd::m_rcOverlay.IsValid() )
	{	// Do not redraw screen if modal window is active, but prepare for a refresh when it's closed
		bRefresh = true;
		return;
	}

	bool refreshValue = false;
	// TODO make refresh rate configurable
	// No more than one refresh each 500 ms for values
	if (!bTimer)
	{	// Only delay next update if we were not refreshing
		if(!updateBg && !bRefresh)
		{
			KillTimer();
			SetTimer(500);
			bTimer = true;
		}
		refreshValue = true;
	}
	// Draw values
	DisplayValue(Settings.DmmMeas[0].fValue,false,0,Settings.DmmMeas[0].Type,bRefresh,refreshValue);
	DisplayValue(Settings.DmmMeas[1].fValue,false,1,Settings.DmmMeas[1].Type,bRefresh,refreshValue);
	DisplayValue(Settings.DmmMeas[2].fValue,false,2,Settings.DmmMeas[2].Type,bRefresh,refreshValue);
	
	UpdateBargraphValue();
	OnTick(true);

	// Draw Graduation
	if(updateBg)
	{
		int x = CWndDmm::cBargraphLeft;
		int y = CWndDmm::cBargraphTop;
		for(int i = 0 ; i <= 20 ; i++)
		{
			if (i%10 == 0)
			{
				BIOS::LCD::Bar( x-1, y-4, x+1, y+2,  CWndDmm::cOn );
				BIOS::LCD::Printf(i==0 ? x-4 : i == 10 ? x-8: x-12 ,y+2,CWndDmm::cLabel, CWndDmm::cOff, "%d", i*5);
			}
			else if(i%5 == 0)
			{
				BIOS::LCD::Bar( x-1, y-4, x+1, y,  CWndDmm::cOn );
				BIOS::LCD::Printf(x-8,y+2,CWndDmm::cLabel, CWndDmm::cOff, "%d", i*5);
			} 
			else
			{
				BIOS::LCD::Bar( x-1, y-4, x, y-2,  CWndDmm::cOn );
			}
			x+=15;
		}
	}
	
	// Draw bottom line
	//BIOS::LCD::Printf( 8, 220, RGB565(808080), RGB565(ffffff), "adc=%3f cal=%3f var=%3f range=%s ", m_fAverage, fCorrect, m_fVariance, CSettings::AnalogChannel::ppszTextResolution[Settings.CH1.Resolution]);
	if(updateBg || bRefresh)
	{
		int x = 34, y = 226;
		int _x = x;
		x += BIOS::LCD::Print( x, y, Settings.CH1.u16Color, RGB565(000000), "CH1: " );
		x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
			CSettings::AnalogChannel::ppszTextResolution[ Settings.CH1.Resolution ]);
		
		x = _x + 100;
		x += BIOS::LCD::Print( x, y, Settings.CH2.u16Color, RGB565(000000), "CH2: " );
		x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
			CSettings::AnalogChannel::ppszTextResolution[ Settings.CH2.Resolution ]);

		x = _x + 200;
		x += BIOS::LCD::Print( x, y, RGB565(b0b0b0), RGB565(000000), "T: " );
		x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
			CSettings::TimeBase::ppszTextResolution[ Settings.Time.Resolution ]);
	}

	bRefresh = false;
}

void CWndDmm::UpdateBargraphValue() 
{
	m_targetBargraph = (int)(Settings.DmmMeas[0].fValue*300);
	if(m_targetBargraph < 0)
	{
		m_bargraphPositive = false;
		m_targetBargraph = -m_targetBargraph;
	}
	else
	{
		m_bargraphPositive = true;
	}
	if(m_targetBargraph > 301)
	{
		m_targetBargraph = 301;
	}
	else if (m_targetBargraph < 0)
	{
		m_targetBargraph = 0;
	}
}

void CWndDmm::UpdateBargraphDisplay() 
{
	// Draw Bargraph
	int x = CWndDmm::cBargraphLeft;
	int y = CWndDmm::cBargraphTop;
	// Plus / minus sign
	BIOS::LCD::Bar( x-7, y-8, x-5, y-2,  m_bargraphPositive ? CWndDmm::cOn : CWndDmm::cClr);
	BIOS::LCD::Bar( x-9, y-6, x-3, y-4,  CWndDmm::cOn );
	// Bargraph
	BIOS::LCD::Bar( x+m_currentBargraph+1, y-12, x+302, y-4,  CWndDmm::cClr );
	for(int i=0; i <= m_currentBargraph ; i++, x++)
	{
		if (i%150 == 0)
		{
			BIOS::LCD::Bar( x-1, y-12, x+1, y-4,  CWndDmm::cOn );
			x++;
			i++;
		}
		else if(i%75 == 0)
		{
			BIOS::LCD::Bar( x-1, y-10, x+1, y-4,  CWndDmm::cOn );
			x++;
			i++;
		} 
		else if(i%15 == 0)
		{
			BIOS::LCD::Bar( x-1, y-10, x, y-4,  CWndDmm::cOn );
		} 
		else
		{
			BIOS::LCD::Bar( x-1, y-8, x, y-4,  CWndDmm::cOn );
		}
	}
}

void CWndDmm::OnTick(bool force) 
{
	if(m_currentBargraph != m_targetBargraph)
	{
		if(force || m_tickCounter <= 0)
		{
			int diff = (m_targetBargraph - m_currentBargraph);
			bool negative = diff < 0;
			m_tickCounter = 3000/abs(diff);
			if(negative)
			{
				m_currentBargraph--;
			}
			else
			{
				m_currentBargraph++;
			}
			UpdateBargraphDisplay();
			//char test[32];
			//BIOS::DBG::sprintf(test, "cur:%d tarw:%d tick:%d\n", m_currentBargraph, m_targetBargraph, m_tickCounter);
			//BIOS::SERIAL::Send(test);
		}
		else
		{
			m_tickCounter--;
		}
	}
	else if(force)
	{
		UpdateBargraphDisplay();
	}
}

void CWndDmm::DisplayValue(float value, bool isErr, int position, int type, bool redraw, bool refreshValue)
{
	char newDisplay[12];
	bool negative = value < 0;
	value = abs(value);
	if (isErr)
	{
		newDisplay[0]=' ';
		newDisplay[1]=' ';
		newDisplay[2]='E';
		newDisplay[3]='r';
		newDisplay[4]='r';
	}
	else
	{
		BIOS::DBG::sprintf( newDisplay, " %04f", value );
		newDisplay[0] = negative ? '-' : ' ';
	}

	// Setup disaplay according to position
	int width=8;
	int size=30;
	int space=1;
	int x=36;
	int y=70;
	int scale=2;
	switch(position)
	{
	case 0:
		// Main => keep init values
		break;
	case 1:
		// Value1
		width = 4;
		size = 10;
		space = 1;
		x=51;
		y=18;
		scale = 1;
		break;
	case 2:
		// Value2
		width = 4;
		size = 10;
		space = 1;
		x=186;
		y=18;
		scale = 1;
		break;
	default:
		_ASSERT( !!!"Invalid position" );
	}
	bool refreshAll = (redraw || type != values[position][5]);
	if(refreshAll)
	{	// Label
		BIOS::LCD::Bar( x+((size/2+width)+4*space), y, x+4*size+8*width+16*space, y+14*scale, CWndDmm::cClr ); 
		CUtils::Print( x+((size/2+width)+4*space), y, CWndDmm::cLabel, CWndDmm::cOff, scale, CSettings::DmmMeasure::ppszTextType[type]);
	}
	if(refreshAll || refreshValue)
	{
		y+=14*scale+2*space;
		// Value
		int i;
		const char* pDisplay = newDisplay;
		for (i=0; i < 5; i++)
		{
			int nDigit = *pDisplay;
			if(nDigit == 0) 
			{  // End of string
				nDigit = ' ';
			}
			else 
			{
				if ((i < 4) && (pDisplay[1] == '.'))
				{
					nDigit |= 128;
					pDisplay++;
				}
				pDisplay++;
			}
			if(redraw || nDigit != values[position][i])
			{
				if(i == 0)
				{	// Minus sign
					if(nDigit ==  '-')
					{
						_DrawMinus( x, y+width/2+size+2*space-1, width, size/2+width, CWndDmm::cOn );
					}
					else
					{
						_DrawMinus( x, y+width/2+size+2*space-1, width, size/2+width, CWndDmm::cOff );
					}
				}
				else
				{	// Other digits
		 			DrawDigit(x, y, width, size, space, nDigit, CWndDmm::cOn, CWndDmm::cOff);
				}
				values[position][i] = nDigit;
			}
			if(i == 0)
			{
				x+=((size/2+width)+3*space+scale*space);
			}
			else if(i == 4)
			{
				x+=(size+width+4*space+scale*space);
			}
			else
			{
				x+=(size+2*width+7*space);
			}
		}
		if(refreshAll)
		{	// Unit
			BIOS::LCD::Bar( x, y, x+24*scale, y+14*scale, CWndDmm::cClr ); 
			CUtils::Print( x, y, CWndDmm::cOn, CWndDmm::cClr, scale, CSettings::DmmMeasure::ppszTextSuffix[type]);
			values[position][5] = type;
		}
	}
}

void CWndDmm::DrawDigit(int x, int y, int width, int size, int space, int nDigit, ui16 clrOn, ui16 clrOff)
{
	ui32 decoder[] =
	{0x1111110, 0x0110000, 0x1101101, 0x1111001, 0x0110011, 0x1011011, 0x1011111, 0x1110000, 0x1111111, 0x1111011};
	ui32 dec = 0;
	if ( (nDigit&127) >= '0' && (nDigit&127) <= '9' )
	{
		dec = decoder[nDigit&15];
		if ( nDigit & 128 )
			dec |= 0x10000000;
	} 
	else
	{
		switch (nDigit)
		{
		case ' ': dec = 0; break;
		case '?': dec = 0; break;
		case '-': dec = 0x0000001; break;
		case 'E': dec = 0x1001111; break;
		case 'r': dec = 0x0000101; break;
		}
	}

	#define EN(n) ( dec>>(28-n*4)&1 ) ? clrOn : clrOff
 	_DrawDigitHT( x+space, y, width, size, EN(1) );
	_DrawDigitHC( x+space, y+width/2+size+2*space-1, width, size, EN(7) );
	_DrawDigitHB( x+space, y+size*2+width+4*space, width, size, EN(4) );
	_DrawDigitVR( x+size+2*space, y+space, width, size, EN(2) );
	_DrawDigitVR( x+size+2*space, y+size+width+3*space, width, size, EN(3) );
	_DrawDigitVL( x, y+space, width, size, EN(6) ); 
	_DrawDigitVL( x, y+size+width+3*space, width, size, EN(5) ); 
	_DrawDot0(x+size+width+5*space, y+size*2+width+6*space, width, size, space, EN(0));
	#undef EN
}