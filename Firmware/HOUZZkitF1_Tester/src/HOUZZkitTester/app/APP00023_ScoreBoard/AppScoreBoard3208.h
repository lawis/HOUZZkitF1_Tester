#ifndef __APP_ScoreBoard_3208_H__
#define __APP_ScoreBoard_3208_H__


#include "AppScoreBoard.h"

NS_DT_BEGIN


class AppScoreBoardLayer3208 : public AppScoreBoardLayer
{
protected:

    TextSprite *redSide;

    TextSprite *blueSide;

    CanvasSprite *middle;

    CanvasSprite *winCountLine;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppScoreBoardLayer3208,AppScoreBoard);

    void numberUpdate(float dt);

};


NS_DT_END


#endif //__APP_ScoreBoard_3208_H__