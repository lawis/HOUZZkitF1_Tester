#ifndef __APP_ScoreBoard_16016_H__
#define __APP_ScoreBoard_16016_H__


#include "AppScoreBoard.h"

NS_DT_BEGIN


class AppScoreBoardLayer16016 : public AppScoreBoardLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppScoreBoardLayer16016,AppScoreBoard);

};


NS_DT_END


#endif //__APP_ScoreBoard_16016_H__