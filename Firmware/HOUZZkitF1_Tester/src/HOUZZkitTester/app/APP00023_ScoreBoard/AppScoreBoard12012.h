#ifndef __APP_ScoreBoard_12012_H__
#define __APP_ScoreBoard_12012_H__


#include "AppScoreBoard.h"

NS_DT_BEGIN


class AppScoreBoardLayer12012 : public AppScoreBoardLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppScoreBoardLayer12012,AppScoreBoard);

};


NS_DT_END


#endif //__APP_ScoreBoard_12012_H__