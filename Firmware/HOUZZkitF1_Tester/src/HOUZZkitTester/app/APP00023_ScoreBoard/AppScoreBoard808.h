#ifndef __APP_ScoreBoard_808_H__
#define __APP_ScoreBoard_808_H__


#include "AppScoreBoard.h"

NS_DT_BEGIN


class AppScoreBoardLayer808 : public AppScoreBoardLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppScoreBoardLayer808,AppScoreBoard);

};


NS_DT_END


#endif //__APP_ScoreBoard_808_H__