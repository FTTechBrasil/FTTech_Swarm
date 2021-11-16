#ifndef FTTech_Swarm
#define FTTech_Swarm

/**************************************************************************!
    @brief  FTTech Tile Communication
    Currently Supports: 
        SmartNode Swarm;

***************************************************************************/

#include "FTTech_SAMD51Clicks.h"

#define _SWARM_SERIAL Serial2

#define _SWARM_CLICK 2

class FT_SWARM{
	public:
        // Constructor
        FT_SWARM(bool __option);

        // Methods
        void BeginTest(void);
        void Begin(void);
        void TransmitText(String _message);
        void TransmitText(String _message, uint16_t _appId);
        void TransmitData(String _message);
        void TransmitData(String _message, uint16_t _appId);
        void Sleep(uint32_t _time);
        String ReturnDateHex(void);
        String ToHex(float float_input);
        String ToHex(uint8_t _8bits);
        String ToHex(uint16_t _16bits);
        String ToHex(uint32_t _32bits);
        String ToHex(uint64_t _64bits);

    private:
        // Variables
        bool _TESTFLAG, _DATEFLAG, _GPSFLAG, __DEBUG;
        uint16_t _PAYLOADSIZE;
        const uint16_t _MAXPAYLOAD = 382;

        // Methods
        String TexttoHex(String _messageTXT);
        String ShowDebug(void);
        String SendCommand(String _tileComm);
        String doCheckSum(String _ckdString);
        String TurnDatetoHex(String _fulldate);
        String CompleteHex(String _notsurestr, uint8_t _besize);

};

#endif
