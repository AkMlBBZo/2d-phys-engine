#ifndef INTERACTIONS_ENGINE_H
#define INTERACTIONS_ENGINE_H

#include "MagnetPole.h"
#include "MaterialType.h"

namespace engine::physics::Interactions {

bool haveMagneticField(MagnetPole pole);

bool magneticInteraction(MagnetPole pole, MaterialType material);

float getMaterialMultiplier(MaterialType material);

bool isMetall(MaterialType material);

}  // namespace engine::physics::Interactions

#endif  // INTERACTIONS_ENGINE_H