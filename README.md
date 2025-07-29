# Capture The Flag

This project contains the classic **Capture The Flag** game developed based on the  *First Person* template provided by *Unreal Engine*.

The main level is **FirstPersonMap**, so you can just press the play button with this level open on the editor.

This game can be played on **Dedicated Server** and **Listen Server**, it works both ways.

## Game Flow

![MenuScreen](https://github.com/LukasGL/CaptureTheFlag/blob/main/img/MenuScreen.png)

The first screen will let you choose your team and after clicking one of the boxes you will be automatically spawned on the match.

![Spawned](https://github.com/LukasGL/CaptureTheFlag/blob/main/img/Spawned.png)

The other players will be spawned on the map after they do the same (choose a team).

 ![Weapon](img\Weapon.png)

Next step is to pick up a **Weapon**. Next to every base you will have 1 weapon that can be picked up by just walking over it. You will be able to fire to every other character on the map.

![WhiteFlag](https://github.com/LukasGL/CaptureTheFlag/blob/main/img/WhiteFlag.png)

On the center of the map you will find **The White Flag**, pick up this item by just walking over it and bring it back to you base (where your character was spawned) for scoring a point for your team.

![TeamBase](img\TeamBase.png)

![PlayerScored](https://github.com/LukasGL/CaptureTheFlag/blob/main/img/PlayerScored.png)

If other player picked up the flag before you did it, you can just check above the player if they have the white flag icon, then you will have to shoot them for make them drop the flag and bring it to your base.

![Enemy](img\Enemy.png)

![FlagDropped](https://github.com/LukasGL/CaptureTheFlag/blob/main/img/FlagDropped.png)

If your team makes it to 3 points, your team wins and the game will show the message to every player on the match. The game will restart automatically after 3 seconds.

![TeamWin](img\TeamWin.png)

## Controls

​	**W** - Move forward

​	**A** - Move left

​	**S** - Move backwards

​	**D** - Move right

​	**Space Bar** - Jump

​	**Mouse movement** - Rotate the camera

​	**Mouse click** - Fire (After picking up a weapon)

## Project Comments

The development of the entire project using the *Unreal Engine* template, took about **35 hours** spent on coding and assets management.

It was tested with 4 players simultaneously, but it should with an undetermined number of players. 

Gameplay Ability System was used for Movement, Character Death, Jump and Shoot. Also an I used an attribute set for the health and death notification.

Any comments or complications with running the project please let me know sending me an email to lukas.gribbell@gmail.com.