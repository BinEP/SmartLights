void handleCase(int type) {
  //  setAll(0,0,0);
  Serial.print("Handling Case: ");
  Serial.println(type);
  
  //    Serial.println("Default Effects");
  switch (type) {

    case 1:
      Strobe(100, 0xff, 0xff, 0xff, 10, 1000);
      //      Strobe(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
      break;
    case 2:
      CylonBounce(10, 0xff, 0, 0, 30, 30, 1);
      //      CylonBounce(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
      break;
    case 3:
      Fire(3, 55, 120);
      //      Fire(inputs[0], inputs[1], inputs[2]);

      break;
    case 4:
      colorWipe(10, 0x00, 0xff, 0x00);
      //      colorWipe(inputs[0], inputs[1], inputs[2], inputs[3]);
      colorWipe(5, 0x00, 0x00, 0x00);
      break;
    case 5:
      FadeInOut(1, .5, 0xff, 0x00, 0x00); // red
      FadeInOut(1, .5, 0xff, 0xff, 0xff); // white
      FadeInOut(1, .5, 0x00, 0x00, 0xff); // blue

      //      FadeInOut(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]); // red
      break;
    case 6:
      for (int i = 0; i < 10; i++) {
        HalloweenEyes(random(50, 150), 0x00, 0xff, 0x00, 1, 4, true, random(5, 50), random(0, 3000));
      }
      //      HalloweenEyes(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] == 1, inputs[6], inputs[7], inputs[8]);
      break;
    case 7:
      NewKITT(5, 0xff, 0, 0, 30, 10);
      //      NewKITT(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

      break;
    case 8:
      rainbowCycle(20);
      //      rainbowCycle(inputs[0]);
      break;
    case 9:
      coolCycle(20, 60);
      //      coolCycle(inputs[0], inputs[1]);
      break;
    case 10:
      warmCycle(20, 150);
      //      warmCycle(inputs[0], inputs[1]);

      break;
    case 11:
      natureCycle(20, 50);
      //      natureCycle(inputs[0], inputs[1]);

      break;
    case 12:
      candyCycle(20, 40);
      //      candyCycle(inputs[0], inputs[1]);

      break;
    case 13:
      christmasCycle(20);
      //      christmasCycle(inputs[0]);
      break;
    case 14:
      Twinkle(100, 0, 0xff, 0, 10, false, false, 1);
      //      Twinkle(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] == 1, inputs[6] == 1, inputs[7]);

      break;
    case 15:
      TwinkleRandom(30, 100, false, false, 1);
      //      TwinkleRandom(inputs[0], inputs[1], inputs[2] == 1, inputs[3] == 1, inputs[4]);
      break;
    case 16:
      RunningLights(30, 0xff, 0xff, 0xff);
      //      RunningLights(inputs[0], inputs[1], inputs[2], inputs[3]);

      break;
    case 17:
      SnowSparkle(random(100, 500), 0x10, 0x10, 0x10, 100, 6);
      //       SnowSparkle(inputs[0], inputs[1], inputs[2], inputs[3], random(inputs[4], inputs[5]));
      break;
    case 18:
      BouncingBalls(3, 0xff, 0, 0);
      //      BouncingBalls(inputs[0], inputs[1], inputs[2], inputs[3]);

      break;
    case 19: {
        byte colors[3][3] = { {0xff, 0, 0}, {0xff, 0xff, 0xff}, {0, 0xff, 0} };
        //       byte colors[(sizeof(inputs)/3)/sizeof(int)][3] = { {inputs[1], inputs[2], inputs[3]}, {inputs[4], inputs[5], inputs[6]}, {inputs[7], inputs[8], inputs[9]} };
        //        byte colors[(sizeof(inputs)/3)/sizeof(int)][3];
        //        for (int i = 0; i < (sizeof(inputs)/sizeof(int)); i++) {
        //            colors[int(i/3)][i % 3] = inputs[i];
        //        }

        BouncingColoredBalls(3, colors);
        //      BouncingColoredBalls((sizeof(inputs)/3)/sizeof(int), colors);

      }
      break;

    case 20:
      Sparkle(30, 0xff, 0xff, 0xff);
      //      Sparkle(inputs[0], inputs[1], inputs[2], inputs[3]);

      break;
    case 21:
      theaterChase(50, 0xff, 0, 0);
      //      theaterChase(inputs[0], inputs[1], inputs[2], inputs[3]);

      break;
    case 22:
      theaterChaseRainbow(67, 3);
      //      theaterChaseRainbow(inputs[0], inputs[1]);

      break;
    case 23:
      candyCane(70, 6);
      break;
    case 24:
      setAll(255, 255, 255);
      break;
    case 25:
      setAll(0, 0, 0);
  }
  Serial.println("Exiting handle case");
}
