#ifndef __SDTApp_H__
#define __SDTApp_H__

#include "Dot2D/base/dtMacros.h"
#include "Dot2D/dtScene.h"

NS_DT_BEGIN

class SDTApp : public Scene
{
private:

    

protected:

    bool init();

    void customUpdate(float dt);
    
public:

    SDTApp();

    ~SDTApp();

    static SDTApp* create();

    void update(float delta);

};



NS_DT_END


#endif //__SDTApp_H__
