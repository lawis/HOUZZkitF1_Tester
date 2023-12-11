#ifndef __DOT2D_H__
#define __DOT2D_H__

#define DOT2D_VERSION 0x00000100



//base
#include "base/dtMacros.h"
#include "base/dtVector.h"
#include "base/dtTypes.h"
#include "base/dtEventButton.h"
#include "base/dtEventPS2.h"
#include "base/dtEventListenerPS2.h"
#include "base/dtEventListenerButton.h"


//dot2d
#include "dtNode.h"
#include "dtDirector.h"
#include "dtScene.h"
#include "dtLayer.h"
#include "dtSprite.h"
#include "dtActionManager.h"
#include "dtActionInterval.h"
#include "dtActionInstant.h"


//platform
#include "platform/dtFileUtils.h"

NS_DT_BEGIN

const char* dot2dVersion();

NS_DT_END


#endif // __DOT2D_H__
