#ifndef MATERIAL_TYPE_ENGINE_H
#define MATERIAL_TYPE_ENGINE_H

namespace engine::physics {

/**
 * @enum MaterialType
 * @brief Represents different types of materials.
 *
 * This enumeration defines material types that can be used for physical properties,
 * rendering, or other material-specific behaviors in a simulation or application.
 */
enum class MaterialType {
    DEFAULT = 0,  // Default material type (basic properties)
    IRON          // IRON material type (specific conductive/magnetic properties)
};

}  // namespace engine::physics

#endif  // MATERIAL_TYPE_ENGINE_H