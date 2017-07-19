/*
** Haaf's Game Engine 1.9
** hge.relishgames.com
**
** hgeFader helper class header
**
** Created by Scott Conner
*/


#ifndef HGEFADER_H
#define HGEFADER_H

#include "hge.h"

class hgeFader
{
public:
    hgeFader(HGE* hge);
    ~hgeFader();

    void update(float delta);
    void render();

    void FadeIn(float duration);
    void FadeOut(float duration);

    bool DoneFading();

private:
    HGE*    m_hge;
    hgeQuad fader;
    float   alpha;
    int     direction;
    float   speed;
};

#endif
