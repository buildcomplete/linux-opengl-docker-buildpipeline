#include "IG_types.h"


std::ostream& operator<<(std::ostream& os, const CellPosition& pos) {
    os << "CP: (" << pos.x << ", " << pos.y << ")";
    return os;
};
