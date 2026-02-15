# Ascend

A **Celeste / Super Meat Boy**-inspired 2D platformer prototype built in **C++** with **openFrameworks**, created for my final-year university module.  
Ascend focuses on responsive movement and trial and error: tight jumps, dashes, wall interactions, and hazard-driven level flow.

## Gameplay

You play through a handcrafted Level 1 made up of multiple connected “parts”. Reaching the screen edge transitions you to the next part, with hazards and movement challenges along the way.

## Demo
<p align = "center">
  <img src="screenshots/demo.gif" alt="Ascend Gameplay Demo" width=800>
</p>

## Features

- **Level 1 fully playable** with **transitions between level parts**
- **Advanced movement mechanics**
  - Dash (8-directional)
  - Wall slide + wall jump
  - Double jump
  - Air/ground damping for better control
- **Hazards & interaction**
  - Spikes
  - Animated saw traps (static + moving variants)
  - Jump pads that launch the player and reset dash/double-jump
- **Visual polish**
  - Sprite animation sets (idle/walk/jump/fall/dash)
  - Animated environment elements (e.g., flag)
  - Parallax scrolling background layers
  - Rain particle effect
- **Audio**
  - Jump / dash / walk / land / death sounds
  - Looping rain ambience

## Controls

| Action | Key |
|-------|-----|
| Move Left / Right | **A / D** |
| Aim Dash Direction | **W / A / S / D** (combinations supported) |
| Jump | **Space** |
| Dash | **Shift** |
| Quit after finish | **Enter** (on completion screen) |

> Dash direction is based on current input (supports diagonals).

## Build & Run (openFrameworks)

### Requirements
- **openFrameworks** (tested with a standard OF project structure)
- A C++ compiler supported by your OF version (Visual Studio on Windows is typical)

### Setup
1. Download openFrameworks and generate a new project (Project Generator).
2. Copy this project’s `src/` and `bin/data/` into your generated OF project folder.
3. Open the generated solution/project and build.

### Run
- Launch from your IDE or run the compiled executable.
- Default window size is **1920x1080** (see `main.cpp`).

## Project Structure (high level)

- `src/`
  - `ofApp.*` — main game loop (setup/update/draw) and input handling
  - `player.*` — movement, collision, animation, audio
  - `map.*` — level loading, tiles, hazards, transitions, rendering, rain/parallax
  - `trap.*` — moving trap logic (sin-based movement + phase offsets)
- `bin/data/`
  - `images/` — player animations, tiles, background layers, traps, jump pad, etc.
  - `sounds/` — movement/impact sounds + rain ambience

## Level / Tiles (how it works)

The map loads from character grids and spawns objects based on tile symbols:

- `#`, `~`, `|` — solid tiles (different visuals)
- `P` — player start when entering a part from the left (next level part)
- `p` — player start when entering a part from the right (previous part)
- `s` — spike trap
- `t`, `T` — saw traps (animated; some move on X/Y)
- `j` — jump pad
- `f` — visual element (flag)

Crossing the **left or right screen border** transitions between level parts.

## What I’d Improve Next

- More levels + checkpoint system
- Coyote time / jump buffering (even tighter platforming feel)
- Better camera framing (look-ahead + smoothing) for larger maps
- UI polish (timer, deaths, restart key)
- Cleaner data-driven level format (JSON/TMX)

## Credits / Inspiration

- Inspired by **Celeste** and **Super Meat Boy**
- Built with **openFrameworks**

## License

This project is licensed under the **MIT License**.

> **Academic note:** This repository is published as part of a university module submission and as a portfolio piece.  
> You are free to fork and learn from it under the MIT License, but **submitting this work (or close derivatives) as your own academic work is academic misconduct**.  
> If you are a student, do not copy this code for coursework—use it for learning only.