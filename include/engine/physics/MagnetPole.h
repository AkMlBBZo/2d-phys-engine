#ifndef MAGNET_POLE_ENGINE_H
#define MAGNET_POLE_ENGINE_H

namespace engine::physics {

/**
 * @enum MagnetPole
 * @brief Represents the possible magnetic pole types.
 *
 * This enumeration defines the different types of magnetic poles that can be assigned
 * to objects or particles in a magnetic simulation or system.
 */
enum class MagnetPole {
    NONE = 0,  // No magnetic pole (neutral or non-magnetic)
    NORTH,     // Represents a north magnetic pole
    SOUTH      // Represents a south magnetic pole
};

}  // namespace engine::physics

#endif  // MAGNET_POLE_ENGINE_H