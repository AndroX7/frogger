#include <iostream>
#include <vector>
#include <string>
using namespace std;


class TileData {
public:

    //Koordinat Tile
    int x;
    int y;

    //Tipe Tile
    bool isWater;
    bool isBank;
    bool isLilyPad;
    bool isTurtle;
    bool isLog;
    bool isFrog;
    bool isBug;
    bool isEndRow;
    string displayOutput() const {
        if(isFrog){
            return "F";
        }
        if(isBug){
            return "B";
        }
        if(isBank){
            return "x";
        }
        if(isLilyPad){
            return "o";
        }
        if(isTurtle){
            return "T";
        }
        if(isLog){
            return "L";
        }
        if(isWater){
            return "~";
        }
        return "";
    }
    
     void displayDebug () const {
        cout<<" ";
        cout << "["<<x<<","<<y<<","<<displayOutput()<<"]";
        cout<<" ";
        if(isEndRow){
            cout<<endl;
        }
    }

    void displayBoard () const {
        cout<<" ";
        cout << displayOutput();
        cout<<" ";
        if(isEndRow){
            cout<<endl;
        }
    }
    
};

class Insect {
public:
    //Koordinat Bug
    int x;
    int y;
};


//Global Variable
vector<TileData> daftarTile;
vector<Insect> daftarBug;
int frogPositionX=8;
int frogPositionY=4;
int frogLives =3;

void moveFrog(string command){
    if(command=="w"){
        if(frogPositionX-1>=0){
            frogPositionX-=1;
        }    
    }
    else if(command=="a"){
        if(frogPositionY-1>=0){
            frogPositionY-=1;
        }    
    }
    else if(command=="s"){
        if(frogPositionX+1<=8){
            frogPositionX+=1;
        }    
    }
    else if(command=="d"){
        if(frogPositionY+1<=8){
            frogPositionY+=1;
        }    
    }
    for (TileData& tile : daftarTile) {
        tile.isFrog=false;
        if(tile.x==frogPositionX&&tile.y==frogPositionY){
            tile.isFrog = true;
        }
    }

}

void bugMovement(){
    for (Insect& bug : daftarBug) {
        for (TileData& tile : daftarTile) {
                if(tile.x==bug.x){ //ada di x 
                    if((tile.isLog||tile.isTurtle)&&(tile.y==bug.y+1)){
                        //Cek bergerak ke kanan
                        bug.y=bug.y+1;
                        break;
                    }
                    // else if((tile.isLog||tile.isTurtle)&&(tile.y+1==bug.y)){
                    //     //Cek bergerak ke kiri
                    //     bug.y=bug.y-1;
                    //     break;
                    // }
                }
        }
    }
    for (Insect& bug : daftarBug) {
        for (TileData& tile : daftarTile) {
                if(tile.x==bug.x&&tile.y==bug.y){
                    tile.isBug=true;
                }else{
                    tile.isBug=false;
                }
        }
    }
}

void commandHandler(){
    string command;
    cout<<"Enter Command: ";
    cin>>command;
    if(command=="l"){
        //Command Adding Logs
        int row;
        int startColumn;
        int endColumn;
        cin>>row>>startColumn>>endColumn;
        for (TileData& tile : daftarTile) {
                if(tile.x==row&&(tile.y>=startColumn&&tile.y<=endColumn)){
                    tile.isLog=true;
                    tile.isWater=false;
                }
        }
    }else if(command=="c"){
        //Command clear
        int row;
        cin>> row;
        //Check if there is frog 
        bool thereIsAFrog=false;
        for (TileData& tile : daftarTile) {
                if(tile.x==row&&tile.isFrog){
                    thereIsAFrog=true;
                }
        }
        //Check if there is a lilypad
        bool thereIsALilyPad=false;
        for (TileData& tile : daftarTile) {
                if(tile.x==row&&tile.isLilyPad){
                    thereIsALilyPad=true;
                }
        }
        if(!thereIsAFrog&&!thereIsALilyPad){
            //clear data
            for (TileData& tile : daftarTile) {
                if(tile.x==row){
                    tile.isWater=true;
                    tile.isBank=false;
                    tile.isLilyPad=false;
                    tile.isTurtle=false;
                    tile.isLog=false;
                    tile.isFrog=false;
                    tile.isBug=false;
                }
            }
        }
    }
    else if(command=="r"){
        //Command Remove Log
        int row;
        int column;
        cin>> row;
        cin>> column;
        bool isDetectingLog=false;
        int startLog=0;
        int endLog=0;
        int counter=0;
        for (TileData& tile : daftarTile) {
                if(tile.x==row&&tile.isLog){
                    if(!isDetectingLog){
                        startLog = tile.y;
                        isDetectingLog=true;
                    }
                    counter++;
                }else if(tile.isWater&&isDetectingLog){
                    endLog=startLog+counter-1;
                    isDetectingLog=false;
                }
        }

        for (TileData& tile : daftarTile) {
            if(tile.x==row&&(tile.y>=startLog&&tile.y<=endLog)){
                    if(column>=startLog&&column<=endLog){
                        tile.isWater=true;
                        tile.isLog=false;
                        tile.isBug=false;
                    }else{
                        isDetectingLog=false;
                        startLog=0;
                        endLog=0;
                        counter=0;
                    }
                }
        }
        
    }
    else if((command=="w"||command=="a")||(command=="s"||command=="d")){
        moveFrog(command);
        bugMovement();
    }
    else if(command=="b"){
        int row;
        int column;
        cin>>row>>column;;

        for (TileData& tile : daftarTile) {
                if(tile.x==row&&tile.y==column){
                    if(tile.isLog||tile.isTurtle){
                        Insect bug;
                        bug.x=row;
                        bug.y=column;
                        daftarBug.push_back(bug);
                        tile.isBug=true;
                    }
                }
        }
    }
}

void resetFrogPositionWhenDead(int x,int y){
    frogPositionX=x;
    frogPositionY=y;
     for (TileData& tile : daftarTile) {
        tile.isFrog=false;
        if(tile.x==x&&tile.y==y){
            tile.isFrog = true;
        }
    }
}

void disPlayBoard(){
    for (const TileData& tile : daftarTile) {
        // tile.displayDebug();
        tile.displayBoard();
    }
}

int main() {
    int jumlahBaris=9;
    int jumlahKolom=9;

    //===================================================
    //Initialize Board with object (1.1)
    for (int i = 0; i < jumlahBaris; ++i) {
        TileData tile;
        for (int j = 0; j < jumlahKolom; ++j) {
            tile.x=i;
            tile.y=j;
            tile.isWater=true;
            tile.isBank=false;
            tile.isLilyPad=false;
            tile.isTurtle=false;
            tile.isLog=false;
            tile.isFrog=false;
            tile.isBug=false;
            tile.isEndRow=false;

            //Define lilypad di baris pertama
            if(j%2==0&&i==0){
                tile.isLilyPad=true;
                tile.isWater=false;
            }

            //Define Frog
            if(i==8&&j==4){
                tile.isFrog=true;
                tile.isWater=false;
                tile.isBank=true;
            }

            //Define Bank
            if(i==8&&!tile.isFrog){
                tile.isBank=true;
                tile.isWater=false;
            }

            //Set Baris Baru
            if(j==8){
                tile.isEndRow=true;
            }
            daftarTile.push_back(tile);
        }
    }
    //===================================================

    //===================================================
    //Adding Turtles(1.2)  

    int numOfTurtle;
    cout<<"How many turtle?: ";
    cin>>numOfTurtle;
    for(int turtleCount = 0; turtleCount < numOfTurtle; ++turtleCount){
        int x=0;
        int y=0;
        cin>>x>>y;
        for (TileData& tile : daftarTile) {
                if(tile.x==x&&tile.y==y){
                    tile.isTurtle=true;
                    tile.isWater=false;
                }
        }
    }
    //===================================================

    //OUTPUT BOARD
    disPlayBoard();

    //===================================================

    //Making Game Loop(1.3)
    bool isPlaying=true;
    bool isWin=false;
    bool isLose=true;
    int frogLives=3;
    while(isPlaying){
        commandHandler();

        //Game Logic
        for (TileData& tile : daftarTile) {
                //FROG Meet Water 
                if(tile.isFrog&&tile.isWater){
                    disPlayBoard();
                    frogLives-=1;
                    resetFrogPositionWhenDead(8,4);
                    cout<<endl;
                    cout<<"# LIVES LEFT : "<< frogLives<<" #"<<endl;
                    cout<<endl;
                }
                //FROG Meet Bug 
                else if(tile.isFrog&&tile.isBug){
                    disPlayBoard();
                    frogLives-=1;
                    resetFrogPositionWhenDead(8,4);
                    cout<<endl;
                    cout<<"# LIVES LEFT : "<< frogLives<<" #"<<endl;
                    cout<<endl;
                }
                else if(tile.isFrog&&tile.isLilyPad){
                    disPlayBoard();
                    frogLives-=1;
                    resetFrogPositionWhenDead(8,4);
                    isWin=true;
                }
        }

        //===================================================
        disPlayBoard();
        if(frogLives==0){
            isLose = true;
            cout<<"YOU LOSE, Thank you For Playing";
            break;
        }else if(isWin){
            cout<<endl;
            cout<<"YOU WIN, Thank you For Playing";
            break;
        }

    }
    return 0;
}