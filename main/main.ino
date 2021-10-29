#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET //小米
#include <Servo.h>
 
#define PIN_SERVO D0 //舵机信号线
 
Servo myservo;
 
#include <Blinker.h>
 
char auth[] = "";//点灯秘钥
char ssid[] = "";//无线网名称
char pswd[] = "";//无线网密码
 
bool oState = false;

//小米
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);
    myservo.attach(PIN_SERVO);
 
    if (state == BLINKER_CMD_ON) {
        myservo.write(40);//舵机正转度数
        delay(3000); 
        myservo.write(70);//舵机归位
        delay(1000); 
        
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
 
        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        myservo.write(100);//舵机反转度数
        delay(3000); 
        myservo.write(70);//舵机归位
        delay(1000); 
        
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();  
 
        oState = false;
    }
}
 
//小米
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);
 
    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}
 
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
 
    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}
 
void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
 
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
 
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
}
 
void loop()
{
    Blinker.run();
}
