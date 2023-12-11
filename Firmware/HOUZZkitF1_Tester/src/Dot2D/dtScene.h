#ifndef __DT_SCENE_H__
#define __DT_SCENE_H__

#include "dtNode.h"

NS_DT_BEGIN



class Scene : public Node
{
public:

    static Scene *create();

    static Scene *createWithSize(const Size& size);

    virtual void render(Renderer* renderer,const Transform& transform);

    virtual void removeAllChildren() override;

    virtual void onEnter() override;

    virtual int8_t nodeClassType() override {return 1;}

protected:

    Scene();

    virtual ~Scene();
    
    virtual bool init() override;

    virtual bool initWithSize(const Size& size);

protected:

    friend class Node;

    friend class Renderer;

};

NS_DT_END


#endif //__DT_SCENE_H__
