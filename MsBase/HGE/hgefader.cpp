/*
** Haaf's Game Engine 1.9
** hge.relishgames.com
**
** hgeFader helper class implementation
*/

#include "hgefader.h"

//HGE *hgeFader::hge = 0;

hgeFader::hgeFader(HGE* hge)
{
    m_hge = hge;
    // get hge, released on delete
    //hge = hgeCreate(HGE_VERSION);

    //sets up the quad to the coords of the screen
    fader.v[0].x = 0;
    fader.v[0].y = 0;
    fader.v[0].z = 1.0f;
    fader.v[1].x = (float)m_hge->System_GetState(HGE_SCREENWIDTH);
    fader.v[1].y = 0;
    fader.v[1].z = 1.0f;
    fader.v[2].x = (float)m_hge->System_GetState(HGE_SCREENWIDTH);
    fader.v[2].y = (float)m_hge->System_GetState(HGE_SCREENHEIGHT);
    fader.v[2].z = 1.0f;
    fader.v[3].x = 0;
    fader.v[3].y = (float)m_hge->System_GetState(HGE_SCREENHEIGHT);
    fader.v[3].z = 1.0f;

    fader.tex = 0;
    fader.blend = BLEND_DEFAULT;

    // default to fade in for 1.5 seconds
    FadeIn(1.5);
}

hgeFader::~hgeFader()
{
    m_hge->Release();
}

void hgeFader::update(float delta)
{
    if (direction == 0) return;

    alpha += float(direction) * delta * speed * 255.0f;

    if (alpha >= 255)
    {
        alpha = 255;
        direction = 0;
    }
    else if (alpha <= 0)
    {
        direction = 0;
        alpha = 0;
    }

    // fade to black
    for (int x = 0; x < 4; x++)
    {
        fader.v[x].col = ARGB(alpha, 0, 0, 0);
    }
}

void hgeFader::render()
{
    if (alpha != 0)
    {
        m_hge->Gfx_RenderQuad(&fader);
    }
}

void hgeFader::FadeIn(float duration)
{
    direction = -1;
    speed = 1.0f / duration;

    //starts the alpha at full black, and is ready to fade in
    alpha = 255;

    //sets the color of the quad
    for (int x = 0; x < 4; x++)
    {
        fader.v[x].col = ARGB(alpha, 0, 0, 0);
    }
}

void hgeFader::FadeOut(float duration)
{
    direction = 1;
    speed = 1.0f / duration;

    //starts the alpha at clear, and is ready to fade out
    alpha = 0;

    //sets the color of the quad
    for (int x = 0; x < 4; x++)
    {
        fader.v[x].col = ARGB(alpha, 0, 0, 0);
    }
}

bool hgeFader::DoneFading()
{
    return (direction == 0);
}
