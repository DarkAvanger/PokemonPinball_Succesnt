Pokemon Pinball Game made in SDL2 and Box2D


# **Controls**

### Game

Left Arrow -> Left Flipper

Right Arrow -> Right Flipper

Down Arrow -> Kicker

### Debugging Keys

F1 -> Show hitboxes and allows mouse joint

F2 -> Swap between Floor 1 and Floor 2 hitboxes

F4 -> Move the ball to mouse position

P -> Add 5.000 points 

L -> Lose one life

O -> Add one life

### Extras

Get extra points for hitting pokemons.

Each pokemon has it's own sounds.

An extra ball every time you reach 10.000 points.

Try to reach 20.000 points. 

## Development 

This game has been designed with box2D and SDL.

### Bugs 

Hitbox position and size caused a lot of game bugs, fixed by redoing the hitboxis to a proper size.

Ball hitbox is not in the same position as the ball, it still interacts well with other game elements besides the 3 bumpers. (No fix has been found)

If ball exceed max speed while interacting with map sensors they will not react in time and the ball would go out of the map, slower ball velocity fixed this.

Map sensor would not work after reseting the ball, to fix this we created another sensor at the begging of the kicker.

### Links

Original game: [Pokemon Pinball GBA](https://pokemon.fandom.com/es/wiki/Pok%C3%A9mon_Pinball:_Rub%C3%AD_y_Zafiro)

Github: [Succesn't](https://github.com/DarkAvanger/PokemonPinball)

### Game differences

The original game has a lot of complex mechanics that were not implemented:

No catch pokemon/bonus map

Only the main loop was implemented

Missing some animations that are not relevant to gameplay

### Authors

Angel Consola

Juand de Dios Garcia Salguero

Luis Fernandez Diaz
