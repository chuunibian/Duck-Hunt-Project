/******************************************************/
/*  Name:  Austin Bian, Jackson Trigg  Date:  12/4/22 */
/*  Group: 6    File:  main.cpp                       */
/*  Instructor:  Prof. Kecskemety 08:00 AM            */
/******************************************************/

/* Include neccessary OSU Proteus libraries */
#include "FEHLCD.h" 
#include "FEHImages.h"
#include "FEHUtility.h"
#include "FEHRandom.h"
#include <math.h>

/* Decleration of Class GameFunction. This class houses all of the methods needed to run the game as well as all of the instance variables. */
class GameFunctions 
{
    public:
    void LoadMenuFunction(); //These are the named methods
    void LoadLevelFunction();
    void ResetStatisticsFunction();
    void LoadTargetFunction();
    void TargetTouchFunction(float, float);

    int levelcounter = 1; //these are the instance variables 
    float playerscore = 0;
    int failflag = 0;
    int maxlevelcounter = 1;
    int maxplayerscore = 0;


};

/* Decleartion of LoadMenuFunction method this method loads the menu screen and the buttons and the functionality of the buttons, 
but it calls another method when "play game" is pressed. */
void GameFunctions::LoadMenuFunction()
{
    float x,y; //declare float x,y for location

    FEHImage menubackground, menutitle; //declare FEHImage file class objects

    menubackground.Open("duckhuntbackgroundFEH.pic"); //open the .pic files
    menutitle.Open("titletitle2FEH.pic");
   
    while(true) //indefinite loop that keeps placeing the menu background and buttons with funtionality 
    {
        menubackground.Draw(320,240); //draw the images full scale
        menutitle.Draw(100,5);
       
        LCD.SetFontColor(ORANGE); //set the font color to orange

        LCD.WriteAt("Play Game", 110, 110); //output text for the buttons
        LCD.WriteAt("View Statistics", 110, 135);
        LCD.WriteAt("View Instructions", 110, 160);
        LCD.WriteAt("View Credits", 110, 185);

        LCD.SetFontColor(GREEN); //set font color to green

        LCD.DrawRectangle(110,110,200,20); //draw rectangles that act as buttons
        LCD.DrawRectangle(110,135,200,20);
        LCD.DrawRectangle(110,160,200,20);
        LCD.DrawRectangle(110,185,200,20);

        LCD.ClearBuffer(); //clear touch buffer
       
        while(!LCD.Touch(&x, &y)){}; //wait for touch
        while(LCD.Touch(&x, &y)){};
       
        if(y >= 100 && y <= 120 && 320 >= x && x >= 110) //these are if statements that enter when positioning is correct
        {
            LoadLevelFunction(); //This is the Play Game button and it will call the loadlevelfunction method to laod the level.
        }
        else if(y <= 160 && y > 120 && 320 >= x && x >= 110) //This is the touch conditions for the View Statistics button
        {
            LCD.SetFontColor(WHITE);//set font to white
            LCD.FillRectangle(0,0,320,239); //draw full scale rectangle

            LCD.SetFontColor(BLACK); //set font to black
            LCD.WriteLine("Heighest Player Score: "); //write text to screen
            LCD.WriteLine(maxplayerscore);
            LCD.WriteLine("Highest Level: ");
            LCD.WriteLine(maxlevelcounter);

            LCD.SetFontColor(ORANGE); //go back button
            LCD.DrawRectangle(19, 73, 100, 20);
            LCD.WriteAt("Go Back", 20, 75);

            do //go back button touch
            {
                LCD.ClearBuffer();
                while(!LCD.Touch(&x, &y)){};
                while(LCD.Touch(&x, &y)){};
            } while(y >= 90 || y <= 50);
        }
        else if(y <= 180 && y > 160 && 320 >= x && x >= 110) //This is the touch conditions for the view instructions button
        {
            LCD.SetFontColor(WHITE); //set font to white
            LCD.FillRectangle(0,0,320,239); //draw full scale rectangle

            LCD.SetFontColor(BLACK); //set font to black

            LCD.WriteAt("Instructions for game: ",5,40); //write instructions
            LCD.WriteAt("You have to click",5,65);
            LCD.WriteAt("on the ducks or targets ",5,90);
            LCD.WriteAt("to gain points ",5,115);
            LCD.WriteAt("Hit a certain amount to ",5,140);
            LCD.WriteAt("Pass the level ",5,165);

            LCD.SetFontColor(ORANGE); //go back button draw
            LCD.DrawRectangle(19, 1, 100, 20);
            LCD.WriteAt("Go Back", 20, 3);

            do //go back button touch tester
            {
                LCD.ClearBuffer();
                while(!LCD.Touch(&x, &y)){};
                while(LCD.Touch(&x, &y)){};

            } while(y >= 25 || y <= 1);
        }
        else if(y <= 200 && y > 180 && 320 >= x && x >= 110) //This is the touch conditions for the Credits button
        {
            LCD.SetFontColor(WHITE); //set font color to white
            LCD.FillRectangle(0,0,320,239); //draw full scale rectangle background

            LCD.SetFontColor(BLACK); //set font color to black
            LCD.WriteLine("Credits: Austin, Jackson."); //write to the screen

            LCD.SetFontColor(ORANGE); //go back button
            LCD.DrawRectangle(19, 73, 100, 20);
            LCD.WriteAt("Go Back", 20, 75);

            do //touch tester for the go back button
            {
                LCD.ClearBuffer();
                while(!LCD.Touch(&x, &y)){};
                while(LCD.Touch(&x, &y)){};

            } while(y >= 90 || y <= 50);

        }
    }

    menutitle.Close(); //close the .pic files
    menubackground.Close();
}

/* This is the LoadLevelFunction method. This method will load in the level description screen and the level background. It will call method LoadTargetFunction to begin 
the game mechanics */
void GameFunctions::LoadLevelFunction()
{
    FEHImage GameBackground;

    float x,y; //decleare variables for the position

    GameBackground.Open("gamebackgroundFEH.pic"); //open background graphic 

    /* This set of code will load in the level description screen which is a black screen with white text indicating what level is and what is the score
    required to pass the level */
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(0,0,320,240);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Level: ", 100,120);
        LCD.WriteAt(levelcounter,195,120);
        LCD.WriteAt("Score Requirement: ", 45, 150);
        LCD.WriteAt((levelcounter * 350), 260, 150);
        Sleep(3.00);

    /* While loop that loads the real graphic background then calls LoadTargetFunction method to load in the ducks and game mechanics.*/
    while(true)
    {
        GameBackground.Draw(320,240);
        Sleep(1.00);

        LoadTargetFunction();
    }

    GameBackground.Close();
}

/* This is the LoadTargetFunction method. It loads in the duck targets by random generation and it also loads in a functional timer and functional score tracker */
void GameFunctions::LoadTargetFunction()
{
    FEHImage DuckSprite;

    int timelimit=30; //variables needed for the timer
    float timediff=0;
    int currenttime=0;
    int starttime=0;

    float randomX; //variables needed for the touch
    float randomY;
    float x,y;

    DuckSprite.Open("duckFEH.pic"); //open the duck picture

    currenttime=TimeNow(); //variables needed for the time
    starttime=TimeNow();

    /* This do while loop generates the random duck sprite positioning, and writes the timer and score box. As the loop repeats the sprites, timer, and score all should update.
    This while loop also calls the TargetTouchFunction method. */
    do
    {
        randomX = (Random.RandInt() % 10) * 25; //generate random x coordinate
        randomY = (Random.RandInt() % 10) * 20; //generate random y coordinate
        DuckSprite.Draw(randomX, randomY); //place sprite at the randomly generated coordinates

        /* This code will put the timer into the user interface */
        LCD.SetFontColor(BLACK); 
        LCD.FillRectangle(180, 200, 127, 35);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Time: ",180,200);
        LCD.WriteAt("Seconds ",200,220);
        LCD.WriteAt(floor(timediff), 240, 200);

        timediff=TimeNow()-starttime; //calcuate the difference in time

        TargetTouchFunction(randomX, randomY); //call the target touch function with the random coordinates produced above. This method will check touch.

        /* This code will put the scoreboard into the user interface */
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(5,200, 120, 35);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Score: ", 5,200);
        LCD.WriteAt(playerscore, 5, 217);
        
        Sleep(2.00 / levelcounter); //This is the cooldown between duck spawns, It scales with difficulty.

    } while(timediff <= timelimit);

    /* This segment of code will update the timer and it will place a round over box onto the screen.*/
    LCD.SetFontColor(BLACK); 
    LCD.FillRectangle(210, 200, 100, 35);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Time: ",180,200);
    LCD.WriteAt("Seconds ",200,220);
    LCD.WriteAt(30, 240, 200);
    LCD.SetFontColor(RED); 
    LCD.FillRectangle(105, 100, 120, 23);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("ROUND OVER", 105, 100);
    
    /* This is to check if the score for the round was higher than the max score */
    if(playerscore > maxplayerscore)
    {
        maxplayerscore = playerscore; 
    }

    /* These are logic statements that will check if the player meet the score requirement or not. If the player meets, then the next level will be loaded with the 
    levelcounter incremented. Since the difficulty is scaled by the levelcounter, it essentially increases the level and the difficulty. The reset statistics method is also called */
    if(playerscore > (levelcounter * 350))
    {
        levelcounter = levelcounter + 1; //increment the level counter

        if(levelcounter > maxlevelcounter) //This will determine if the current level is the max level
        {
            maxlevelcounter = levelcounter;
        }

        /* This code will print out the next level box to the user interface */
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(105, 150, 120, 23);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Next Level", 105, 150);

        /* This while loop will keep looping and checking use touch until the correct position for the button is touched. If it is touched the ResetStatisticsFunction method
        is called and the LoadLevelFunction is called to go to next level */
        while(1)
        {
            while(!LCD.Touch(&x, &y)){};
            while(LCD.Touch(&x, &y)){};

            if( x >= 105 && x <= 225 && y >= 150 && y <= 173)
            {
                ResetStatisticsFunction();
                LoadLevelFunction();
            }

        }
    }
    /* This is if the player does not meet the score requirement*/
    else
    {
        /* This code will make a main menu box in the user interface. */
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(105, 150, 120, 23);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Main Menu", 105, 150);

        failflag = 1; //Initilization of fail flag to mimic the status of "FAILED"

        /* This while loop will keep looping and checking use touch until the correct position for the button is touched. If it is touched the ResetStatisticsFunction method
        is called and the LoadMenuFunction is called to go back to the menu */
        while(1)
        {
            while(!LCD.Touch(&x, &y)){};
            while(LCD.Touch(&x, &y)){};

            if( x >= 105 && x <= 225 && y >= 150 && y <= 173)
            {
                ResetStatisticsFunction();
                LoadMenuFunction();
            }

        }
    }


}

/* This is the TargetTouchFunction method. This method will check if the position the user touches is the same position as where the duck has spawned.*/
void GameFunctions::TargetTouchFunction(float PositionX, float PositionY)
{
    FEHImage DuckDeathImage; 

    float TouchX, TouchY; //the current touch position variables

    /* This loop will wait for user touch then check if the touch is the corret position, if correct then points are incremented and the death sprite is drawn and the funciton returns
    void */
    do
    {
        DuckDeathImage.Open("DuckDeathSpriteFEH.pic");
        LCD.ClearBuffer();

        while(!LCD.Touch(&TouchX, &TouchY)){}; //wait for human touch
        while(LCD.Touch(&TouchX, &TouchY)){};


        if(TouchX >= PositionX && TouchX <= (PositionX + 50) && TouchX && TouchY >= PositionY && TouchY <= (PositionY + 50))
        {
            
            playerscore+=50;
            DuckDeathImage.Draw(PositionX,PositionY);
            DuckDeathImage.Close();

            return;
        }

    } while (true);
}

/* This is the ResetStatisticsFunction method. This method will reset the statistics accordingly to the state of the game. Returns Void */
void GameFunctions::ResetStatisticsFunction()
{
    playerscore = 0; //Make the playerscore instance variable be 0

    if(levelcounter > 5 || failflag == 1) //make the level counter reset to 1 if level 5 is completed or failed a level
    {
        levelcounter = 1;
    }

    failflag = 0; //revert fail flag

    return;
}


int main()
{
    GameFunctions Menu; //declear menu object of gamefunction class
    Menu.LoadMenuFunction(); //load the load menu funciton in the class. 

    LCD.Clear(); //clear the lcd 

    while (1) {
        LCD.Update();
        // Never end
    }

    return 0;
}