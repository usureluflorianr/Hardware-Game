# Seek Memories Game

## <a href="https://github.com/usureluflorianr/Robotics" target="_blank">Repo related</a>

## <a href="https://www.youtube.com/watch?v=K9cxmqRYDvA" target="_blank">Watch demo</a>

# Game description

You have a room of 23x23 (the initial matrix is 24x24, but we have borders). You start from (1, 1) and you have to collect memories (fast blinking objects). There are bad people who want to stop you from having a great life and if they touch you, they kill your enthusiasm (you have 2 units of enthusiasm). The score is a sqare root of a coeficient based on how many memories did you collected and how much time you were "alive", moving in this world. If you stay in the same point too much (3 seconds in our game), you will start to lose score points. In addition, you can use a superpower named "motivational video weekend". When you use your superpower, the killers will freeze and you can escape or go to your desired memory. Last but not least, you get old and there will be more and more barriers in your life (health, job, etc...). There will be dead cells (walls) spawned which will block you. The score is "how awesome" was your life. 

# User experience
* you are a dot; if you don't move for at least 3 seconds, you will start blinking and lose points 
* when you are using your power, the led will go crazy and light in blue and green 
* the led will show you the distance to the existing memory on the map (the intensity is how near you are)
* when you die, you will spawn to (1, 1) cell, by default in superpower mode 
* 1st memory is always near you 
* you have 2 game modes: rooms (where the map is 3x3, each 1x1 being an 8x8 leds configuration) or following (where the camera moves with you)
* the killers have an increasing speed and they will spawn near you as the time passes

# Menu

* Promo - it will dissapear after automatically (or by pressing the button); you have some dancing dots on the 8x8 leds; you have also an image for each menu stage 
* Menu - you can go to: Play, About, Scores or Settings 
* About - information about me
* Settings - you can control the sound, contrast and brightness for 8x8 leds or lcd
* Scores - you can se top 3 highscores and clear them all
* Play - you have to choose your name (exactly 3 characters), and then you can go and choose the game mode
* Game mode - you can choose between Rooms mode or Following mode and then you will start the game
* The game: you will se how many powers and lifes do you have, the score and a nice message; when the game is over, you will se the score and see if is highscore `

# Components
* Arduino UNO
* 16x2 LCD
* joystick
* active buzzer
* 8x8 led matrix
* breadboards
* wires
* electrolytic capacitor
* ceramic capacitor
* resistors
* button
* RGB led


