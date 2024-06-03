#include "Sprite.h"

Sprite::Sprite(const Texture2D* texture)
{
    img = texture;
    animacionactual = -1;
    frameactual = 0;
    retardoactual = 0;
    modo = AnimMode::AUTOMATIC;
}
Sprite::~Sprite()
{
    Release();
}
void Sprite::SetNumberAnimations(int num)
{
    animations.clear();
    animations.resize(num);
}
void Sprite::SetAnimationDelay(int id, int retraso)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].retraso = retraso;
    }
}
void Sprite::AddKeyFrame(int id, const Rectangle& rect)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].frames.push_back(rect);
    }
}
void Sprite::SetAnimation(int id)
{
    if (id >= 0 && id < animations.size())
    {
        animacionactual = id;
        frameactual = 0;
        retardoactual = animations[animacionactual].retraso;
    }
}
int Sprite::GetAnimation()
{
    return animacionactual;
}
void Sprite::SetManualMode()
{
    modo = AnimMode::MANUAL;
}
void Sprite::SetAutomaticMode()
{
    modo = AnimMode::AUTOMATIC;
}
void Sprite::Update()
{
    //Both animation modes (automatic and manual) are carry out with animation delay
    if (retardoactual > 0)
    {
        retardoactual--;
        if (retardoactual == 0)
        {
            //Only automatic animation mode advances next frame
            if (modo == AnimMode::AUTOMATIC)
            {
                frameactual++;
                frameactual %= animations[animacionactual].frames.size();
                retardoactual = animations[animacionactual].retraso;
            }
        }
    }
}
void Sprite::NextFrame()
{
    //Next frame is only available in manual animation mode
    if (modo == AnimMode::MANUAL)
    {
        retardoactual--;
        if (retardoactual <= 0)
        {
            frameactual++;
            frameactual %= animations[animacionactual].frames.size();
            retardoactual = animations[animacionactual].retraso;
        }
    }
}
void Sprite::PrevFrame()
{
    //Previous frame is only available in manual animation mode
    if (modo == AnimMode::MANUAL)
    {
        retardoactual--;
        if (retardoactual <= 0)
        {
            frameactual--;
            frameactual %= animations[animacionactual].frames.size();
            retardoactual = animations[animacionactual].retraso;
        }
    }
}
void Sprite::Draw(int x, int y) const
{
    DrawTint(x, y, WHITE);
}
void Sprite::DrawTint(int x, int y, const Color& col) const
{
    if (animacionactual >= 0 && animacionactual < animations.size())
    {
        Rectangle rect = animations[animacionactual].frames[frameactual];
        DrawTextureRec(*img, rect, { (float)x, (float)y }, col);
    }
}
void Sprite::Release()
{
    //Release each animation's frames
    for (auto& animation : animations)
    {
        animation.frames.clear();
    }
    //Clear the animations vector
    animations.clear();
}
