# RPG-engine

A data-driven game engine for creating simple RPGs.

## TO-DO
1. make a human-readable text format for dialogue and choices (core visual-novel components)
  - game data file has a "start state" entry so that the editor can test starting from arbitrary states
2. implement game.c (reads the aforementioned text file and plays the game) (editor will come later)

## Limitations
- there's no "world map" to walk around in, so this is more of a visual novel engine but with battles/a party system
- no party system (enemies during combat "look out of the screen" like earthbound, since there's no point in a "face-to-face" system like pokemon if it's just you)

## Notes
editor and the launcher are two programs (there is confirmed a launcher since idk how I would make a cross-compatible executable)

The game is basically just traversing a state-machine. A state can be a:
- dialogue (can have a character portrait + name or not) (has one successor-state, for when the button is pressed to advance the dialogue)
- choice (has multiple successor-states, depending on what choice is selected)
- battle (has two successor-states, for when you win/lose the battle)
- shop (has one successor-state, for when you exit the shop)
  - shops let you buy stuff and manage your party

all the game data is stored in one file, which is saved to by the editor

every asset is made in-engine (no importing, no filesystem). no tilemaps, mainly just sprites for 1) scene backgrounds 2) character in-scene sprites 3) dialogue portraits

![](https://static.wikia.nocookie.net/undertale/images/7/7d/Tem_Shop_soundtrack.png/revision/latest?cb=20151105132749)

https://www.deviantart.com/shweetmagnet/art/2025-06-12-Natalie-5-1224856754

steal SDL code from https://github.com/dairycultist/Berry2D/blob/main/src/window.c

## Fonts
https://www.dafont.com/vcr-osd-mono.font

https://opendyslexic.org/