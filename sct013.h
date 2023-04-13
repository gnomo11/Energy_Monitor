#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS) //0b10000000000= 1024
#define ICAL 1
#define SupplyVoltage 5000

 //void current(unsigned int _inPinI, double _ICAL);
 float calcIrms(void);