#ifndef TIMER_H_
#define TIMER_H_

extern void
setupTimer(uint16_t period);

extern void 
setupLETimer(uint16_t period);

extern void
startLETimer(void);

extern void
stopLETimer(void);

#endif
