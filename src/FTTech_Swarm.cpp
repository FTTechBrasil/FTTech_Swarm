/***********************************************************************
 * Includes
 ***********************************************************************/

#include "FTTech_SAMD51Clicks.h"
#include "FTTech_Swarm.h"

/***********************************************************************
 * Class contructors
 ***********************************************************************/	

FT_SWARM::FT_SWARM(bool __option = false){
    __DEBUG = __option;
    _TESTFLAG = _DATEFLAG = _GPSFLAG = false;
    _PAYLOADSIZE = 0;
}


/***********************************************************************
 * Public Methods of the Class
 ***********************************************************************/

void FT_SWARM::BeginTest(void){
    if (__DEBUG == true){
        if (_TESTFLAG == 0){
            Serial.begin(9600);
            while(!Serial);
            FTClicks.begin();
            delay(1000);
            pinMode(EXTERN_LED_PIN, OUTPUT);
            digitalWrite(EXTERN_LED_PIN, LOW);
            _SWARM_SERIAL.begin(115200);
            while(!_SWARM_SERIAL);
            Serial.println("Please, use the serial input to send commands to the swarm tile.");
            Serial.println("There is no need to use $ or *XX in the commands.");
            _TESTFLAG = 1;
        }
        if (Serial.available()){
            delay (500);
            String _incomming = Serial.readStringUntil('\n');
            Serial.println("Sent command: " + _incomming);
            SendCommand(_incomming);
        }
        if (_SWARM_SERIAL.available() > 0) {
            delay(500);
            String _SWARMincomingString;
            _SWARMincomingString = _SWARM_SERIAL.readStringUntil('\n');
            Serial.println("Tile Message: " + _SWARMincomingString);
        }
    }
    return;
}

void FT_SWARM::Begin(void){
    if (__DEBUG == true){
        Serial.begin(9600);
        while(!Serial);
    }
    FTClicks.begin();
    FTClicks.turnON_5V();
    FTClicks.turnON(_SWARM_CLICK);
    delay(1000);
    pinMode(EXTERN_LED_PIN, OUTPUT);
    digitalWrite(EXTERN_LED_PIN, LOW);
    _SWARM_SERIAL.begin(115200);
    while(!_SWARM_SERIAL);
    String _okmessage;
    if (__DEBUG == true){
        Serial.println("Waiting until Tile gets its position and date/time, it should be ready in 5 minutes in an open area.");
    }
    while ((!_DATEFLAG)||(!_GPSFLAG)){
        if (__DEBUG == true){
            delay(5000);
        }else{
            FTClicks.sleepForSeconds(5);
        }
        digitalWrite(EXTERN_LED_PIN, !digitalRead(EXTERN_LED_PIN));
        if (_SWARM_SERIAL.available() > 0) {
            delay(200);
            _okmessage = _SWARM_SERIAL.readStringUntil('\n');
            if (_okmessage == "$TILE POSITION*2d"){
                _GPSFLAG = true;
                if (__DEBUG == true){
                    Serial.print("Tile Message: GPS position aquired.");
                }
            }
            if (_okmessage == "$TILE DATETIME*35"){
                _DATEFLAG = true;
                if (__DEBUG == true){
                    Serial.print("Tile Message: date and time aquired.");
                }
            }
        }
        if (__DEBUG == true){
            Serial.println(".");
        }
    }
    if (__DEBUG == true){
        Serial.println("Tile ready to send and receive messages.");
    }
    digitalWrite(EXTERN_LED_PIN, LOW);
    for(uint8_t k=0;k<4;k++){
        delay(200);
        digitalWrite(EXTERN_LED_PIN, !digitalRead(EXTERN_LED_PIN));
    }
    return;
}

void FT_SWARM::TransmitText(String _message){
    String __nothing = SendCommand("TD "+ TexttoHex(_message));
    return;
}

void FT_SWARM::TransmitText(String _message, uint16_t _appId){
    String __nothing = SendCommand("TD AI="+ String(_appId) + ","+ TexttoHex(_message));
    return;
}

void FT_SWARM::TransmitData(String _message){
    _PAYLOADSIZE = 0;
    String __nothing = SendCommand("TD " + _message);
    return;
}

void FT_SWARM::TransmitData(String _message, uint16_t _appId){
    _PAYLOADSIZE = 0;
    String __nothing = SendCommand("TD AI=" + String(_appId) + ","+ TexttoHex(_message));
    return;
}

void FT_SWARM::Sleep(uint32_t _time){
    if (_time < 5) {
        if (__DEBUG == true){
            Serial.println("Error: Sleeping time must be greater than 5 seconds.");
        }
    return;
    }
    SendCommand("SL S=" + String(_time));
    String __dump = ShowDebug();
    FTClicks.sleepForSeconds(_time-1);
    return;
}

String FT_SWARM::ReturnDateHex(void){
    String _date = SendCommand("DT @");
    _date = TurnDatetoHex(_date.substring(4,18));
    return _date;
}

String FT_SWARM::ToHex(float float_input){
    union{
        float floater;
        uint8_t Byarray [(sizeof(float)/sizeof(uint8_t))];
    }_operator;
    _operator.floater = float_input;
    String _float_hex = "";
    for (uint8_t i=0;i<(sizeof(float)/sizeof(uint8_t));i++){
        _float_hex = _float_hex + ToHex(_operator.Byarray[i]);
    }
    return _float_hex;
}

String FT_SWARM::ToHex (uint8_t _8bits){
    uint8_t _l_nib = _8bits & 0x0F;
    uint8_t _h_nib = _8bits >> 4;
    String _hexby = String(_h_nib,HEX) + String(_l_nib,HEX);
    _PAYLOADSIZE = _PAYLOADSIZE + 2;
    return _hexby;
}

String FT_SWARM::ToHex(uint16_t _16bits){
    uint8_t _l_half = _16bits;
    uint8_t _h_half = _16bits >> 8;
    String _hexby = ToHex(_h_half) + ToHex(_l_half);
    return _hexby;
}

String FT_SWARM::ToHex(uint32_t _32bits){
    uint16_t _l_half = _32bits;
    uint16_t _h_half = _32bits >> 16;
    String _hexby = ToHex(_h_half) + ToHex(_l_half);
    return _hexby;
}

String FT_SWARM::ToHex(uint64_t _64bits){
    uint32_t _l_half = _64bits;
    uint32_t _h_half = _64bits >> 32;
    String _hexby = ToHex(_h_half) + ToHex(_l_half);
    return _hexby;
}

/***********************************************************************
 * Private Methods of the Class
 ***********************************************************************/

String FT_SWARM::TexttoHex(String _messageTXT){
    String _messageHEX = "";
    uint8_t _operator, _remain, _quot;
    for (uint16_t _indx;_indx<_messageTXT.length();_indx++){
        _operator = _messageTXT.charAt(_indx);
        _remain = _operator%16;
        _quot = (_operator-_remain)/16;
        _messageHEX = _messageHEX + String(_quot,HEX) + String(_remain,HEX);
    }
    return _messageHEX;
}

String FT_SWARM::ShowDebug(void){
    delay(500); // Waits until serial is available
    String _incomingString;
    if (_SWARM_SERIAL.available() > 0) {
        _incomingString = _SWARM_SERIAL.readStringUntil('\n');
        if (__DEBUG == true){
            Serial.print("Tile Message: ");
            Serial.println(_incomingString);
        }
    }
    return _incomingString;
}

String FT_SWARM::SendCommand(String _tileComm){
    String __realcomm = "$"+ _tileComm +"*"+ doCheckSum(_tileComm);
    _SWARM_SERIAL.println(__realcomm);
    String __dump = ShowDebug();
    return __dump;
}

String FT_SWARM::doCheckSum(String _ckdString){
    uint8_t __i = 0;
    uint8_t __cs = 0;
    for (__i = 0;__i < _ckdString.length(); __i++)
        __cs ^= ((uint8_t) _ckdString.charAt(__i));
    if (__cs < 16)
        return (String(0)+String(__cs,HEX));
    else
        return String(__cs,HEX);
}

String FT_SWARM::TurnDatetoHex(String _fulldate){
    
    String _year    = _fulldate.substring(0,4);
    String _month   = _fulldate.substring(4,6);
    String _day     = _fulldate.substring(6,8);
    String _hour    = _fulldate.substring(8,10);
    String _minute  = _fulldate.substring(10,12);
    String _second  = _fulldate.substring(12);

    String _hexdate = CompleteHex(String(_year.toInt(),HEX),3) + String(_month.toInt(),HEX) +
    CompleteHex(String(_day.toInt(),HEX),2) + CompleteHex(String(_hour.toInt(),HEX),2) + 
    CompleteHex(String(_minute.toInt(),HEX),2) + CompleteHex(String(_second.toInt(),HEX),2);

    return _hexdate;
}

String FT_SWARM::CompleteHex(String _notsurestr, uint8_t _besize){
    String _surestr = _notsurestr;
    while(_surestr.length()<_besize){
        _surestr = "0" + _surestr;
    }
    return _surestr;
}