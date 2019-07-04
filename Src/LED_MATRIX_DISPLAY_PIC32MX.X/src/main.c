/* 
 * Author: J. Bajic
 */

#include <p32xxxx.h>
#include <plib.h>
#include "CPUConfig.h"
#include "displayConfig.h"

#if (REFRESH_MODE==1)

#define PRESCALER           T1_PS_1_256
#define T1_TICK       		(SYS_FREQ/256/REFRESH_RATE) 

void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void)
{
    //disk_timerproc();
    __builtin_disable_interrupts();
    mT1ClearIntFlag(); 
    refreshDisplay();
    __builtin_enable_interrupts();
}
 
void initTimer(void)
{
    OpenTimer1(T1_ON | T1_SOURCE_INT | PRESCALER, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
    INTEnableSystemMultiVectoredInt();   
    
  
}

#endif
void init(void)
{
    unsigned int cache_status;
    // disable JTAG port
    DDPCONbits.JTAGEN = 0;
    AD1PCFG = 0xFFFF;
    
    mBMXDisableDRMWaitState();
    mCheConfigure(3);
    cache_status = mCheGetCon();
    cache_status |= CHE_CONF_PF_ALL;
    mCheConfigure(cache_status);
    CheKseg0CacheOn();
    SYSTEMConfigPerformance(SYS_FREQ);
    
#if (REFRESH_MODE==1)
    initTimer();
#endif 
    
    
#if (USE_SD_CARD)
    unmountSD();
#endif

}

void main(void) 
{
    init();
    
    setup();

    while(1)
    { 
        loop(); 
    }
}