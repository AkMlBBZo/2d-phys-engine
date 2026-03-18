# 2D Physics Engine  

A lightweight 2D physics engine implementing particle systems, constraints, and collision detection.  

## Current Features
- **Particle System**
  - Mass, velocity, and force integration
  - Verlet integration with damping
- **Spring System**
  - Elastic constraints with rest length
  - Velocity-based damping
- **Performance**
  - SIMD (AVX2) optimizations for batch processing
  - Structure of Arrays (SoA) data layout
  - Aligned memory allocator (32-byte alignment)
- **Rendering**
  - SFML-based visualization
  - Points and springs debug rendering

## Dependencies  
- **CMake** (>= 3.16)  
- **SFML** (>= 3.0) (for rendering and window management)  
- **C++17** compatible compiler  

## Planned Features  

### Architecture
- [ ] Evaluate Entity-Component-System (ECS) for better scalability  
- [ ] Perfect Forwarding support for efficient resource management 

### Physics & Constraints  
- [ ] **Constraint Types**  
  - [ ] `Line` (infinite constraint)  
  - [x] `Spring` (elastic connection)  
  - [ ] `Rod` / `Stick` (fixed-length constraint)  
- [ ] **Collision Detection**  
  - [ ] Point vs. Line  
  - [ ] Basic shape primitives (using `Point` and `Line` internally)  
- [ ] **Damping**  
  - [ ] Air resistance & friction simulation  

### Performance Optimizations  
- [x] SoA (Struct of Arrays) data layout
- [x] SIMD optimizations (AVX2)
- [x] Aligned memory allocator
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