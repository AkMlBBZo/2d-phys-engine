#include "engine/physics/Interactions.h"

namespace engine::physics::Interactions {

bool haveMagneticField(MagnetPole pole) {
    return pole != MagnetPole::NONE;
}

bool magneticInteraction(MagnetPole pole, MaterialType material) {
    if (haveMagneticField(pole))
        return true;

    bool magneticMaterial = false;

    switch (material) {
    case MaterialType::IRON:
        // any cases
        magneticMaterial = true;
        break;
    default: break;
    }

    return magneticMaterial;
}

float getMaterialMultiplier(MaterialType material) {
    switch (material) {
    case MaterialType::IRON: return 3.0f;
    default: return 1.0f;
    }
}

bool isMetall(MaterialType material) {
    switch (material) {
    case MaterialType::IRON: return true;
    default: return false;
    }
}

}  // namespace engine::physics::Interactions
