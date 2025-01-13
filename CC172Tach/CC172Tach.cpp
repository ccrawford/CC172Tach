#include "CC172Tach.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "Art\rpmGaugeBackground.h"
#include "Art\tachNeedle.h"
#include "ArialBold16.h"

#define MF_CUSTOMDEVICE_POLL_MS 100
#define MF_CUSTOMDEVICE_HAS_UPDATE
#define MF_CUSTOMDEVICE_SUPPORT 1

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite mainSpr = TFT_eSprite(&tft); //Full screen main background

TFT_eSprite tachNeedleSpr = TFT_eSprite(&tft); //Needle for tachometer


CC172Tach::CC172Tach()
{
}

void CC172Tach::begin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    tft.begin();
    tft.initDMA();
    tft.setRotation(3);
    tft.fillScreen(TFT_DARKGREY);

    mainSpr.createSprite(BackgroundWidth, BackgroundHeight);
    mainSpr.setSwapBytes(true);
    mainSpr.pushImage(0, 0, BackgroundWidth, BackgroundHeight, RpmGaugeBackground);
    mainSpr.pushSprite(0, 0);
    // tft.setTextColor(TFT_LIGHTGREY);
    mainSpr.loadFont(ArialBold16);
    // tft.showFont(1000);

    tachNeedleSpr.createSprite(TachNeedleWidth, TachNeedleHeight);
    tachNeedleSpr.setSwapBytes(true);
    tachNeedleSpr.setPivot(TachNeedleCenter_x, TachNeedleCenter_y);
    tachNeedleSpr.pushImage(0, 0, TachNeedleWidth, TachNeedleHeight, TachNeedle);
    
}

void CC172Tach::attach()
{
}

void CC172Tach::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void CC172Tach::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    
    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case 0:
         _curRpm = atoi(setPoint);
        break;
    case 1:
         _curHours = atof(setPoint); 
        break;
    case 2:
        /* code */
        break;
    default:
        break;
    }
}

unsigned long lastMillis = 0;

void CC172Tach::update()
{
    
    
   // if(micros() - lastMicros > 1000 )
     //{
    //     _curRpm += 10;
    //     lastMicros = micros();
    // } 
    
    //if (_curRpm > 3000) _curRpm = 0;
                       
    unsigned long millis0, millis1, millis2, millis3;
    millis0 = millis() ;
;
    mainSpr.pushImage(0, 0, BackgroundWidth, BackgroundHeight, RpmGaugeBackground);

    millis1 = millis();

    updateRPM();
    millis2 = millis();

    updateEngineHours(); 

    millis3 = millis();

    // mainSpr.setTextColor(TFT_WHITE, TFT_BLACK, true);
    // mainSpr.setCursor(30, 50);
    // mainSpr.setTextWrap(false);
    // mainSpr.printf("0: %lu 1: %lu 2: %lu 3:%lu", millis0 - lastMillis, millis1 - millis0, millis2 - millis1, millis3 - millis2);

    mainSpr.pushSprite(0, 0, TFT_RED);

    lastMillis = millis();
}



bool CC172Tach::updateRPM()
{
    _curRpm = min(3500, _curRpm);
    _curRpm = max(0, _curRpm);
    double angle = ((double)_curRpm - 500.0)/13.889 ;  

    mainSpr.setPivot(BackgroundCenter_x, BackgroundCenter_y);
    tachNeedleSpr.pushRotated( &mainSpr, (int16_t)angle, TFT_BLACK);
  
    // Try pushing straight to the screen
    //tft.setPivot(BackgroundCenter_x, BackgroundCenter_y);  
    //tachNeedleSpr.pushRotated( (int16_t)angle, TFT_BLACK);
    

    

    return true;
}

void left_pad_float(float num, char* padded_number, char* decimal_part) {
    // Truncate to get integer and first decimal digit
    int integer_part = (int)num;
    int decimal = (int)((num * 10) - (integer_part * 10));

    // Format padded number with spaces
    sprintf(padded_number, "%d  %d  %d  %d  %d ", 
            (integer_part / 10000) % 10, 
            (integer_part / 1000) % 10, 
            (integer_part / 100) % 10, 
            (integer_part / 10) % 10, 
            integer_part % 10);

    // Convert decimal part to string
    sprintf(decimal_part, " %d", decimal);
}

bool CC172Tach::updateEngineHours()
{
    static double lastEngineHours = 0;
   // if (lastEngineHours != _curHours)
    {
        char hours[18];
        char tenths[6];
        left_pad_float(_curHours, hours, tenths);
        lastEngineHours = _curHours;

        // Testing!
        // _curHours += 0.1;
        
        mainSpr.setCursor(101, 207);
        mainSpr.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        mainSpr.print(hours);
        mainSpr.setTextColor(TFT_BLACK, TFT_TENTHS_BACKGROUND_GREY, true);
        mainSpr.print(tenths);
    }
    return true;
}