// ********************************************************* 
// Course: TCP1101 PROGRAMMING FUNDAMENTALS 
// Year: Trimester 1, 2022/23 (T2215) 
// Lab: TT6L 
// Names: JERRY JEREMIAH RAO | Eswarr Ilamaran | Mohammad 
// IDs: 1201101725 | 1221303411 | 1211305449
// Emails: 1201101725@student.mmu.edu.my| 1221303411@student.mmu.edu.my | 1211305449@student.mmu.edu.my
// Phones: 018-2795817 | 017-7542577 | 018-2188417 
// ********************************************************* 




#include <iostream>
#include <iomanip>

// helper variables
int rows;
int columns;
int zombies;
int alienHealth;
int alienDamage;
int alienX, alienY;
int zombiesHealth[10];
int zombiesDamage[10];
int ranges[10];
int zombiesX[10];
int zombiesY[10];
char **grid;
// Define an array of characters
char characters[8] = {'^', 'v', '>', '<', 'h', 'p', 'r', ' '};

using namespace std;

void StartUp();
void customSetting();
void board();
void displayBoard();
void displayHealth();
void toLower(string &s);
bool boundary(int i, int j);
void alienMove(string direction);
void alienMove(string direction, int r, int c);
void zombieMove(int z);
void helpSection();
int Pause();
void arrow();
bool alienWin();
bool alienDead();
bool validPosition(int r, int c);
void hitpod(int r, int c);
void attack(int z, int r, int c, int range);
void resetPath();

int main()
{
    // Seed the random number generator
    srand(time(0));

    // Initial value
    rows = 5;
    columns = 9;
    zombies = 5;
    alienDamage = 0;

    StartUp();
    customSetting();

    alienHealth = (rand() % 5) * 50 + 150;
    grid = new char*[rows];
    for(int i=0; i<rows; i++)
        grid[i] = new char[columns];

    board();

    string command;
    bool endGame = false;

    do {
        displayBoard();
        cout << "\nCommand > ";
        cin >> command;
        toLower(command);
        if( command == "up" || command == "down" || command == "left" || command == "right" ) {
            alienMove(command);
        }
        else if(command == "help"){
            helpSection();
            Pause();
        }
        else if(command == "arrow"){           
            arrow();
            Pause();
        }
        else if(command == "save"){
            // will implement in part 2
            Pause();
        }
        else if(command == "load"){
            // will implement in part 2
            Pause();
        }
        else if(command == "quit"){
            cout << "\nQuiting Game...\n";
            Pause();
            endGame = true;
        }
        else
            cout << "\n\nInvalid Command!!\n\n";

        if( alienWin() ){
            cout << "Alien Wins!\n\n";
            endGame = true;
        }
        else if( alienDead() ){
            cout << "Alien Died (^,^)";
            endGame = true;
        }

    }while( !endGame );

    system("pause");

    return 0;
}



void attack(int z, int r, int c, int range)
{
    if( (abs( r - alienX ) <= range) && (abs( c - alienY ) <= range) )
    {
        cout << "Zombie " << z + 1 << " attacks Alien with damage " << zombiesDamage[z] << "\n";
        alienHealth -= zombiesDamage[z];
    }
    else
    {
        cout << "Zombie cannot attack. Alien is out of range.\n\n";
    }
}

void hitpod(int r, int c)
{
    int i, minDistance = 99, minDisIndex, distance;
    for(i = 0; i < zombies; i++)
    {
        distance = (abs(zombiesX[i] - r) + abs(zombiesY[i] - c));
        if( minDistance > distance ) {
            minDistance = distance;
            minDisIndex = i;
        }
    }

    cout << "Zombie " << minDisIndex + 1 << " is closest to the zombie. It gets a damage of 10.\n\n";
    zombiesHealth[ minDisIndex ] -= 10;
    if (zombiesHealth[ minDisIndex ] <= 0)
    {
        cout << "Zombie " << minDisIndex + 1 << "Died.\n\n";
        zombiesX[ minDisIndex ] = -1;
        zombiesY[ minDisIndex ] = -1;
    }
}

bool validPosition(int r, int c)
{
    if( alienX == r && alienY == c )
        return false;

    for(int i=0; i<zombies; i++)
        if(zombiesX[i] == r && zombiesY[i] == c)
            return false;
    return true;
}

void resetPath()
{
    int i, j;
    for(i = 0; i < rows; i++)
    {
        for( j = 0; j < columns; j++ )
        {
            if(grid[i][j] == '.')
                grid[i][j] = characters[ rand() % 8 ];
        }
    }
}

void zombieMove(int z)
{
    int num;
    cout << "\nZombie " << z+1 << "\'s turn.\n";
    while (true) {
        num = rand() % 4;

        if (num == 0 && !boundary(zombiesX[z] - 1, zombiesY[z]) && validPosition(zombiesX[z] - 1, zombiesY[z]) ){
            grid[zombiesX[z] - 1][zombiesY[z]] = grid[zombiesX[z]][zombiesY[z]];
            grid[zombiesX[z]][zombiesY[z]] = ' ';
            zombiesX[z]--;
            cout << "\nZombie " << z+1 << " moves up\n";
            break;
        }
        else if (num == 1 && !boundary(zombiesX[z] + 1, zombiesY[z]) && validPosition(zombiesX[z] + 1, zombiesY[z])){
            grid[zombiesX[z] + 1][zombiesY[z]] = grid[zombiesX[z]][zombiesY[z]];
            grid[zombiesX[z]][zombiesY[z]] = ' ';
            zombiesX[z]++;
            cout << "\nZombie " << z+1 << " moves down\n";
            break;
        }
        else if (num == 2 && !boundary(zombiesX[z], zombiesY[z]-1) && validPosition(zombiesX[z], zombiesY[z] - 1)){
            grid[zombiesX[z]][zombiesY[z]-1] = grid[zombiesX[z]][zombiesY[z]];
            grid[zombiesX[z]][zombiesY[z]] = ' ';
            zombiesY[z]--;
            cout << "\nZombie " << z+1 << " moves left\n";
            break;
        }
        else if (num == 3 && !boundary(zombiesX[z], zombiesY[z] + 1) && validPosition(zombiesX[z], zombiesY[z] + 1) ){
            grid[zombiesX[z]][zombiesY[z] + 1] = grid[zombiesX[z]][zombiesY[z]];
            grid[zombiesX[z]][zombiesY[z]] = ' ';
            zombiesY[z]++;
            cout << "\nZombie " << z+1 << " moves right\n";
            break;
        }
    }

    attack(z, zombiesX[z], zombiesY[z], ranges[z]);
}

void alienMove(string direction)
{
    alienDamage = 0;
    if(direction == "up")
        alienMove(direction, alienX-1, alienY);
    else if(direction == "down")
        alienMove(direction, alienX+1, alienY);
    else if(direction == "left")
        alienMove(direction, alienX, alienY-1);
    else if(direction == "right")
        alienMove(direction, alienX, alienY+1);
    alienDamage = 0;
    cout << "Alien Move Ends. Reset the path...\n\n";
    Pause();
    resetPath();
    for(int i=0; i<zombies; i++)
    {
        if( zombiesHealth[i] > 0 ) {
            displayBoard();
            zombieMove(i);
            Pause();
        }
    }
}

void alienMove(string direction, int r, int c)
{
    if( boundary(r, c) )
        return;

    int tr, tc;
    displayBoard();
    if(direction == "up")
        cout << "\nAlien moves Up\n";
    else if(direction == "down")
        cout << "\nAlien moves Down\n";
    else if(direction == "right")
        cout << "\nAlien moves Right\n";
    else if(direction == "left")
        cout << "\nAlien moves Left\n";

    tr = alienX;
    tc = alienY;
    grid[alienX][alienY] = '.';
    alienX = r;
    alienY = c;

    if(grid[r][c] == '^')
    {
        grid[r][c] = 'A';
        cout << "Alien get a arrow in upward direction.\n";
        cout << "Alien Damage is increased by 20.\n";
        Pause();
        alienDamage += 20;
        alienMove("up", r-1, c);
        return;
    }

    else if(grid[r][c] == '>')
    {
        grid[r][c] = 'A';
        cout << "Alien get a arrow in Right direction.\n";
        cout << "Alien Damage is increased by 20.\n";
        Pause();
        alienDamage += 20;
        alienMove("right", r, c+1);
        return;
    }

    else if(grid[r][c] == 'v')
    {
        grid[r][c] = 'A';
        cout << "Alien get a arrow in Downward direction.\n";
        cout << "Alien Damage is increased by 20.\n";
        Pause();
        alienDamage += 20;
        alienMove("down", r+1, c);
        return;
    }

    else if(grid[r][c] == '<')
    {
        grid[r][c] = 'A';
        cout << "Alien get a arrow in left direction.\n";
        cout << "Alien Damage is increased by 20.\n";
        Pause();
        alienDamage += 20;
        alienMove("left", r, c-1);
        return;
    }

    else if(grid[r][c] == 'h')
    {
        cout << "Alien get a health.\n20 is Added to Alien Health.\n";
        alienHealth += 20;
    }

    else if(grid[r][c] == 'p')
    {
        cout << "Alien get a pod.\nHandle 10 damage to nearest Zombie.\n";
        hitpod(r, c);
    }

    else if(grid[r][c] == 'r')
    {
        cout << "Alien finds a rock.\n";
        int a = rand() % 3;
        if(a == 0){
            cout << "Alien discover a Pod beneath the Rock.\n";
            grid[r][c] = 'p';
        }
        else if(a == 1){
            cout << "Alien discover a Health beneath the Rock.\n";
            grid[r][c] = 'h';
        }
        else{
            cout << "Alien discover Nothing beneath the Rock.\n";
            grid[r][c] = ' ';
        }
        alienX = tr;
        alienY = tc;
        grid[alienX][alienY] = 'A';
        return;
    }

    else if( grid[r][c] >= '1' && grid[r][c] <= '9' ){
        cout << "Alien hits a Zombie.\n";
        cout << "Alien Attacks with damage " << alienDamage << "\n";
        zombiesHealth[ grid[r][c] - '0' - 1 ] -= alienDamage;
        if (zombiesHealth[ grid[r][c] - '0' - 1 ] <= 0)
        {
            cout << "Zombie " << grid[r][c] << " Died.\nAlien continues to move.\n\n";
            zombiesX[  grid[r][c] - '0' - 1 ] = -1;
            zombiesY[  grid[r][c] - '0' - 1 ] = -1;
        }
        else {
            alienX = tr;
            alienY = tc;
            grid[alienX][alienY] = 'A';
            return;
        }
    }
    else
        cout << "Alien finds an empty space.\n\n";

    grid[r][c] = 'A';
    Pause();
    if(direction == "up")
        alienMove(direction, r-1, c);
    else if(direction == "down")
        alienMove(direction, r+1, c);
    else if(direction == "left")
        alienMove(direction, r, c-1);
    else if(direction == "right")
        alienMove(direction, r, c+1);
}

bool alienWin(){
    for(int i=0;i<zombies;i++){
        if( zombiesHealth[i] > 0 ) {
            return false;
        }
    }
    return true;
}

bool alienDead(){
    if(alienHealth<=0)
        return true;
    return false;
}

bool boundary(int i, int j)
{
    return ( i < 0 || j < 0 || i >= rows || j >= columns );
}

void arrow()
{
    int r, c;
    string direction;
    do{
        cout << "\nEnter row: ";
        cin >> r;

        if(cin.fail() || r < 1 || r > rows)
        {
            cin.clear();
            cin.ignore();
            cout << "\nInvalid Input!! Number of rows are " << rows << ".\n";
        }
    }while(r < 1 || r > rows);

    do{
        cout << "\nEnter column: ";
        cin >> c;

        if(cin.fail() || c < 1 || c > columns)
        {
            cin.clear();
            cin.ignore();
            cout << "\nInvalid Input!! Number of columns are " << columns << ".\n";
        }
    }while(c < 1 || c > columns);

    cin.ignore();
    do{
        cout << "\nEnter direction: ";
        cin >> direction;

        toLower(direction);
        if(direction != "up" && direction != "down" && direction != "left" && direction != "right"){
            cout << "\nInvalid Input!!\n";
        }
    }while(direction != "up" && direction != "down" && direction != "left" && direction != "right");

    if( grid[r-1][c-1] == '^' || grid[r-1][c-1] == '<' || grid[r-1][c-1] == '>' || grid[r-1][c-1] == 'v' ) {
        if(direction == "up")
            grid[r-1][c-1] = '^';
        else if(direction == "down")
            grid[r-1][c-1] = 'v';
        else if(direction == "left")
            grid[r-1][c-1] = '<';
        else if(direction == "right")
            grid[r-1][c-1] = '>';
    } else {
        cout << "\nThis is not an arrow.\n\n";
    }
    cin.ignore();
}

void StartUp()
{
    cout << "|>>>>>>>>>>>>>>>>o<<<<<<<<<<<<<<<<|" << endl;
    cout << "|   Welcome To Alien VS Zombie!   |" << endl;
    cout << "|>>>>>>>>>>>>>>>>o<<<<<<<<<<<<<<<<|" << endl << endl;
    cout << "Press Enter to continue . . ." ;
    cin.ignore();
    cout << " " << endl;
}

void customSetting() {
    cout << "-----------------------" << endl;
    cout << " Default Game Settings" << endl;
    cout << "-----------------------" << endl;
    cout << " " << endl;
    cout << "Board Rows: " << rows << endl;
    cout << "Board Columns: " << columns << endl;
    cout << "Zombie Count: " << zombies << endl << endl;

    char answer;
    cout << "Do you wish to change default setting? (y/n) => ";
    cin >> answer;
    cout << " " << endl;

    // check if user wants to change default setting
    if (tolower(answer) == 'y') { // tolower() is to convert input to lowercase
        cout << "Enter the number of rows: ";
        cin >> rows;
        cout << "Enter the number of columns: ";
        cin >> columns;
        cout << "Enter number of zombie(s):";
        cin >> zombies;

        while (zombies > 10) {
            cout << " " << endl;
            cout << "Number of zombie must below than 10" << endl << endl;
            cout << "Enter number of zombie(s):";
            cin >> zombies;
        }
    }
}

void helpSection()
{
    cout << "\n\tCommands\n";
    cout << "up    - Alien to move up\n";
    cout << "down  - Alien to move down\n";
    cout << "left  - Alien to move left\n";
    cout << "right - Alien to move right\n";
    cout << "arrow - Change the direction of an arrow\n";
    cout << "help  - Display these user commands\n";
    cout << "save  - Save the game\n";
    cout << "load  - Load a game\n";
    cout << "quit  - Quit the game\n\n";
}



void board()
{
    // calculate the middle row and column
    int middleRow = (rows-1) / 2;
    int middleColumn = (columns-1) / 2 ;
    alienX = middleRow;
    alienY = middleColumn;

    // randomly place the zombies on the board
    int x, y;
    for (int i = 0; i < zombies; i++)
    {
        x = rand() % rows;
        y = rand() % columns;
        if( !validPosition(x, y) ) {
            i--;
            continue;
        }
        zombiesX[i] = rand() % rows;
        zombiesY[i] = rand() % columns;
    }

    for(int i=0;i<zombies;i++)
    {
        zombiesHealth[i] = (rand() % 3) * 50 + 150;
        zombiesDamage[i] = (rand() % 3) * 5 + 5;
        ranges[i] =  (rand() % (rows-1) - 1) + 1;
    }

    // Print the grid with your specified characters
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Use the modulo operator to cycle through the characters array
            char c = characters[rand() % 8];
            grid[i][j] = c;
            if (i == middleRow && j == middleColumn)
            {
                grid[i][j] = 'A';
            }
            // check if the current cell is a zombies
            for (int k = 0; k < zombies; k++)
            {
                if (i == zombiesX[k] && j == zombiesY[k])
                {
                    grid[i][j] = k+1+'0';
                }
            }
        }
    }

    // Print column numbers below grid
    cout << setw(5) << " ";
    for (int i = 1; i <= columns; i++)
    {
        cout << setw(2) << i << setw(2) << ' ';
    }
    cout << endl;
}

void displayHealth()
{
    cout << "\nAlien :     Health " << alienHealth << "\tDamage " << alienDamage <<  endl;
    for(int i=0;i<zombies;i++){
        cout << "Zombie "<<i+1<<":  "<<" Health " << zombiesHealth[i] <<
             ", Damage " << zombiesDamage[i] << ", Range " << ranges[i] << endl;
    }
}

void displayBoard(){
    // Print top row of "+" and "-"
    cout << setw(4) << " ";
    for (int i = 1; i <= columns; i++)
    {
        cout << "+" << " - ";
    }
    cout << "+" << endl;

    // Print the grid with your specified characters
    for (int i = 1; i <= rows; i++)
    {
        cout << setw(4) << i;
        for (int j = 1; j <=columns; j++)
        {
            cout << "|" << " " << grid[i-1][j-1] << " ";
        }
        cout << "|" << endl;

        // Print row of "+" and "-"
        cout << setw(5) << "+";
        for (int j = 1; j <= columns; j++)
        {
            cout << " - " << "+";
        }
        cout << endl;
    }

    // Print column numbers below grid
    cout << setw(5) << " ";
    for (int i = 1; i <= columns; i++)
    {
        cout << setw(2) << i << setw(2) << ' ';
    }
    cout << endl;
    displayHealth();
}

void toLower(string &s)
{
    int i=0, l=s.length();
    for(i = 0; i < l; i++)
        if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] -= 32;
}

int ClearScreen()
{
#if defined(_WIN32)
    return std::system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    return std::system("clear");
#endif
}

int Pause()
{
#if defined(_WIN32)
    return std::system("pause");
#elif defined(__linux__) || defined(__APPLE__)
    return std::system(R"(read -p "Press any key to continue . . . " Gamer)");
#endif
}
