# EterNights

**EterNights** is an open-world Souls-like adventure game developed in **Unreal Engine 5** using **C++**.

## Overview

Embark on a perilous journey through a vast, interconnected open world. As a Souls-like adventure, the game emphasizes challenging combat, exploration, and environmental storytelling.

## Features

- **Open World Exploration**: Seamlessly travel across diverse and dangerous landscapes.
- **Souls-like Combat**: Tactical gameplay requiring precision, timing, and resource management (Stamina/Health).
- **RPG Elements**: Character progression, equipment systems, and stat management.

## Codebase

The core logic is implemented in C++ for performance and extensibility, exposing key variables and functions to Blueprints for design iteration.

### Key Classes
- **BaseCharacter**: The parent class for all character entities, handling common attributes (health, basic movement).
- **EchoCharacter**: A specialized character class (Player or Hero) containing specific abilities and input handling.
- **EchoAnimation**: Handles animation state machines and blending for the character.

## Tech Stack

- **Engine**: Unreal Engine 5
- **Language**: C++
- **Architecture**: Actor-Component based design.

## Script Reference

### C++ Classes (`Scripts/EterNights/`)

#### Characters (`Public/Characters/`)
- **`BaseCharacter`**: The foundational class for all entities (Player/AI). Handles attributes like Health, Stamina, and common movement functions.
- **`EchoCharacter`**: The primary player character. Extends `BaseCharacter` to include specific input handling (Attack, Dodge, Interact) and camera control logic.
- **`EchoAnimation`**: The Animation Instance class handling the communication between the Character logic and the Animation Blueprint (State Machine variables).
- **`CharacterState`**: Header defining enums and structs for character states (e.g., `ECharacterState::Attacking`, `ECharacterState::Idle`).

## Installation

1. Clone the repository.
2. Ensure you have **Visual Studio 2019/2022** installed with "Game Development with C++" workload.
3. Right-click `Scripts/EterNights.uproject` (or the relevant uproject file) and select **Generate Visual Studio project files**.
4. Open the solution (`.sln`) and build the project.
5. Launch the Unreal Editor.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 ARGUS
