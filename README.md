
# _BOMBERMAN_ 💣
<img width="1000" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/6247079b-f57c-4839-b0ff-2ccce60f1ad8">


<details>
  <summary> 
     <h2> A little history 🤓 </h2>
  </summary>
  
          Bomberman games have two main modes: single-player campaigns and multiplayer battles. 
          In single-player, you navigate mazes, defeat enemies, and reach exits. 
          In multiplayer, the goal is to be the last player standing by strategically using bombs.
          The gameplay involves placing bombs strategically to destroy obstacles, enemies, and other players. 
          You can collect power-ups for advantages like bigger explosions or more bombs. 
          If you touch an enemy or get caught in a bomb blast, you lose.
          Apart from the classic maze games, some Bomberman titles include adventures, puzzles, and racing. 
          It's all about smart bomb placement and outlasting opponents.
</details>

<details>
  <summary> 
     <h2> Task Requirements 🤓 </h2>
  </summary>
  
  <details>
  <summary> 
     <h3>Components🤓 </h3>
  </summary>
    
  ### Components 
            • Arduino Uno Board ⌨️
            • Joystick 
            • 8x8 LED Matrix
            • MAX7219
            • Resistors and capacitors as needed
            • Breadboard and connecting wires
            • LED 🚨
            • Buzzer 🔊
            • LCD
  </details>
  <details>
  <summary> 
     <h3>Menu Requirements 🤓 </h3>
  </summary>
    
  Create a menu for your game, emphasis on the game. The player
  should scroll on the LCD with the joystick. The menu should include the 
  following functionality ->
  
  ### MENU 
            🕹️ START GAME
                • If you click on this option, the game will start 
            🕹️ SETTINGS
                • ENTER NAME
                • RESET NAME
                • BRIGHTNESS
                • SOUND
                • RESET HIGHSCORE
                • VIEW MY NAME
            🕹️ ABOUT
                • Shows the creator's name and github.
            🕹️ HOW TO PLAY
                • Shows the navigations rules .   
            🕹️ HIGHSCORE
                • Shows the player's highscore.
  </details>
<details>
<details>
    <summary>INTRO MESSAGE LCD</summary>
    
      🐱 The project lights up, this message will display the message   
         "HI!" on the matrix, and the welcome messages will be played on 
          the LCD:
                                "WELCOME PLAYERS"
                                "ARE YOU READY?"
                                "FOR THIS GAME"
                                "BOMBERMAN"

    
      🤓 After displaying the welcome message on the matrix screen
      a representative picture for the menu will appear.

<img width="300" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/0b96c78d-9b7d-4e0e-b5d5-38dcce74c418">
</details>

<details>
    <summary> START GAME </summary>
  
    🐱 A countdown sequence (3, 2, 1) is displayed, signaling the start of the gameplay.
                
<img width="217" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/6b946d37-d482-4849-8821-a40eb1a009bf">
<img width="217" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/82241178-b397-4908-aa76-6ff6198a3ba3">
<img width="217" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/09036ea9-441c-4bed-94a9-2e2aeb32cad9">
</details>
<details>
When we select the settings, a representative image will appear on the matrix.
<img width="300" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/b82a75d6-1b71-414e-9989-e4225f77bb98">

    <summary> ENTER NAME </summary>
  
    🐱 The USER can set a name of maximum 3 characters, by moving up/down to slide between letters, moving to the right to go to the next character
<img width="300" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/b55a670f-eb6b-486b-a330-51efc11b39b8">

</details>
<details>
    <summary> RESET </summary>
  
    🐱 The USER can reset the name and can change with another one .
        Super Easy !
</details>
<details>
    <summary> BRIGHTNESS </summary>
  
            • LCD BRIGHTNESS
           The user sets the contrast he wants on the LCD, he will be       
           greeted when he chooses this option of the BRIGHTNESS option 
           of the navigation rule (up or down to increase or decrease the 
           contrast level, respectively), and pressing the left button will 
           take him to SETTINGS
           
           • MATRIX CONTRAST 
           The user sets the contrast he wants on the MATRIX, he will be    
           greeted when he chooses this option of the BRIGHTNESS option 
           of the navigation rule (up or down to increase or decrease the 
           contrast level, respectively), and pressing the left button will 
           take him to SETTINGS.
           
</details>
<details>
    <summary> SOUND </summary>
    
    🐱 The USER can CHOOSE if he want some cool music (ON),
        or nothing (OFF) :(
        
</details>
<details>
    <summary> RESET HIGHSCORE </summary>
    
    🐱 If the user chooses this option, he can reset his best score (who wants to do that?). 
    This option is good for those who play with friends (but still do to face).
        
</details>
<details>
    <summary> VIEW MY NAME </summary>
    
    🐱 The user can also see his name to make sure everything is ok .
        
</details>
<details>
    <summary> About </summary>
    
    🐱 Details about the name of this wonderful creator (me, just                 kidding), github where you can see other things I'm working on, etc.
       On top of that, a cute image appears on the matrix.
       
<img width="300" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/551b3210-ccbc-4d02-a5ac-84f439cb77f0">

</details>
<details>
    <summary> How TO Play </summary>
    
    🐱 In this section, the navigation rules that the player must consult  will be indicated.
       • In the game, the player moves classically up, down, left, right  and pressing means placing the bomb.
       • In the menu, the user can navigate between options up/down and if he wants to choose an option, 
       he gives a simple click. 
       Returning to the menu options / sub-menu options are done by swiping to the left. 
       Something super easy.
       And naturally, a cute image is displayed here as well :)
       
<img width="300" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/4046d768-b054-4890-822b-cfb482a7ae81">
</details>
<details>
    <summary> Highscores </summary>
    
    🐱 The user can see his best father-in-law and, on top of that, brag 
       about it to his friends ;)
       
<img width="300" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/8b770dda-89d4-4b48-934f-ba7ce1880488">

</details>
</details>
</details>

<details>
  <summary> 
     <h2> Rules  🤓 </h2>
  </summary>
  
             🐱 Player Control
                Use the joystick to move the player LED within the matrix.
                The player LED blinks slowly, providing a visual indicator 
                of its position.
                The player can move up, down, left, right without going 
                through walls.
             🐱 Wall Destruction
                Navigate the player to break walls strategically.
                Walls cover 50% - 75% of the matrix initially.
                Breaking walls contributes to the player's score.
                Remember, when generating a new map, the player is 
                strategically positioned in such a way that he can move.                
            🐱 Bomb Placement
                Press the joystick button to place a bomb.
                The bomb blinks rapidly, indicating its presence.
                Bombs eliminate walls in all four directions upon 
               detonation.   
            🐱 Scoring
                Score is calculated based on the time taken to win and the 
                number of walls broken.
                A bonus is awarded for each wall destroyed.              
  
            🐱 Lossing the Game 
              Lives decrease when the player is within the radius of the 
              bomb at a distance of 1.
              Thus, the lives will decrease, and at the moment of 0 lives, 
              the game ends and you will be accompanied by sad music.
              
            🔔 Winning the Game
                The game is won when all walls are eliminated. 
                A winning animation, accompanied by a festive jingle, is 
                displayed on the matrix. The total score, 
                including bonuses, is presented in LCD.
                
            ♻️ Resetting the Game
                After the reset, you can change it again in the menu, it's your decisions. 
                Name and best score, settings will be saved        
            🔔 During the Game
                On the screen you will know how many lives you will have 
                and the time is counted in real time. At the same time, 
                your name also appears.

</details>

<details>
  <summary>
  <h2>🕹 Electronic scheme: </h2>
  </summary>

## Matrix 
<img width="400" hight="500" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/95e1beea-e6bb-4dc6-af20-dd0617830be4">

My matrix is ​​of type AS. I used this table to make the correct connections
- Laboratory source
<img width="400" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/8fc92705-f607-47a6-b0ee-b2798439c8e2">
<img width="400" alt="image" src="https://github.com/anacimpeanu/IntroductionToRobotics-2023-2024-/assets/115561036/050bc037-00db-4902-9b37-8958375c7ca7">

</details>

### 📸 Electronic circuit in real life:
<img width="230" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/96dcaee7-907a-40fc-9bcc-197595125db4">
<img width="230" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/fb9c28b3-5fe1-4b37-b904-a30e47aa83c4">
<img width="230" alt="image" src="https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/assets/115561036/1c50904d-1fce-4b5b-85ba-59777edbee8d">

### 📽 Video : [ Mini Matrix Bomberman Game LCD 💣 ]
### 🖥 Code : [ The Code 🖥 ](https://github.com/anacimpeanu/ROBOTICS-MATRIX-PROJECT-BOMBERMAN/blob/main/BOMBERMAN_MINI_GAME.ino)
### 🔌 Important : 
- The red LED has too much internal resistance, and a 330 resistor further reduces the current reaching the LED
- So, for the red LED, I use a resistance of 10, so we allow a current with a higher intensity to reach the LED
- The LedControl library is properly installed and configured for the game to function correctly.
