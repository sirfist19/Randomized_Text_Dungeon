README - Overview and Main Features, How to Run the game, and How to Play

=================================================================================================
OVERVIEW:

Randomized Text Dungeon is a text-based dungeon crawler game inspired by classic text-based games such a Planetfall and Zork as well as more modern games such as The Legend of Zelda, and Metroid (specifically Zelda dungeons, and Metroid based exploration). 

The game is written entirely in C++ and coded by Aidan Cini. 

Main Features:
· Randomly generates an explorable text-based dungeon. The algorithm creates the start and exit rooms, then randomly picks an exit in a cardinal direction and then through a dice roll picks whether or not it want to place an exit and a new room in that direction. If a new room is added then it later randomly picks exits to create new rooms. 
·These new rooms are then filled randomly with enemies, chests, items, and riddles by placing better items and 
harder enemies far away from the starting area. 
· A boss room is added in the deepest room of the dungeon. To win the player must defeat this cruel dragon.
· Randomly generates an explorable text-based dungeon using BFS and other algorithms whose room 
layout is different every time you play.
· Weapons feature a heavy attack and a light attack which differ in the amount of damage they do and the percent chance for them to miss.
	-Heavy attacks miss more often but hit for more damage
	-Light attacks miss less but hit for less damage
· Armor helps protect against damage in battles again powerful foes.
· Chests feature random loot tiered to what type of chest they are
	-Wooden chests are the worst, then Gold chests, and Dragon Chests are the best
· A map feature allows you not to explore the dungeon blind. It updates as you explore more and more of the dungeon. 
	-The map is implemented by having each room hold a world coordinate relative to the start room. This world coordinate is then converted to a text-document coordinate that can be used 	 to place a character on the map.
· Uses an Object-oriented text parsing system to process player input using a verb-object syntax. 
· Utilizes OOP in a well-designed and thought-out fashion by using inheritance, polymorphism, and a 
hierarchy of classes along with the proper destructors needed to reclaim heap memory taken

=================================================================================================
HOW TO RUN THE GAME:
Because the game is written in C++, in order to play the game you will need a C++ compiler. 

Using gcc:
Downloading the .h and .cpp files and using a compiler such as gcc will allow you to play the game. Simply run the main.cpp file and be sure to copy all the .h and .cpp files.

Using Visual Studio:
To code the game, I used Visual Studio 2019. Clone the repository, extract the zip file, and then simply double click on the solution file called Randomized_Text_Dungeon.sln. This will open the game in Visual Studio where you can start the game by going to Debug then Start without Debugging.

=================================================================================================

HOW TO PLAY/ TUTORIAL:
Like old text based games like Zork, Randomized Text Dungeon uses a text parsing system to understand what the player types. The general syntax is command then object. For example, 
in the command

	take sword

take is the command and sword is the object. If there is a sword present in the room, the player will pick it up and it will be added to their inventory.

An object is not always needed, but a command is always required. Commands such as 'look', 'help', 'map', and 'quit' do not need an object. The text-parser can also understand more complex sentences (to a certain extent) such as ones that include a preposition. Examples are 'go into waterfall' or 'jump into pit'. 


BASIC COMMANDS:

go
	Needs Object: Yes
	Allows the player to move through the dungeon. Need to type a direction (Ex: north (n), south (s), east (e) or west (w)) as the object. Can also just type the direction.
		Ex: go north
		Ex: go n
		Ex: n
		Ex: w

quit
	Needs Object: No");
	Quits the game to the title screen. Restarts all progress.

look
	Needs Object: No
	Displays the information about the room usually printed at the top of the screen.
	
map
	Needs Object: No
	Displays a dungeon map of the areas you have already visited.
	The @ represents the current position of the player. S is the start room, E is the exit to the dungeon, and B is the boss room.
	
	
examine
	Needs Object: Yes
	Takes all as an object: No
	Gives more information on an object in the room, in a chest, or in your inventory. Can reveal hidden items and riddles.
		Ex: examine chest
		Ex: examine axe
		Ex: examine dragon doors
	
	
inventory
	Needs Object: No
	Can type 'i' as a shortcut. Shows your health, current weapon, and items.
		Other relevant commands for using your inventory are:
		 -equip
		 -drink
		 -use
		 -examine
	
open
	Needs Object: Yes
	Opens chests allowing the items inside the chests to be displayed and for the player to take the items inside. When a chest is already open, this will redisplay the chest's 			current items.
		Ex: open chest
	
	
take/pick up
	Needs Object: Yes
	Takes all as an object: Yes
	Takes items from already open chests or from the floor of the current room and places the items into the player's inventory. Using 'take all' picks up all items in the room and in 	already open chests in one command. To take one item at a time use the item you want to pick up's name as the object of this command.
		Ex: take all
		Ex: take sword
	
	
drop
	Needs Object: Yes
	Takes all as an object: Yes");
	Drops items from the player's inventory onto the current room's floor. Using 'drop all' drops all items in the player's inventory. To drop one item at a time use the item you want to 	pick drop's name as the object of this command. If there is more than one object of that type in the player's inventory, you will be prompted to type the amount you want to drop.");
		Ex: drop all
		Ex: drop lesser healing potion
	
	
equip
	Needs Object: Yes
	Takes all as an object: Yes
	Moves a weapon or armor from the player's inventory to the player's main weapon or main armor. The previous player's main weapon or armor is placed into the player's inventory. Using 	'equip all' equips all of the weapons and armor in your inventory that do more damage or have more defense than what the player currently has. In essence, your equipped gear is 	maximized for damage and defense.");
		Ex: equip all
		Ex: equip bow
	
drink
	Needs Object: Yes
	Takes all as an object: Yes
	Consumes a potion in the player's inventory and gives the player the effect. If the command 'drink all' is used, then the smallest healing potions will be drunk first healing the 	player to full health (if you have enough potions).
		Ex: drink greater healing potion
		Ex: drink all
	
jump
	Needs Object: No (but an object may be used)
	The player jumps into the air. If an object is used then the player jumps into or on that object.
	
list
	Needs Object: No
	Prints out a list of all valid verbs and objects. Item names are also valid objects.
	
clear
	Needs Object: No
	Clears the screen.
	
use
	Needs Object: Yes
	Takes all as an object: No
	Uses keys. If a potion is the object, then the player drink the potion (see drink command).
		Ex: use key
		Ex: use common healing potion
	
help
	Needs Object: No (but may be used)
	Takes all as an object: No (but may take one)");
	You should already know what this one does because you are using it! Teaches the player how to play the game by showing them new commands. To find out more information about a 	specific command, type 'help' followed by the command you want to learn more about.");
		Ex: help
		Ex: help go

===================================================================================================
