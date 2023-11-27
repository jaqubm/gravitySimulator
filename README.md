# Gravity Simulator

Gravity Simulator for Particles and GravitySources with collision system made using SFML library in C++. It provides 5 scenes - testing accuracy of collision implementation, test scene with totally random particles movement and three custom scenes.

## Tech Stack

- C++17
- SFML
- CMake

## Build and Run

Project is built using CMake with version at least 3.5

### In file `config.h` you can modify:

- `W` - Window Width
- `H` - Window Height
- `FPS` - Frame Rate Limit
- `MAX_VEL` - Maximum velocity for random velocity generator
- `MIN_VEL` - Minimum velocity for random velocity generator
- `MAX_POS` - Maximum position for random position generator
- `MIN_POS` - Minimum position for random position generator
- `PARTICLES_NUM` - Number of Particles in simulation

## Preview

### Choosing Scene

![Choosing Scene Preview](/preview/preview_1.png)

### Simulation

![Simulation Preview](/preview/preview_2.png)
