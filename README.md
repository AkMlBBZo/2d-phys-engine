# 2D Physics Engine  

A lightweight 2D physics engine implementing particle systems, constraints, and collision detection.  

## Current Features  
- **Particle System**  
  - Basic point-based physics  
  - Mass, velocity, and force integration  

## Dependencies  
- **CMake** (>= 3.16)  
- **SFML** (>= 3.0) (for rendering and window management)  
- **C++17** compatible compiler  

## Planned Features  

### Architecture & Data Layout  
- [ ] Switch from AoS (Array of Structs) to SoA (Struct of Arrays) for SIMD optimizations  
- [ ] Evaluate Entity-Component-System (ECS) for better scalability  
- [ ] Perfect Forwarding support for efficient resource management 

### Physics & Constraints  
- [ ] **Constraint Types**  
  - `Line` (infinite constraint)  
  - `Spring` (elastic connection)  
  - `Rod` / `Stick` (fixed-length constraint)  
- [ ] **Collision Detection**  
  - Point vs. Line  
  - Basic shape primitives (using `Point` and `Line` internally)  
- [ ] **Damping**  
  - Air resistance & friction simulation  

### Performance Optimizations  
- [ ] Spatial partitioning (e.g., grids, quad/octrees)  
- [ ] Multithreading support for parallel physics updates  
- [ ] Profiling-guided optimizations  

### Debugging & Diagnostics  
- [ ] **Logging System**  
  - Collision events  
  - Constraint violations  
  - Performance metrics  
- [ ] **Debug Visualization**  
  - Force vectors  
  - Contact normals  
  - Broad-phase boundaries  

### Long-Term Goals  
- [ ] GPU acceleration (via compute shaders or CUDA/OpenCL)  
- [ ] Scene serialization (JSON/binary formats)  

---  

**Note:** Contributions are welcome! Open an issue or PR for feature requests/bug reports.  