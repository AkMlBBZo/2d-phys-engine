# 2d physics engine

**Currently implemented:**  
•   Only points (basic particle system)  

**Dependencies:**

•   CMake (>= 3.16)
•   SFML (>= 3.0)
•   C++ compiler with C++17 support

## Future Plans

This section outlines the planned features and improvements for future releases.

**Architecture & Data Layout:**

•   [ ] Switch from AoS to SoA for SIMD optimizations.
•   [ ] Explore alternative data-oriented design patterns (e.g., ECS) for improved performance and scalability.

**Physics:**

•   [ ] Implement constraint classes: `Line`, `Spring`, and `Rod/Stick` for fixed-length connections.
•   [ ] Implement Shape and legacy classes for more complex object representation.  These classes will internally utilize `Point` and `Line` primitives.
•   [ ] Implement point/line collision detection.
•   [ ] Implement damping to simulate friction and air resistance.

**Performance:**

•   [ ] Improve performance of interactions using space partitioning.
•   [ ] Profile and optimize critical sections of the code using profiling tools.
•   [ ] Implement multithreading for parallelizing physics calculations.

**Logging & Debugging**  
•   [ ] Add logging system for debugging physics interactions.  
•   [ ] Log collision events, constraint violations, and performance metrics.  
•   [ ] Add runtime debug visualization (e.g., force vectors, collision normals).  

**Long-Term Goals:**

•   [ ] Explore potential GPU acceleration for physics calculations.
•   [ ] Add serialization/deserialization for saving and loading physics scenes. 