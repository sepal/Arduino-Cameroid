
#ifndef _INITCONF_H_
#define _INITCONF_H_

#include "WProgram.h"

struct InitConf
{
  enum EncoderMode
  {
    ENCODER_RELATIVE,
    ENCODER_ABSOLUTE
  };
  EncoderMode encoderMode;
  int encMin;
  int encMax;
  int encPrec;
  
};

#endif
