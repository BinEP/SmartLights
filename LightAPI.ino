//Strobe
void Strobe(int FlashDelay, byte red, byte green, byte blue, int StrobeCount, int EndPause) {

  int timing = (2 * FlashDelay * StrobeCount) + EndPause;

  for (int i = 0; i < 30000 / timing; i++) {
    for (int j = 0; j < StrobeCount; j++) {
      setAll(red, green, blue);
      if (showStrip()) {
        return;
      };
      delay(FlashDelay);
      setAll(0, 0, 0);
      if (showStrip()) {
        return;
      }
      delay(FlashDelay);
    }

    delay(EndPause);
  }
}

//Cylon

void CylonBounce(int SpeedDelay, byte red, byte green, byte blue, int EyeSize, int ReturnDelay, double pixelMultiplier) {

  int timing = ((int(NUM_LEDS * pixelMultiplier) - 2 + EyeSize) * SpeedDelay + ReturnDelay) * 2;
  for (int h = 0; h < 30000 / timing; h++) {
    for (int i = -EyeSize; i < int(NUM_LEDS * pixelMultiplier) - 2; i += 2) {
      setAll(0, 0, 0);

      setPixel(i, red / 10, green / 10, blue / 10);
      setPixel(i + 1, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++) {
        setPixel(i + j, red, green, blue);
        setPixel(i + j + 1, red, green, blue);
      }
      setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
      if (showStrip()) {
        return;
      }
      delay(SpeedDelay);
    }

    delay(ReturnDelay);

    for (int i = NUM_LEDS * pixelMultiplier - 2; i > -EyeSize; i -= 2) {
      setAll(0, 0, 0);
      setPixel(i, red / 10, green / 10, blue / 10);
      setPixel(i - 1, red / 10, green / 10, blue / 10);

      for (int j = 1; j <= EyeSize; j++) {
        setPixel(i + j, red, green, blue);
        setPixel(i + j - 1, red, green, blue);

      }
      setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
      if (showStrip()) {
        return;
      };
      delay(SpeedDelay);
    }

    delay(ReturnDelay);
  }
}

//Fire
void Fire(int SpeedDelay, int Cooling, int Sparking) {
  byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

//Color Wipe

void colorWipe(int SpeedDelay, byte red, byte green, byte blue) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
    if (showStrip()) {
      return;
    };
    delay(SpeedDelay);
  }
}


void FadeInOut(int speedIn, double speedOut, byte red, byte green, byte blue) {
  float r, g, b;

  for (int k = 0; k < int(256 * speedIn); k = k + 1) {
    r = (k / 256.0 / speedIn) * red;
    g = (k / 256.0 / speedIn) * green;
    b = (k / 256.0 / speedIn) * blue;
    setAll(r, g, b);
    if (showStrip()) {
      return;
    };
  }

  for (int k = 255 * speedOut; k >= 0; k = k - 1) {
    r = (k / 256.0 / speedOut) * red;
    g = (k / 256.0 / speedOut) * green;
    b = (k / 256.0 / speedOut) * blue;
    setAll(r, g, b);
    if (showStrip()) {
      return;
    };
  }
}

//Halloween Eyes
void HalloweenEyes(int FadeDelay, byte red, byte green, byte blue,
                   int EyeWidth, int EyeSpace,
                   boolean Fade, int Steps,
                   int EndPause) {
  randomSeed(analogRead(0));

  if (FadeDelay < 0) {
    FadeDelay = random(50, 150);
  }

  if (Steps < 0) {
    Steps = random(5, 50);
  }

  if (EndPause < 0) {
    EndPause = random(0, 3000);
  }
  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2 * EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;

  for (i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }

  if (showStrip()) {
    return;
  };

  if (Fade == true) {
    float r, g, b;

    for (int j = Steps; j >= 0; j--) {
      r = j * (red / Steps);
      g = j * (green / Steps);
      b = j * (blue / Steps);

      for (i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }

      if (showStrip()) {
        return;
      };
      delay(FadeDelay);
    }
  }

  setAll(0, 0, 0); // Set all black

  delay(EndPause);
}

//New KITT
void NewKITT(int SpeedDelay, byte red, byte green, byte blue, int EyeSize, int ReturnDelay) {
  for (int i = 0; i < 2; i++) {
    if (RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
      return;
    }
    if (LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
      return;
    }

    if (OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
      return;
    }

    if (CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
      return;
    }

    //    if (LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
    //      return;
    //    }
    //
    //    if (RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
    //      return;
    //    }
    //
    //    if (OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
    //      return;
    //    }
    //
    //    if (CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay)) {
    //      return;
    //    }

  }
}

bool CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= -EyeSize - 2; i--) {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    if (showStrip()) {
      return true;
    };
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  return false;
}

bool OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = -EyeSize - 2; i <= ((NUM_LEDS - EyeSize) / 2); i++) {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    if (showStrip()) {
      return true;
    };
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  return false;
}

bool LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = -EyeSize - 2; i < NUM_LEDS; i++) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    if (showStrip()) {
      return true;
    };
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  return false;
}

bool RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = NUM_LEDS; i > -EyeSize - 2; i--) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    if (showStrip()) {
      //      Serial.println("Show Strip True");
      return true;
    };
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  return false;
}

int cycleCount = 4;
//Rainbow Cycle
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * cycleCount; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    if (showStrip()) {
      return;
    };
    delay(SpeedDelay);
  }
}


//Cool Blue Cycle
void coolCycle(int SpeedDelay, int amountOfPurple) {
  FadeColorScheme(SpeedDelay, 2, amountOfPurple);
}

//Warm cycle
void warmCycle(int SpeedDelay, int amountOfGreen) {
  FadeColorScheme(SpeedDelay, 1, amountOfGreen);
}

//Nature Cycle
void natureCycle(int SpeedDelay, int amountOfBlue) {
  FadeColorScheme(SpeedDelay, 3, amountOfBlue);

}

void candyCycle(int SpeedDelay, int amountOfSpecial) {
  christmasCycle(SpeedDelay, amountOfSpecial);
}

void christmasCycle(int SpeedDelay) {
  christmasCycle(SpeedDelay, 5);
}

void christmasCycle(int SpeedDelay, int amountOfChristmas) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * cycleCount; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, 255 - *c, *c, amountOfChristmas);
    }
    if (showStrip()) {
      return;
    };
    delay(SpeedDelay);
  }

}
//1 = Warm
//2 = Cool
//3 = Green Bleh
void FadeColorScheme(int SpeedDelay, int setOfColors, int amountOfColor) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * cycleCount; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      //  Serial.println(*c);
      int colorMod = amountOfColor;
      //70 is optimal

      switch (setOfColors) {
        case 1:
          if (*c < colorMod) {
            setPixel(i, 255, colorMod - *c, 0);
          } else {
            setPixel(i, 255, 0, *c - colorMod);
          }
          break;
        case 2:
          if (*c < colorMod) {
            setPixel(i, (colorMod - *c) * 2, 0, 255);
          } else {
            setPixel(i, 0, (*c - colorMod), 255);
          }
          break;
        case 3:
          if (*c < colorMod) {
            setPixel(i, 0, 255, colorMod - *c);
          } else {
            setPixel(i, *c - colorMod, 255, 0);
          }

          break;
      }

    }
    if (showStrip()) {
      return;
    };
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}




//Twinkle
void Twinkle(int SpeedDelay, byte red, byte green, byte blue, int Count, boolean OnlyOne, boolean ifClear, int randomClear) {
  int timing = SpeedDelay * (Count + 1);
  for (int a = 0; a < 30000 / timing; a++) {
    if (ifClear) {
      setAll(0, 0, 0);
    }

    for (int i = 0; i < Count; i++) {
      setPixel(random(NUM_LEDS), red, green, blue);
      for (int j = 0; j < randomClear; j++) {
        setPixel(random(NUM_LEDS), 0, 0, 0);
      }
      if (showStrip()) {
        return;
      };
      delay(SpeedDelay);
      if (OnlyOne) {
        setAll(0, 0, 0);
      }
    }

    delay(SpeedDelay);
  }
}




//Random Twinkle
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne, boolean ifClear, int randomClear) {
  int timing = SpeedDelay * (Count + 1);
  for (int a = 0; a < 30000 / timing; a++) {
    if (ifClear) {
      setAll(0, 0, 0);
    }

    for (int i = 0; i < Count; i++) {
      setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
      for (int j = 0; j < randomClear; j++) {
        setPixel(random(NUM_LEDS), 0, 0, 0);
      }

      if (showStrip()) {
        return;
      };
      delay(SpeedDelay);
      if (OnlyOne) {
        setAll(0, 0, 0);
      }
    }

    delay(SpeedDelay);
  }
}

//Running Lights
void RunningLights(int WaveDelay, byte red, byte green, byte blue) {
  int Position = 0;

  for (int i = 0; i < NUM_LEDS * 2; i++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUM_LEDS; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    if (showStrip()) {
      return;
    };
    delay(WaveDelay);
  }
}


//Snow Sparkle

void SnowSparkle(int SpeedDelay, byte red, byte green, byte blue, int SparkleDelay, int sparkleNum) {
  int timing = SparkleDelay + 300;
  for ( int k = 0; k < 30000 / timing * 2; k++) {
    setAll(red, green, blue);
    int pixels[sparkleNum];
    for (int i = 0; i < sparkleNum; i++) {
      int Pixel = random(NUM_LEDS);
      setPixel(Pixel, 0xff, 0xff, 0xff);
      pixels[i] = Pixel;
    }

    if (showStrip()) {
      return;
    };
    delay(SparkleDelay);
    for (int i = 0; i < sparkleNum; i++) {
      setPixel(pixels[i], red, green, blue);

    }
    showStrip();
    delay(SpeedDelay);
  }
}


//Bouncing Balls

void BouncingBalls(int BallCount, byte red, byte green, byte blue) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i] / 1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if ( Height[i] < 0 ) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }

    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i], red, green, blue);
    }

    if (showStrip()) {
      return;
    };
    setAll(0, 0, 0);
  }
}

//Multi Colored Bouncing Balls
void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i] / 1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if ( Height[i] < 0 ) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }

    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i], colors[i][0], colors[i][1], colors[i][2]);
    }

    if (showStrip()) {
      return;
    };
    setAll(0, 0, 0);
  }
}



//Sparkle
void Sparkle(int SpeedDelay, byte red, byte green, byte blue) {
  for (int i = 0; i < 30000 / SpeedDelay / 1.2; i++) {

    int Pixel = random(NUM_LEDS);
    setPixel(Pixel, red, green, blue);
    if (showStrip()) {
      return;
    };
    delay(SpeedDelay);
    setPixel(Pixel, 0, 0, 0);
  }
}



//Theater Chase
void theaterChase(int SpeedDelay, byte red, byte green, byte blue) {
  int timing = SpeedDelay * 30;
  for (int k = 0; k < 30000 / timing; k++) {
    for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
      for (int q = 0; q < 3; q++) {
        for (int i = 0; i < NUM_LEDS; i = i + 3) {
          setPixel(i + q, red, green, blue);  //turn every third pixel on
        }
        if (showStrip()) {
          return;
        };

        delay(SpeedDelay);

        for (int i = 0; i < NUM_LEDS; i = i + 3) {
          setPixel(i + q, 0, 0, 0);    //turn every third pixel off
        }
      }
    }
  }
}


//Theater Chase Rainbow

void theaterChaseRainbow(int SpeedDelay, int colorSpeedChange) {
  byte *c;
  int timing = SpeedDelay * 3 * 83;
  for (int k = 0; k < 30000 / timing; k++) {
    for (int j = 0; j < 256; j += colorSpeedChange) {  // cycle all 256 colors in the wheel
      for (int q = 0; q < 3; q++) {
        for (int i = 0; i < NUM_LEDS; i = i + 3) {
          c = Wheel( (i + j) % 255);
          setPixel(i + q, *c, *(c + 1), *(c + 2)); //turn every third pixel on
        }
        if (showStrip()) {
          return;
        };

        delay(SpeedDelay);

        for (int i = 0; i < NUM_LEDS; i = i + 3) {
          setPixel(i + q, 0, 0, 0);    //turn every third pixel off
        }
      }
    }
  }
}

void candyCane(int SpeedDelay, int setOfLightsNum) {
  for (int h = 0; h < 40; h++) {
    for (int j = -setOfLightsNum * 2; j < 0; j++) {
      for (int i = j; i < NUM_LEDS; i += setOfLightsNum * 2) {
        for (int k = 0; k < setOfLightsNum; k++) {
          setPixel(i + k, 255, 255, 255);
        }

        for (int k = setOfLightsNum; k < setOfLightsNum * 2; k++) {
          setPixel(i + k, 255, 0, 0);
        }
      }
      if (showStrip()) {
        return;
      }

      delay(SpeedDelay);
    }
  }
}

















bool showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
  // #ifndef ADAFRUIT_NEOPIXEL_H
  //   // FastLED
  //   FastLED.show();
  // #endif

  if (Serial.available() > 0) {

    int newCase = Serial.parseInt(); // read the incoming byte:
    Serial.println(newCase);
    currentCase = newCase;

  }



  if (wifiConnected) {

   // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();
    server.handleClient(); //Handling of incoming requests
    MDNS.update();
    ArduinoOTA.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);
    
  
  }

  if (oldCase != currentCase) {
    strip.setBrightness(brightness);
  }
  return oldCase != currentCase;
  // return false;

}


void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
  // #ifndef ADAFRUIT_NEOPIXEL_H
  //   // FastLED
  //   leds[Pixel].r = red;
  //   leds[Pixel].g = green;
  //   leds[Pixel].b = blue;
  // #endif
}

void setIntPixel(int Pixel, int red, int green, int blue) {
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void clearStrip() {
  setAll(0, 0, 0);
}

void clearStrip(int numLeds) {
  for (int i = 0; i < numLeds; i++ ) {
    setPixel(i, 0, 0, 0);
  }
  //  Serial.println("Setall");
  if (showStrip()) {
    return;
  };
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  //  Serial.println("Setall");
  if (showStrip()) {
    return;
  };
}
