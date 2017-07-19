
#include "hge.h"

float CALL HGE::Timer_GetTime()
{
    return fTime;
}

float CALL HGE::Timer_GetDelta()
{
    return fDeltaTime;
}

int CALL HGE::Timer_GetFPS()
{
    return nFPS;
}

