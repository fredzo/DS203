extern "C" {
#include "interrupt.h"
#include "BIOS.h"
}

#include <Source/Main/Application.h>

extern u8 _vectors[];
int __errno; // required by math.h

int main(void)
{ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, (u32)&_vectors);
	UsartInit();
	__USB_Init();

	CApplication app;
	app.Create();

	while ( app() );

	return 0;
}
