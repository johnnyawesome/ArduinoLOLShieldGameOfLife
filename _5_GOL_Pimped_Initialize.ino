
/*Working Game of Life
 * Resets itself if everything dies off, or if ther is one or more simple oscillating form left
*/

#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

#define Delay 200

const byte SIZEX = 14;
const byte SIZEY = 9;

byte grid[SIZEX][SIZEY];
byte nextGrid[SIZEX][SIZEY];

byte difference = 0;
byte lastdifference = 0;
byte boring = 0;
byte alive = 0;
byte lastalive = 0;

void setup(){

 Serial.begin(9600);
  
 LedSign::Init(); //Initialize LOLShield  
 LedSign::SetBrightness(40);
 
 randomSeed(analogRead(0)); //Initialize Random Number

 clearScreen();
 
 initialize();
 
 delay(Delay);
 
}

void loop(){

  difference = 0;
  alive = 0;

  clearScreen();

  lifecycle();

  showGame();

  fillnextgrid();

  checksamegrid();

  delay(Delay);
    
}  


//Empty Screen
void clearScreen(){
  
 LedSign::Clear();
 
}

//Initializes the very first screen with random cells
void initialize(){

  clearScreen();

  //Turn ALL Led's on counting up
  for(int x = 0; x < SIZEX;x++){
  
     for(int y = 0; y < SIZEY;y++){

      grid[x][y] = 1;
      delay(15);
      LedSign::Set(x,y,grid[x][y]);
    }    
  }

  //Turn SOME Led's off counting up
  for(int x = 0; x < SIZEX;x++){
  
     for(int y = 0; y < SIZEY;y++){

        grid[x][y] = random(2);
        LedSign::Set(x,y,grid[x][y]);
        delay(20);
     }
  }
  delay(Delay * 5);    
}


void lifecycle(){

  for(int x = 0; x < SIZEX; x++){
  
     for(int y = 0; y < SIZEY; y++){

         
         //Count all the neighboring cells
         byte count = neighbours(x, y);


         // The gamerules
         
         // born
         if(grid[x][y] == 0 && count == 3){
          nextGrid[x][y] = 1;
          difference++;
          alive++;
         }


         // Alive and stay alive
         if(grid[x][y] == 1 && count == 2 || count == 3){
          nextGrid[x][y] = 1;
          alive++;
         }
         

         // Alive and die
         if(grid[x][y] == 1 && count < 2 || count > 3){
          nextGrid[x][y] = 0;
          difference++;
          alive--;
         }
          

         //Debugging the neighbor count
         //Serial.println(count);
         //delay(10);
         
       
    }    
  }   
}

//Writes back all the values that lifecycle() calculated into the temporary 'nextGrid-array' into 'grid-array'
void fillnextgrid(){

  for(int x = 0; x < SIZEX; x++){
  
     for(int y = 0; y < SIZEY; y++){

        grid[x][y] = nextGrid[x][y];
       
    }    
  }   
}

//Counts the neighbours and gives back the number of neighbours
byte neighbours(byte x, byte y) {
  return grid[(x + 1) % SIZEX][y] + 
    grid[x][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][y] + 
    grid[x][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + SIZEY - 1) % SIZEY];
}


//Checks if the grid is the same. It does so by ckecking if a cell is born or has died (difference),
//and by comparing the alive count to a previous value.
void checksamegrid(){
  
  if (difference == lastdifference && alive == lastalive) boring++;  

  if (boring >= 25){
    delay(Delay * 3);
    initialize();
    boring = 0;
  }
  
  lastdifference = difference;
  lastalive = alive;

  //if the grid is completely empty, no need to wait for 'boring' to count up. initialize immediately.
  int emptygrid = 0;
  
  for(int x = 0; x < SIZEX; x++){
     
    for(int y = 0; y < SIZEY; y++){
      
      emptygrid = emptygrid + grid[x][y];  
    }
  }
  
  if (emptygrid == 0){   
    clearScreen();
    delay(Delay * 3);
    initialize();
  }

  //Serial.println(alive);

}
  


        

//Shows the gamescreen
void showGame(){

  for(int x = 0; x < SIZEX; x++){
  
     for(int y = 0; y < SIZEY; y++){
    
      LedSign::Set(x,y,grid[x][y]);
      
    }    
  }  
}

  
