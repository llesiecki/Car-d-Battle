# Car(d) Battle
A card game focused on car parameters.
![Car(d)Battle](https://user-images.githubusercontent.com/37122127/119583742-873d1180-bdc7-11eb-8389-6f67f44bfbe9.png)

![Car(d)Battle](https://user-images.githubusercontent.com/37122127/120085423-43545000-c0d8-11eb-9db7-61e4e7824eb5.gif)

This game is developed with pure C++ and OpenGL, no game engines, no frameworks.
# Description
It's a classic card battle game. There are 24 cards evenly distributed to all players. Each card represents a different car and has 6 fields: Cylinders, Capacity, Power, Torque, Top Speed and Acceleration.  
The player whose turn it is, picks the category and then all the players are compared by this category. The winner of the round, takes all the cards from other players.  
In order to win, a player has to collect all the 24 cards.
# Controls
The category is picked by clicking on it with LMB.
**Other controls related to multiplayer mode are in progress**
# Mechanics
The gameplay is actually a state machine, there are 7 states:
* *Cards Distribution:*
  * The cards from the central stack are evenly distributed to all the players stacks. The current player is set to the host player.
* *Cards To Players:*
  * For each player, a one card from the top of player's stack is transferred to player's hand.
* *Choose category:*
  * The current player has 10 seconds to pick the category, if the time runs out, the category is randomized.
* *Show Players Cards:*
  * All the opponents cards are flipped on top, so the fields can be seen.
* *Compare By Chosen Category:*
  * All the cards are compared by the chosen category, and the round winner is chosen. If two or more cards have the same value in the chosen category, the tiebreak algorithm is engaged.
* *Tie Break:*
  * Two cards are transferred from players who are taking part in the tie-break to their hands. If a player stack runs out of the cards, he loses, except the case when all the players run out their cards, then cards are transferred from their hands back to their stacks and shuffled.  
  If all the cards have the same value in the chosen category, the category is randomized.
  After that the cards on players' hands are compared by chosen category and the winner is chosen, or if there is still a draw, the tie-break algorithm is called again.
* *Cards To Winner:*
  * All the cards from all players' hands are transferred to the winner's stack. If his stack size is 24, then he becomes the final winner, if not, he becomes the current player and the game state is set to **Cards To Players**

The cards data is stored in an M$ Excel sheet. The cards textures are stored in the **textures** directory and the cars photos are in the **textures/cars** directory.
The game also uses a native C++ code for **.PNG** pictures decompression to bitmaps.  
> **_NOTE:_** The multiplayer mode and UI allowing the player to choose a nickname and battle is stll in progress.
# Software
* Language: [C++](https://www.cplusplus.com/) 17
* Graphics API: [OpenGL](https://www.opengl.org/)
* OpenGL Implementation: [GLFW](https://www.glfw.org/)
* OpenGL Loader-Generator: [Glad](https://glad.dav1d.de/)
* Platform: [Windows](https://www.microsoft.com/en-us/windows) 7 SP1 and newer
* Library for M$ Excel sheets reading: [LibXL](https://www.libxl.com/)
* .PNG decoding: [LodePNG](https://github.com/lvandeve/lodepng)
