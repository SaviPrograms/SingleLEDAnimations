#include <FastLED.h>
#include <EncoderButton.h>

#define LED_r 0
#define LED_g 1
#define en_r 2
#define en_l 3
#define btn 4
#define LED_b 6

EncoderButton encoder(en_l, en_r, btn);

bool power = false;
int menuOption;
int lastMenuOption;

long animationTime;
unsigned long animationDuration;
float animationDurationMultiplier;

int mode = 0;
int lastMode = -1;
uint8_t brightness = 255;
CHSV color1 = CHSV(128, 255, 255);
CHSV color2 = CHSV(128, 255, 255);

unsigned long lastMillis;

void turnPower(EncoderButton &eb)
{
   power = !power;
}

void changeMenuOption(EncoderButton &eb)
{
   menuOption++;
   if (menuOption > 4)
      menuOption = 0;
}

void menuReset()
{
   menuOption = 0;
   lastMenuOption = 0;

   animationTime = 0;
   lastMillis = millis();
}

void setup()
{
   // Ledy
   pinMode(LED_r, OUTPUT);
   pinMode(LED_g, OUTPUT);
   pinMode(LED_b, OUTPUT);

   // Encoder events
   encoder.setLongPressHandler(turnPower);
   encoder.setClickHandler(changeMenuOption);

   menuReset();
}

void setLED(const CHSV &color)
{
   const CRGB ledColor = CRGB(CHSV(color.h, color.s, scale8(color.v, brightness))); // Possiblly delete dim8
   analogWrite(LED_r, ledColor.red);
   analogWrite(LED_g, ledColor.green);
   analogWrite(LED_b, ledColor.blue);
}

void pulseLED(const CHSV &color)
{
   setLED(CHSV(0, 0, 0));
   delay(200);
   setLED(color);
   delay(200);
   setLED(CHSV(0, 0, 0));
   delay(200);
}

double animationPos(){
   return (double)animationTime / (animationDuration * animationDurationMultiplier);
}


void hueMenu(CHSV *color){
   if(lastMenuOption != menuOption)
   {
      encoder.resetPosition(color->hue);
   }
   (*color).h = encoder.position(); 
}


void menu()
{
   switch (menuOption)
   {
   case 0:
      if (lastMenuOption != 0)
      {
         pulseLED(CHSV(153, 186, 255));
         animationTime = 0;
      }
      break;
   case 1:
   hueMenu(color1);
      break;
   case 2:
   hueMenu(color2);
      break;
      
   }

   lastMenuOption = menuOption;
   return;
}


void ModeStill()
{
   setLED(color1);
}

void ModeFireplace()
{
}

void ModeGradient()
{
   if(lastMode != mode){
      animationDuration = 300;
   }

   setLED(blend(color1, color2, animationPos()))

   if (animationPos() > 1.0f)
      animationTime = 0;
}

void loop()
{
   encoder.update();
   if (!power)
      return;
   menu();

   ModeStill();

   const long currentMillis = millis();
   animationTime += currentMillis - lastMillis;
   lastMillis = currentMillis;
}
