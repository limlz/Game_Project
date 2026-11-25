# WEWASHPl8

A dishwashing arcade game written in C on top of CProcessing.

Scrub dirt off plates with the sponge, rinse them at the faucet, and keep pace as the daily quota climbs. Earnings feed a shop where upgrades buy you a chance at surviving the later days.

## Gameplay

- **Day loop** — each day sets a plate quota against a timer; clear it to advance
- **Economy** — washing earns money, money buys upgrades
- **Progression** — shop, upgrades, leaderboard, tutorial, settings and credits screens

## Layout

```
src/
  core/        game states: splash, main menu, game, settings, leaderboard, credits, game over
  entities/    plate, sponge, dirt, faucet, bubblegun, roomba
  systems/     day, money, shop, upgrades, soap, timer, tutorial, sounds, settings
  utils/       debug overlay, image and font loading
  vfx/         bubbles, confetti, discoball
Extern/
  CProcessing/ vendored framework
```

Screens are driven through CProcessing's `CP_Engine_SetNextGameState`, each exposing Init / Update / Exit.

## My contribution

Largest contributor on the team by commit count (103 of 239). Files I authored:

- `src/vfx/bubbles.c` — the bubble particle system: spawning, per-frame update and draw, driven from cursor position with a separate manual-spawn path for scripted moments
- `src/entities/bubblegun.c`, `src/entities/dirt.c`, `src/entities/roomba.c`
- `src/systems/money.c`, `src/systems/sounds.c`, `src/systems/settings_options.c`
- `src/core/gameover.c`, `src/core/settings.c`
- `src/utils/debug.c` — debug overlay; `src/utils/img_font_init.c` — image and font loading

Co-authored `mainmenu.c`, `plate.c`, `sponge.c`, `shop.c` and `tutorial.c`.

## Build

Open `Game_Project.sln` in Visual Studio and build. CProcessing is vendored under `Extern/`, so there is nothing else to fetch.

## Team

Built by a five-person team: limlz, junwxn, n4ddeer, mightbedan, choonzs.

---

Coursework project at DigiPen Institute of Technology Singapore. Source files carry DigiPen copyright headers.
