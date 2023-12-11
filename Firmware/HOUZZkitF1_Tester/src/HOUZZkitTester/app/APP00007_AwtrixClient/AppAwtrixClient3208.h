#ifndef __APP_AwtrixClient_3208_H__
#define __APP_AwtrixClient_3208_H__


#include "AppAwtrixClient.h"

NS_DT_BEGIN


class AppAwtrixClientLayer3208 : public AppAwtrixClientLayer
{
protected:

    WiFiClient* espClient = nullptr;
    
    PubSubClient* mqttClient = nullptr;

    SpriteCanvas* renderCanvas = nullptr;

    SpriteCanvas* canvas = nullptr;

    void canvasShow();

    void sendMsgToServer(String s);

public:

    ~AppAwtrixClientLayer3208();

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppAwtrixClientLayer3208,AppAwtrixClient);

    void update(float dt);

    void updateMatrix(const uint8_t* payload,const uint32_t length);
};


NS_DT_END


#endif //__APP_AwtrixClient_3208_H__