![Crazy Thursday Banner](./title.jpg)

# Team Members

<!-- markdownlint-disable MD033 -->

<table>
    <tbody>
        <tr>
            <th>Name</th>
            <th>UID</th>
            <th>Profile</th>
        </tr>
        <tr>
            <td>Li Yitong</td>
            <td>3036289377</td>
            <td><a href="https://github.com/Lyt060814"><img src="https://avatars.githubusercontent.com/Lyt060814" alt="Li Yitong" width=50></a></td>
        </tr>
        <tr>
            <td>Qu Sijin</td>
            <td>3036289676</td>
            <td><a href="https://github.com/Rosette2048"><img src="https://avatars.githubusercontent.com/Rosette2048" alt="Qu Sijin" width=50></a></td>
        </tr>
        <tr>
            <td>Xue Hengtai</td>
            <td>3036289391</td>
            <td><a href="https://github.com/R1card0xht"><img src="https://avatars.githubusercontent.com/R1card0xht" alt="Xue Hengtai" width=50></a></td>
        </tr>
        <tr>
            <td>Yu Chuhan</td>
            <td>3036449006</td>
            <td><a href="https://github.com/Yu-Chuhan"><img src="https://avatars.githubusercontent.com/Yu-Chuhan" alt="Yu Chuhan" width=50></a></td>
        </tr>
        <tr>
            <td>Zhao Dingcheng</td>
            <td>3036292283</td>
            <td><a href="https://github.com/sh0redry"><img src="https://avatars.githubusercontent.com/sh0redry" alt="Zhao Dingcheng" width=50></a></td>
        </tr>
        <tr>
            <td>Zheng Yilin</td>
            <td>3036289195</td>
            <td><a href="https://github.com/Yilinss27"><img src="https://avatars.githubusercontent.com/Yilinss27" alt="Zheng Yilin" width=50></a></td>
        </tr>
    </tbody>
</table>

<!-- markdownlint-enable MD033-->

## Contributions

- [**Li Yitong**](https://github.com/Lyt060814): Lead developer responsible for core game architecture and system design. Implemented [main game classes](src/Core/) and [terminal settings](src/UI/Terminal.h) (Game, Player, Terminal), developed [necessary utilities](src/Utils/), handled code refactoring and optimization, enhanced game documentation, and wrote [README file](README.md).

- [**Qu Sijin**](https://github.com/Rosette2048): Developed animation effect ([Animation class](src/UI/Animation.h)), designed [start/end frames](anim/), implemented [Explore counter class](src/Counters/Explore.h), and contributed to game interface development.

- [**Xue Hengtai**](https://github.com/R1card0xht): Main designer of the whole game. Implemented [three counter classes](src/Counters/) (Farming, Mining, Recruit) and corresponding [user interfaces](ui/), developed [difficulty selection](src/Core/Difficulty.h), optimized difficulty settings, and recorded demo video.

- [**Yu Chuhan**](https://github.com/Yu-Chuhan): Designed [user interfaces](ui/) and implemented it ([UI class](src/UI/UI.h)), developed [Shop counter classes](src/Counters/Shop.h), Drew beautiful logo, handled copy writing and edited demo video.

- [**Zhao Dingcheng**](https://github.com/sh0redry): Implemented [combat system classes](src/Combat/) (Combat, Weapon, Zombie) and improved them with different difficulties, optimized game frame rate and movement logic, designed [start/end animation of combat](anim/), and wrote [README file](README.md).

- [**Zheng Yilin**](https://github.com/Yilinss27): Developed [Weekcycle System](src/Core/WeekCycle.h) and corresponding [user interfaces](ui/Days/), implemented [combat system classes](src/Combat/) (Combat, Weapon, Zombie), tested the game and adjusted the number settings.

> **Note:** Due to some operations like **Merge** and **Bulk Edits to .txt Files**, the GitHub metrics may not reflect our true efforts. Everyone in our team was actively involved in the project and there is no free rider.


# Description

:wave: **Welcome to a post-apocalyptic world built with C++!** You are the leader of a shelter in a world overrun by zombies. Guide your survivors to gather resources, explore the wasteland, and hold out until rescue arrives.  

The year is 2025. A zombie outbreak has erupted at HKU, and you and your teammates have built a makeshift shelter. The infected—former classmates—still retain strange habits, like becoming inactive on weekends. But **Thursdays** are different: zombies grow more aggressive, posing both danger and opportunity.  

Within your sanctuary's limited operational capacity, you and fellow survivors must strategically select a single task per day. Will you harvest gold coins from the mine to upgrade your defensive arsenal, or cultivate crops to sustain your population? Or you may venture into the chaotic wastelands with your companions – where every expedition balances between claiming glory or suffering disgrace. **Strategic allocation of labor will determine your survival odds!**

When **crimson Thursday** dawns – that cursed cycle triggering the horde's primal rage – zombies unleash their frenzied siege, battering against your defenses with unnatural ferocity. This is your trial by fire: deploy strategic mastery to fortify your stronghold, channeling every upgraded turret and stockpiled resource. **Hold the line until dawn brings salvation... or die trying!**

This world blends **base-building, tactical combat, and roguelike randomness**—no two playthroughs are the same. Forge your own path to survival. **The wasteland awaits!**

# How to play
## Quick Demo

## Compilation

Prerequisite:

- Root directory of this repository.
- A working `g++` compiler that supports `C++17`
- `make`

To compile:

```bash
make
```

To run the game:

```bash
./bin/game
```

Or if you prefer one step:

```bash
make run
```

To clean the build files:

```bash
make clean
```
> If you use `CMake`:
> ```bash
> mkdir build && cd build
> cmake ..
> make
> cd ../bin
> ./Crazy_Thursday
> ```

### Terminal Size

1. **Maximize your terminal window to its fullest extent.**

   On Windows, you can achieve this by pressing the `Alt+Enter` key combination, which will toggle the terminal to fullscreen mode.


2. **Adjust the font size to make the box fit the screen.**
     
    After running the game, you will see a message: `Please adjust your terminal size to make the box fit the screen` shown in a box.
    The box is the user interface of the game, and you should make it almost fill the screen.

## Short Tutorial
### In main menu / home
- Press ***Enter*** to select/continue/enter the counter.
- Press ***W/A/S/D***  to move or change the selection in the home.
- Press ***L*** to show the game status and data.
- Press ***Q*** to quit the game.
### In counters
- Press ***Enter*** to confirm input.
- Press ***H*** to delete all the change and return to home.
- Press ***L*** to show the game status and data.
- Press ***Q*** to quit the game.
- Press ***W*** to show the weapon information.**(Only available in the shop)**
### In combat
- Press ***A/D*** to move left/right.
- Press ***Z/C*** to move faster.
- Press ***Space*** to shoot.
- Press ***P*** to pause the fight.
>**Tips:** DO NOT press ***Enter*** many times, otherwise it may cause unexpected scene skipping.


# Overview
The game follows a component-based architecture with a central Game Engine coordinating various subsystems.
```mermaid
flowchart TD
    subgraph UI_Layer
        UI[UI Module]
        Terminal[Terminal Display]
        Animation[Animation System]
    end

    subgraph Core_Game_Layer
        Game[Game Manager]
        Player[Player System]
        WeekCycle[Week Cycle System]
        Difficulty[Difficulty System]
    end

    subgraph Counter_System
        CounterBase[Counter Base]
        Explore[Exploration]
        Farming[Farming]
        Mining[Mining]
        Recruit[Recruitment]
        Shop[Shop]
    end

    subgraph Combat_System
        Combat[Combat Manager]
        Weapon[Weapon System]
        Zombie[Zombie System]
    end

    subgraph Utility_Layer
        Random[Random Generator]
        Constants[Constants]
        SpecialFunctions[Special Functions]
    end

    %% Data Flow
    UI -->|User Input| Game
    Game -->|State Update| UI
    Game -->|Game State| Player
    Game -->|Time Control| WeekCycle
    Game -->|Difficulty Settings| Difficulty
    
    Player -->|Resource Management| CounterBase
    CounterBase -->|Task Execution| Explore
    CounterBase -->|Task Execution| Farming
    CounterBase -->|Task Execution| Mining
    CounterBase -->|Task Execution| Recruit
    CounterBase -->|Task Execution| Shop
    
    Game -->|Combat Trigger| Combat
    Combat -->|Combat Data| Weapon
    Combat -->|Enemy Data| Zombie
    Combat -->|Combat Results| Player
    
    Random -->|Random Numbers| Game
    Random -->|Random Numbers| Combat
    Constants -->|Configuration Data| Game
    Constants -->|Configuration Data| Combat
    SpecialFunctions -->|Helper Functions| Game
    SpecialFunctions -->|Helper Functions| Combat

    style UI fill:#f9f,stroke:#333,stroke-width:2px
    style Game fill:#bbf,stroke:#333,stroke-width:2px
    style Combat fill:#bfb,stroke:#333,stroke-width:2px
    style CounterBase fill:#fbb,stroke:#333,stroke-width:2px
```

## Core Components

### Object Model Hierarchy
The codebase is organized around several core classes, each responsible for a major aspect of the game's logic and flow. The design emphasizes modularity and clear separation of concerns, with `Game` as the central coordinator, and `Player`, `Combat`, and `CounterBase` (and its derivatives) as key subsystems.

```mermaid
classDiagram
    %% 上方主要子系统
    class Player {
        - int m_people
        - int m_crop
        - int m_gold
        - int m_weaponLevel
        + void assignWorkers(...)
        + void upgradeWeapon()
    }
    class WeekCycle
    class UI
    class Animation
    class Combat {
        - Player& player
        - Weapon weapon
        - ZombieManager zombieManager
        + bool run()
    }

    %% 核心类
    class Game {
        - GameState m_state
        - unique_ptr<Player> m_player
        - WeekCycle m_weekCycle
        - string m_difficulty
        + void run()
        - void initNewGame()
        - void processMainMenu()
        - void processDay()
        - void triggerCombat()
        - void showEndScreen(bool)
    }

    %% 下方Counters相关
    class CounterFactory
    class CounterBase {
        # Player& m_player
        # string m_name
        + CounterBase(Player&, string)
        + virtual ~CounterBase()
        + virtual void OnEnter() = 0
        + virtual void Process() = 0
        + virtual void OnExit()
    }    
    class ExploreCounter
    class FarmingCounter
    class MiningCounter
    class RecruitCounter
    class ShopCounter

    %% 其他子系统
    class Weapon
    class ZombieManager
    class Terminal

    %% 连接关系
    Player --|> Game
    WeekCycle --|> Game
    UI --|> Game
    Animation --|> Game
    Combat --|> Game

    Game o-- CounterFactory

    CounterFactory <|-- ExploreCounter
    CounterFactory <|-- FarmingCounter
    CounterFactory <|-- MiningCounter
    CounterFactory <|-- RecruitCounter
    CounterFactory <|-- ShopCounter

    CounterBase <|-- ExploreCounter
    CounterBase <|-- FarmingCounter
    CounterBase <|-- MiningCounter
    CounterBase <|-- RecruitCounter
    CounterBase <|-- ShopCounter

    Combat o-- Weapon
    Combat o-- ZombieManager

    UI o-- Terminal
```
---
### Game Class
The `Game` class serves as the **central coordinator** for the entire game. Its main responsibilities include:
- **Managing the main game loop** (`run()`), which drives the progression of days, events, and combat.
- **Maintaining game state** (e.g., current day, difficulty, and references to all major subsystems).
- **Coordinating interactions** between the player, counters (resource systems), and combat.
- **Handling state transitions** (e.g., from main menu to gameplay, or from gameplay to combat).
- **Initializing and cleaning up** game resources.

The `Game` class holds references to:
- A `Player` object (managing resources and upgrades)
- A `WeekCycle` object (tracking the passage of time)
- A `CounterFactory` (for creating and managing resource counters)
- A `Combat` object (for handling zombie sieges)
- UI and Animation subsystems

---

### Player Class
The `Player` class encapsulates all **player-related data and operations**, including:
- **Resource management**: tracks population, food (crop), gold, and weapon level.
- **Worker assignment**: manages how many people are assigned to each resource counter (farming, mining, recruiting, shopping, exploring).
- **Upgrades**: handles weapon upgrades and their costs.
- **Daily consumption**: processes food consumption and population changes.
- **Difficulty**: stores and provides access to the current difficulty setting.

The `Player` class provides methods for:
- Adding/subtracting resources (`addCrop`, `addGold`, `addPeople`)
- Assigning and resetting workers
- Upgrading weapons
- Querying current resource and worker status

---

### Combat Class
The `Combat` class manages the **combat system** during zombie sieges. Its responsibilities include:
- **Player movement and shooting**: processes user input for real-time combat.
- **Zombie management**: spawns and moves zombies, checks for collisions.
- **Weapon usage**: integrates with the player's weapon upgrades.
- **Game state management**: tracks health, time, and win/loss conditions.
- **Pause and resume**: supports pausing the combat sequence.

The `Combat` class interacts closely with the `Player` (for health, weapon, and upgrades) and the `WeekCycle` (for difficulty scaling).

---

### CounterBase and Resource Counters
The `CounterBase` class is an **abstract base class** for all resource management systems (counters), such as farming, mining, recruiting, shopping, and exploring. Its key features:
- **Polymorphic interface**: defines virtual methods (`OnEnter`, `Process`, `OnExit`) for entering, processing, and exiting a counter.
- **Player reference**: each counter operates on the player's resources and assignments.
- **Key callback management**: provides utility functions for handling user input (e.g., returning to home, special actions).
- **Extensibility**: derived classes (e.g., `FarmingCounter`, `MiningCounter`) implement specific resource logic.

The `CounterFactory` is responsible for creating the appropriate counter objects as needed.

## Data Flow
The main game loop follows this sequence:
```mermaid
sequenceDiagram
    participant M as main.cpp
    participant G as Game
    participant P as Player
    participant C as Counters
    participant B as Combat
    participant U as UI
    
    M ->> G: Start Game
    G ->> U: Play Opening Animation
    U -->> G: Animation Complete
    
    loop Daily Cycle
        G ->> U: Show Home Screen
        U ->> G: Pass Player Choice
        alt Choose Counter
            G ->> C: Enter Counter
            C ->> P: Update Resources/Personnel
            C -->> G: Return Result
        else Thursday Combat
            G ->> B: Trigger Combat
            B ->> P: Update Combat Results
            B -->> G: Return Victory/Defeat
        end
        G ->> U: Update Status Display
    end
    
    alt Game Victory
        G ->> U: Play Win Animation
    else Game Defeat
        G ->> U: Play Lose Animation
    end
    U -->> M: Exit Game
```


## File Structure
The codebase is organized into multiple files and directories, each handling specific aspects of the game:

| System | File | Description |
|--------|------|-------------|
| **Core Components** | [Game.cpp/h](src/Core/Game.h) | Manages the main game loop, state transitions, and overall game flow |
| | [Player.cpp/h](src/Core/Player.h) | Handles player data, survivor management, and resource tracking |
| | [WeekCycle.cpp/h](src/Core/WeekCycle.h) | Controls the game's time system and zombie behavior patterns |
| | [Difficulty.h](src/Core/Difficulty.h) | Defines game difficulty levels and their associated parameters |
| **Counter System** | [CounterBase.cpp/h](src/Counters/CounterBase.h) | Base class for all game counters and activities |
| | [CounterFactory.h](src/Counters/CounterFactory.h) | Implements the factory pattern for creating different types of counters |
| | [Explore.cpp/h](src/Counters/Explore.h) | Manages exploration mechanics and resource discovery |
| | [Farming.cpp/h](src/Counters/Farming.h) | Handles crop growth and food production |
| | [Mining.cpp/h](src/Counters/Mining.h) | Controls resource gathering and mining operations |
| | [Recruit.cpp/h](src/Counters/Recruit.h) | Manages survivor recruitment and population |
| | [Shop.cpp/h](src/Counters/Shop.h) | Handles trading and upgrades |
| **Combat System** | [Combat.cpp/h](src/Combat/Combat.h) | Core combat mechanics and battle resolution |
| | [Weapon.cpp/h](src/Combat/Weapon.h) | Weapon properties and combat calculations |
| | [Zombie.cpp/h](src/Combat/Zombie.h) | Zombie behavior, stats, and special abilities |
| **User Interface** | [UI.cpp/h](src/UI/UI.h) | Main interface system and screen management |
| | [Terminal.cpp/h](src/UI/Terminal.h) | Terminal display and text rendering |
| | [Animation.cpp/h](src/UI/Animation.h) | Game animations and visual effects |
| **Utilities** | [Constants.h](src/Utils/Constants.h) | Game constants and configuration values |
| | [Random.h](src/Utils/Random.h) | Random number generation utilities |
| | [SpecialFunctions.cpp/h](src/Utils/SpecialFunctions.h) | Helper functions used throughout the game |

