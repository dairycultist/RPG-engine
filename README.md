# RPG-engine

A data-driven game engine for creating simple RPGs.

there's no "world map" to walk around in, so this is more of a visual novel engine but with battles/a party system

The engine is also the game launcher (like pico-8), where when you open a gamefile to play, you can switch to edit mode through the pause menu (makes for quick testing/iteration).

basically just a state-machine. states can be:
- battles (has two successor-states, for when you win/lose the battle)
- scene dialogue (i.e. not a character speaking), character dialogue, partymember joined dialogue, partymember left dialogue (has one successor-state, for when the button is pressed to advance the dialogue)
- choice dialogue (has multiple successor-states, depending on what choice is selected)
- shops (has one successor-state, for when you exit the shop)
  - shops let you buy stuff and manage your party

all the game data is stored in one file, which is saved to by the engine

every asset is made in-engine (no importing, no filesystem). no tilemaps, mainly just sprites for 1) scene backgrounds 2) character in-scene sprites 3) dialogue portraits

![](https://static.wikia.nocookie.net/undertale/images/7/7d/Tem_Shop_soundtrack.png/revision/latest?cb=20151105132749)

https://www.deviantart.com/shweetmagnet/art/2025-06-12-Natalie-5-1224856754

steal SDL code from https://github.com/dairycultist/Berry2D/blob/main/src/window.c
