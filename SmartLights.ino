#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <HttpRequest.h>


#define PIN 5
int NUM_LEDS = 200;
int oldNumLeds = NUM_LEDS;
bool changeLEDNum = false;
bool clearStripBeforehand = false;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);

//To do - Make array of inputs
//If receive index, set index of effect
//If don't receive index, clear array and stuff
int ledMode = 12;
double inputs[10] = {0xff, 0x77, 0x00, 3, 100, 1000, 0, 0, 0, 0};
int inputCount = 0;
int speedSpecial = false;

//WiFi stuff
char ssid[] = "StoffelNetwork";      //  your network SSID (name)
char pass[] = "9529471171";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
char mdnsName[] = "ledlightsboard"; // the MDNS name that the board will respond to


int status = WL_IDLE_STATUS;
// Create a MDNS responder to listen and respond to MDNS name requests.
WiFiServer server(80);
HttpRequest httpReq;


//http://forum.arduino.cc/index.php?topic=109579.0
//https://forum.arduino.cc/index.php?topic=386829.0
//http://blog.nyl.io/esp8266-led-arduino/


#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif
void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
//WiFi.setPins(8,7,4,2);
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      while (true);       // don't continue
    }
  
    // attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(ssid);                   // print the network name (SSID);
  
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      delay(10000);
    }
    server.begin();                           // start the web server on port 80
    printWifiStatus();                        // you're connected now, so print out the status

  




strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void loop() {
  // put your main code here, to run repeatedly:

 

  //Wifi stuff here to get LED Mode
   checkWifi();
   if (clearStripBeforehand) {
    clearStrip();
    clearStripBeforehand = false;
   }
  if (changeLEDNum) {
    setNumLEDS(NUM_LEDS);
  }
  handleCase(ledMode, inputs, speedSpecial);
  
/*
  //Strobe
  // Slower:
  //Strobe(0xff, 0x77, 0x00, 3, 100, 1000);
  // Fast:
//  Strobe(0xff, 0xff, 0xff, 10, 50, 1000);



  //Cylon
//  CylonBounce(0xff, 0, 0, 4, 10, 50);



//Fire
//Fire(55,120,15);



//Color Wipe
// colorWipe(0x00,0xff,0x00, 50);
//  colorWipe(0x00,0x00,0x00, 50);



  //Fade red white and blue
//  FadeInOut(0xff, 0x00, 0x00, 5, 5); // red
//  FadeInOut(0xff, 0xff, 0xff, 5, 5); // white 
//  FadeInOut(0x00, 0x00, 0xff, 5, 5); // blue



  //Halloween Eyes
// Fixed:
//   HalloweenEyes(0xff, 0x00, 0x00, 1,4, true, 10, 80, 100);
  // or Random:
//  HalloweenEyes(0xff, 0x00, 0x00, 1, 4, true, random(5,50), random(50,150), random(0, 3000));
//  HalloweenEyes(0x00, 0xff, 0x00, 1, 4, true, random(5,50), random(50,150), random(0, 3000));



//New Kitt
//  NewKITT(0xff, 0, 0, 8, 10, 50);




//Rainbow Cycle
//  rainbowCycle(20);

//Cool Cycle = 2
//50 is optimal purple
//  coolCycle(20, 50);

//warm Cycle = 1
//70 is optimal green
//  warmCycle(20, 150);


//Nature cycle = 3
//50 is good
//natureCycle(20, 50);

//Christmas cycle
//Fades red and green
//candyCycle(20, 40);

//christmasCycle(20);




//Twinkle - twinkle random is better
//  Twinkle(0xff, 0, 0, 10, 100, false, false, 1);



  
//Rainbow Twinkle
//Find out way to clear off the last ones lit but not all
//  TwinkleRandom(30, 100, false, false, 7);


  
//Running Lights
//Fix weird flashing
//  RunningLights(0xff,0xff,0x00, 30);





//Snow Sparkle
//  SnowSparkle(0x10, 0x10, 0x10, 100, random(100,500));





//Bouncing Balls
//  BouncingBalls(0xff,0,0, 3);

//Multi Colored Bouncing Balls - like this one, but options
//byte colors[3][3] = { {0xff, 0,0}, {0xff, 0xff, 0xff}, {0, 0xff, 0} };

//  BouncingColoredBalls(3, colors);
  



//Sparkle
//  Sparkle(0xff, 0xff, 0xff, 30);



//Theater Chase
//  theaterChase(0xff,0,0,50);

//  theaterChaseRainbow(67, 3);

*/
}

void setNewNumberOfLEDs(int ledNum) {
  oldNumLeds = NUM_LEDS;
  NUM_LEDS = ledNum;
  changeLEDNum = true;
}

void setNumLEDS(int ledNum) {
  clearStrip(oldNumLeds);
  strip = Adafruit_NeoPixel(ledNum, PIN, NEO_RGB + NEO_KHZ800);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  changeLEDNum = false;
}

bool checkWifi() {

// Call the update() function on the MDNS responder every loop iteration to
  // make sure it can detect and respond to name requests.

  
  bool didStuff = false;
  char name[HTTP_REQ_PARAM_NAME_LENGTH], value[HTTP_REQ_PARAM_VALUE_LENGTH];

WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
       
        //parse the received caracter
        httpReq.parseRequest(c);
        
        Serial.write(c);
        
        //IF request has ended -> handle response
        if (httpReq.endOfRequest()) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
      client.println("<body>");

          //access object properties
          client.print("Method: ");
          client.print(httpReq.method);
          client.println("<br>");
          client.print("Uri: ");
          client.print(httpReq.uri);
          client.println("<br>");
          client.print("Version: ");
          client.print(httpReq.version);
          client.println("<br>");
          client.print("paramCount: ");
          client.print(httpReq.paramCount);
          client.println("<br>");
          //list received parameters GET and POST
      client.println("Parameters:<br>");
          for(int i=1;i<=httpReq.paramCount;i++){
            httpReq.getParam(i,name,value);
            client.print(name);
            client.print("-");
            client.print(value);
            client.println("<br>");
          }
          //list received cookies
          client.println("Cookies:<br>");
          for(int i=1;i<=httpReq.cookieCount;i++){
            httpReq.getCookie(i,name,value);
            client.print(name);
            client.print(" - ");
            client.print(value);
            client.println("<br>");
          }
//          int input = atoi(value);
//            Serial.println("After atoi");
//            inputs[i-1] = input;
//          printInputs();

          //find a particular parameter name
          int pos=httpReq.getParam("style",value);
          if(pos>0){
            client.print("<br>");
            client.print("Found 'style'. Value: ");
            client.print(value);
            ledMode = atoi(value) + 1;
            clearStripBeforehand = true;
            Serial.print("LED Mode:");
            Serial.println(ledMode);
            client.println("<br>");  
          }

          //find a particular parameter name
          pos=httpReq.getParam("numberOfLEDs",value);
          if(pos>0){
            client.print("<br>");
            client.print("Found 'numberOfLEDs'. Value: ");
            client.print(value);
            setNewNumberOfLEDs(atoi(value));
//            clearStrip();
            Serial.print("Number of LEDs:");
            Serial.println(NUM_LEDS);
            client.println("<br>");  
          }

          pos=httpReq.getParam("speed",value);
          speedSpecial = pos > 0;
          if(pos>0){
            client.print("<br>");
            client.print("Found 'speed'. Value: ");
            client.print(value);
            client.println("<br>");  
          }
          
          clearInputs();
          if (speedSpecial) {
            inputs[0] = atoi(value);
          } else {
            Serial.println("Inputs");
            int lookInputCount = 0;
            for(int i = 0; i < sizeof(inputs) / sizeof(int); i++) {
              String input = "input";
              input += i;
              char charBuf[10];
              input.toCharArray(charBuf, 50);
              Serial.print("CharBuf: ");
             Serial.println(charBuf);
              int pos=httpReq.getParam(charBuf,value);
              if(pos>0){
                Serial.println(charBuf);
                inputs[lookInputCount] = atoi(value);
                lookInputCount++;
              }
              inputCount = lookInputCount;
            }
          }
          
          Serial.println();
          printInputs();
            
          client.println("</body>");
          client.print("</html>");
//          Serial.println("Setting distuff to true");
          didStuff = true;

         
          
          //Reset object and free dynamic allocated memory
          httpReq.resetRequest();
          
          break;

      }
        
        
        /*
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(9, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(9, LOW);                // GET /L turns the LED off
        }
        */
      }
    }

    // xmlhttp.open("GET", "http://ipAddressOfArduino?r=" + redVal + "&g=" + greenVal + "&b=" + blueVal + "&e", true);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  if (didStuff) {
    Serial.print("Returning Stuff: ");
    Serial.println(didStuff);
  }
  
  return didStuff;
  
}

void clearInputs() {
  for(int i = 0; i < sizeof(inputs)/sizeof(int); i++) {
    inputs[i] = 0;
  }
}

void printInputs() {
  Serial.print("Inputs: ");
  for(int i = 0; i < sizeof(inputs)/sizeof(int); i++) {
    Serial.print(inputs[i]);
    Serial.print(", ");
  }
  Serial.println();
}


void handleCase(int type, double inputs[], bool speedDelayIf) {

  if (speedDelayIf) {
//    Serial.println("Special Speed case");
    int speedVal = inputs[0];
    switch (ledMode) {

    case 1: 
      Strobe(speedVal, 0xff, 0x77, 0x00, 3, 1000);
//      Strobe(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
    break;
    case 2: 
      CylonBounce(speedVal, 0xff, 0, 0, 4, 50, 1.5);
//      CylonBounce(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
    break;
    case 3: 
      Fire(speedVal, 55,120);
//      Fire(inputs[0], inputs[1], inputs[2]);

    break;
    case 4: 
      colorWipe(speedVal, 0x00,0xff,0x00);
//      colorWipe(inputs[0], inputs[1], inputs[2], inputs[3]);
      colorWipe(speedVal, 0x00,0x00,0x00);
    break;
    case 5: 
      FadeInOut(speedVal, speedVal, 0xff, 0x00, 0x00); // red
      FadeInOut(speedVal, speedVal, 0xff, 0xff, 0xff); // white 
      FadeInOut(speedVal, speedVal, 0x00, 0x00, 0xff); // blue

//      FadeInOut(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]); // red
    break;
    case 6: 
      HalloweenEyes(random(50,150), 0x00, 0xff, 0x00, 1, 4, true, random(5,50), random(0, 3000));
//      HalloweenEyes(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] == 1, inputs[6], inputs[7], inputs[8]);
   break;
    case 7: 
      NewKITT(speedVal, 0xff, 0, 0, 30, 10);
//      NewKITT(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

    break;
    case 8: 
      rainbowCycle(speedVal);
//      rainbowCycle(inputs[0]);
    break;
    case 9: 
      coolCycle(speedVal, 50);
//      coolCycle(inputs[0], inputs[1]);
    break;
    case 10: 
      warmCycle(speedVal, 150);
//      warmCycle(inputs[0], inputs[1]);

    break;
    case 11: 
      natureCycle(speedVal, 50);
//      natureCycle(inputs[0], inputs[1]);

    break;
    case 12: 
      candyCycle(speedVal, 40);
//      candyCycle(inputs[0], inputs[1]);

    break;
    case 13: 
      christmasCycle(speedVal);
//      christmasCycle(inputs[0]);
    break;
    case 14: 
      Twinkle(speedVal, 0xff, 0, 0, 10, false, false, 1);
//      Twinkle(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] == 1, inputs[6] == 1, inputs[7]);

    break;
    case 15: 
      TwinkleRandom(speedVal, 100, false, false, 7);
//      TwinkleRandom(inputs[0], inputs[1], inputs[2] == 1, inputs[3] == 1, inputs[4]);
    break;
    case 16: 
      RunningLights(speedVal, 0xff,0xff,0x00);
//      RunningLights(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 17: 
       SnowSparkle(speedVal, 0x10, 0x10, 0x10, 100, 6);
//       SnowSparkle(inputs[0], inputs[1], inputs[2], inputs[3], random(inputs[4], inputs[5]));
    break;
    case 18: 
      BouncingBalls(3, 0xff,0,0);
//      BouncingBalls(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 19: {      
      byte colors[3][3] = { {0xff, 0,0}, {0xff, 0xff, 0xff}, {0, 0xff, 0} };
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
      Sparkle(speedVal, 0xff, 0xff, 0xff);
//      Sparkle(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 21: 
      theaterChase(speedVal, 0xff,0,0);
//      theaterChase(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 22: 
      theaterChaseRainbow(speedVal, 3);
//      theaterChaseRainbow(inputs[0], inputs[1]);

    break;
    
   }
  } else if (inputCount != 0) {
//    Serial.println("Inputs case");
    switch (ledMode+1) {

    case 1: 
//      Strobe(100, 0xff, 0x77, 0x00, 3, 1000);
      Strobe(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
    break;
    case 2: 
//      CylonBounce(10, 0xff, 0, 0, 4, 50, 1.5);
      CylonBounce(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
    break;
    case 3: 
//      Fire(15, 55, 120);
      Fire(inputs[0], inputs[1], inputs[2]);

    break;
    case 4: 
//      colorWipe(50, 0x00,0xff,0x00);
      colorWipe(inputs[0], inputs[1], inputs[2], inputs[3]);
      colorWipe(50, 0x00,0x00,0x00);
    break;
    case 5: 
//      FadeInOut(5, 0xff, 0x00, 0x00, 5); // red
//      FadeInOut(5, 0xff, 0xff, 0xff, 5); // white 
//      FadeInOut(5, 0x00, 0x00, 0xff, 5); // blue

      FadeInOut(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]); // red
    break;
    case 6: 
//      HalloweenEyes(0x00, 0xff, 0x00, 1, 4, true, random(5,50), random(50,150), random(0, 3000));
      HalloweenEyes(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] >= 1, inputs[6], inputs[7], inputs[8]);
   break;
    case 7: 
//      NewKITT(0xff, 0, 0, 8, 50, 10);
      NewKITT(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

    break;
    case 8: 
//      rainbowCycle(20);
      rainbowCycle(inputs[0]);
    break;
    case 9: 
//      coolCycle(20, 50);
      coolCycle(inputs[0], inputs[1]);
    break;
    case 10: 
//      warmCycle(20, 150);
      warmCycle(inputs[0], inputs[1]);

    break;
    case 11: 
//      natureCycle(20, 50);
      natureCycle(inputs[0], inputs[1]);

    break;
    case 12: 
//      candyCycle(20, 40);
      candyCycle(inputs[0], inputs[1]);

    break;
    case 13: 
//      christmasCycle(20);
      christmasCycle(inputs[0]);
    break;
    case 14: 
//      Twinkle(0xff, 0, 0, 10, 100, false, false, 1);
      Twinkle(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] >= 1, inputs[6] >= 1, inputs[7]);

    break;
    case 15: 
//      TwinkleRandom(30, 100, false, false, 7);
      TwinkleRandom(inputs[0], inputs[1], inputs[2] >= 1, inputs[3] >= 1, inputs[4]);
    break;
    case 16: 
//      RunningLights(0xff,0xff,0x00, 30);
      RunningLights(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 17: 
//       SnowSparkle(0x10, 0x10, 0x10, 100, random(100,500));
       SnowSparkle(inputs[0], inputs[1], inputs[2], inputs[3], random(inputs[4], inputs[5]), inputs[6]);
    break;
    case 18: 
//      BouncingBalls(0xff,0,0, 3);
      BouncingBalls(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 19: {      
//      byte colors[3][3] = { {0xff, 0,0}, {0xff, 0xff, 0xff}, {0, 0xff, 0} };
//       byte colors[(sizeof(inputs)/3)/sizeof(int)][3] = { {inputs[1], inputs[2], inputs[3]}, {inputs[4], inputs[5], inputs[6]}, {inputs[7], inputs[8], inputs[9]} };  
        byte colors[(sizeof(inputs)/3)/sizeof(int)][3];
        for (int i = 1; i < (sizeof(inputs)/sizeof(int)); i++) {
            colors[int(i/3)][i % 3] = inputs[i];
        }

//      BouncingColoredBalls(3, colors);
      BouncingColoredBalls((sizeof(inputs)/3)/sizeof(int), colors);

    }
    break;
    
    case 20: 
//      Sparkle(0xff, 0xff, 0xff, 30);
      Sparkle(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 21: 
//      theaterChase(0xff,0,0,50);
      theaterChase(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 22: 
//      theaterChaseRainbow(67, 3);
      theaterChaseRainbow(inputs[0], inputs[1]);

    break;
    
   }
  } else {
//    Serial.println("Default Effects");
    switch (ledMode+1) {

    case 1: 
      Strobe(100, 0xff, 0, 0, 3, 1000);
//      Strobe(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
    break;
    case 2: 
      CylonBounce(10, 0xff, 0, 0, 30, 30, 1);
//      CylonBounce(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
    break;
    case 3: 
      Fire(5, 55, 120);
//      Fire(inputs[0], inputs[1], inputs[2]);

    break;
    case 4: 
      colorWipe(10, 0x00,0xff,0x00);
//      colorWipe(inputs[0], inputs[1], inputs[2], inputs[3]);
      colorWipe(5, 0x00,0x00,0x00);
    break;
    case 5: 
      FadeInOut(.5, .5, 0xff, 0x00, 0x00); // red
      FadeInOut(.5, .5, 0xff, 0xff, 0xff); // white 
      FadeInOut(.5, .5, 0x00, 0x00, 0xff); // blue

//      FadeInOut(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]); // red
    break;
    case 6: 
      HalloweenEyes(random(50,150), 0x00, 0xff, 0x00, 1, 4, true, random(5,50), random(0, 3000));
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
      coolCycle(20, 50);
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
      Twinkle(100, 0xff, 0, 0, 10, false, false, 1);
//      Twinkle(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5] == 1, inputs[6] == 1, inputs[7]);

    break;
    case 15: 
      TwinkleRandom(30, 100, false, false, 2);
//      TwinkleRandom(inputs[0], inputs[1], inputs[2] == 1, inputs[3] == 1, inputs[4]);
    break;
    case 16: 
      RunningLights(30, 0xff,0xff,0xff);
//      RunningLights(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 17: 
       SnowSparkle(random(100,500), 0x10, 0x10, 0x10, 100, 6);
//       SnowSparkle(inputs[0], inputs[1], inputs[2], inputs[3], random(inputs[4], inputs[5]));
    break;
    case 18: 
      BouncingBalls(3, 0xff,0,0);
//      BouncingBalls(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 19: {      
      byte colors[3][3] = { {0xff, 0,0}, {0xff, 0xff, 0xff}, {0, 0xff, 0} };
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
      theaterChase(50, 0xff,0,0);
//      theaterChase(inputs[0], inputs[1], inputs[2], inputs[3]);

    break;
    case 22: 
      theaterChaseRainbow(67, 3);
//      theaterChaseRainbow(inputs[0], inputs[1]);

    break;
     case 23:
      candyCane(40, 6);
    break;
    case 24:
    setAll(0, 0, 0);
   }
  }
   
}


