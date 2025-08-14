#include "Component.h"

UI_Component::UI_Component(unsigned char id_, Rectangle anchor_) : c_id(id_), anchor(anchor_)
{
}

bool UI_Component::Inside(Vector2 pWorld)
{
    return CheckCollisionPointRec(pWorld, anchor);
}
