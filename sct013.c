#include <xc.h>
#include "sct013.h"

float calcIrms(void)
{

  for (unsigned int n = 0; n < NUMBER_OF_SAMPLES; n++)
  {
   

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    //  then subtract this - signal is now centered on 0 counts.
    
    

    // Root-mean-square method current
    // 1) square current values
    
    // 2) sum
    sumI += sqI;
  }

  double I_RATIO = ICAL *((SupplyVoltage/1000.0) / (ADC_COUNTS));
  Irms = I_RATIO * sqrt(sumI / Number_of_Samples);

  //Reset accumulators
  sumI = 0;
  //--------------------------------------------------------------------------------------

  return Irms;
}