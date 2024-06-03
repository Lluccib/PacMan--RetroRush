#pragma once

#include "RenderComponent.h"
#include <vector>

enum class AnimMode { AUTOMATIC, MANUAL };

struct Animation
{
    int retraso;
    std::vector<Rectangle> frames;
};

class Sprite : public RenderComponent
{
public:
    Sprite(const Texture2D* texture);
    ~Sprite();

    void SetNumberAnimations(int num);
    void SetAnimationDelay(int id, int retraso);
    void AddKeyFrame(int id, const Rectangle& rect);
    void SetAnimation(int id);
    int GetAnimation();

    void SetManualMode();
    void SetAutomaticMode();

    void Update();
    void NextFrame();
    void PrevFrame();

    void Draw(int x, int y) const override;
    void DrawTint(int x, int y, const Color& col) const override;

    void Release() override;

private:
    int animacionactual;


    const Texture2D* img;
    std::vector<Animation> animations;
    int frameactual;
    int retardoactual;
    AnimMode modo;
};