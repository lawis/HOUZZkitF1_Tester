#include "dtScene.h"
#include "dtDirector.h"

NS_DT_BEGIN

Scene* Scene::create()
{
    Scene *ret = new Scene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* Scene::createWithSize(const Size& size)
{
    Scene *ret = new Scene();
    if (ret && ret->initWithSize(size))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

void Scene::render(Renderer* renderer,const Transform& transform)
{
    this->visit(renderer,transform);
}

void Scene::removeAllChildren()
{
    Node::removeAllChildren();
}

void Scene::onEnter()
{
    Node::onEnter();
}

Scene::Scene()
{
    setAnchorPoint(Vec2(0,0));
}

Scene::~Scene()
{
    
}

bool Scene::init()
{
    Size size = Director::getInstance()->getCanvasSize();
    return initWithSize(size);
}

bool Scene::initWithSize(const Size& size)
{
    setContentSize(size);
    return true;
}

NS_DT_END