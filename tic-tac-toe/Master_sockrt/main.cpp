/*********************************************

//Server -- Code

FUNCTION TO RETURN GAME STATUS
1 FOR GAME IS OVER WITH RESULT
2 GAME IS BEGIN NOW
3 FOR GAME IS IN PROGRESS
O GAME IS OVER AND NO RESULT
 **********************************************/

#include <iostream>
#include <winsock2.h>
#include<stdio.h>
#include<conio.h>
#include<windows.h>

using namespace std;

////Function Definations for Socket
int server_setup();
void client_diss();
void send_data();
void rec_data();

//Function Definations for game

void board_design();
void nav();
void print_array();
void checkwin();
void update_data();
void gotoxy(short x, short y);
void update_game(char pos, char result);
void wel_screen();
void new_game();
void result_Display(int x);
void game_rematch();

//Socket variable
SOCKET serverSock, clientSock;

// Global Variable
// Data Array

char square[10] = {'0','1','2','3','4','5','6','7','8','9'};

//defalt location of curser
int x = 8 , y = 2;
// input fron keybord
int ip;
//result of game
char result = '2';
//At which position data is update
char position = '0';
// game mode
int mode =0;
// player name
 char player_name1[20];
 char name_wait ='0';



int main()
    {
    new_game();

    while(1){
             if (mode == 0 )
                {
                     ip = getch();
                     nav();
                     print_array();

                if ((result == '1') || (result == '0'))
                        {
                            result_Display(2);
                        }
                }

             if (mode == 1)
                {

                rec_data();

                }

             if (mode == 2)
                {
                    ip = getch();
                    if (ip =='y')
                        {
                            exit(0);
                        }
                    if (ip == 'r')
                        {
                        game_rematch();
                        }
                }

            }
    }

 /*********************************************

 //Disply result//


 **********************************************/



    void result_Display(int x){

            gotoxy(0,18);

            printf("int x = %d & result %c\n",x,result);
        // Display Result
        if ((x == 1)&&(result == '1')){
            printf("PLAYER 2 WIN \n");
        }
        else if ((x == 2)&&(result == '1')){
            printf("PLAYER 1 WIN \n");
        }
        else if (result == '0'){
            printf("Game Draw\n");
        }
            printf("GAME OVER");
             //Ask user for rematch Or quit
            printf("\nDo you want quit the match type y Or for rematch type r");
        mode = 2;
    }

 /*********************************************

 //Executed game 1st time//


 **********************************************/

    void new_game(){

        wel_screen();
        print_array();
		board_design();
		gotoxy(y,x);
        server_setup();
        //Send player Name to client
        send(clientSock, player_name1, sizeof(player_name1), 0);
        //wait for client name
        while(name_wait == '0'){
           rec_data();
        }
    }
 /*********************************************

 //rematch conf //


 **********************************************/

    void game_rematch(){

        // again match Process

        // Data Array

        square[0] = '0';
        square[1] = '1';
        square[2] = '2';
        square[3] = '3';
        square[4] = '4';
        square[5] = '5';
        square[6] = '6';
        square[7] = '7';
        square[8] = '8';
        square[9] = '9';

        // Global Variable
        //defalt location of curser
        x = 8 ; y = 2;
        //result of game
        result = '2';
        //At which position data is update
        position = '0';
        // game mode
        mode =0;
        name_wait ='0';

        print_array();
		board_design();
        gotoxy(y,x);
        //Send player Name to client
        send(clientSock, player_name1, sizeof(player_name1), 0);
        //wait for clint name
        while(name_wait == '0'){
           rec_data();
        }
    }
 /*********************************************

 // Send data to client -> position where data change , game status  //


 **********************************************/

    void send_data(){

        char buffer[1024];
        buffer[0] = position;
        buffer[1] = ',';
        buffer[2] = result;
        buffer[4] = '\0';
        send(clientSock, buffer, sizeof(buffer), 0);
        //cout << "Message sent!" << endl;

    }

/*********************************************

 // Rec data from clint & update main data buffer //


 **********************************************/

    void rec_data(){

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(clientSock, buffer, sizeof(buffer), 0);
        gotoxy(25,25);
        cout << "Client says:  " << buffer << endl;
        gotoxy(y,x);

        if (result == '2' ){
                gotoxy(0,5);
                printf("Player 2 Symbol - (o) Name - %s",buffer);
                gotoxy(y,x);
                name_wait = '1';
        }

        if (result != '2'){
        update_game(buffer[0],buffer[2]);
        memset(buffer, 0, sizeof(buffer));
        mode = 0;
        }
    }
/*********************************************

 // Socket Setup//


 **********************************************/



    int server_setup(){

        WSADATA WSAData;
        SOCKADDR_IN serverAddr, clientAddr;

        WSAStartup(MAKEWORD(2,0), &WSAData);
        serverSock = socket(AF_INET, SOCK_STREAM, 0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5555);

        bind(serverSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(serverSock, 0);

     //   cout << "Listening for incoming connections..." << endl;

        int clientAddrSize = sizeof(clientAddr);

        if((clientSock = accept(serverSock, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
            {
            //cout << "Client connected!" << endl;
            return 1;
            }

    }

/*********************************************

 // Close Socket //


 **********************************************/

    void client_diss(){

        closesocket(clientSock);
      //  cout << "Client disconnected." << endl;
    }

/*********************************************

 //Print data array & result //
 Use for debug perpose

 **********************************************/

void print_array()
    {

        gotoxy(20,20);

        for (int i =0 ; i < 10;i++){
            printf("%c",square[i]);
        }

        gotoxy(22,22);
        printf("Rseult = %c ", result);

        gotoxy(y,x);

}

/*********************************************

 //This function provided navigation into game//
 d->right move
 a->left move
 w->up move
 s->down move

 **********************************************/

void nav(){

	if (ip == 'd'){

	 	y = (((y + 6)%20) + ((y == 14)*2));
	 }

	else if (ip == 'a'){

		y = (((y - 6)*(y != 2)) + ((y == 2)*14));
	 }

	else if (ip == 's'){

	 	x = (((x + 3)%17) + ((x == 14)*8));
	 }

	else if (ip == 'w'){

	 	x = (((x - 3)*(x != 8)) + ((x == 8)*14));
	 }

	gotoxy(y,x);

	// if data input x then call data save algo

	if (ip == 'x'){
		update_data();
		mode = 1;

	}
}

/*********************************************

FUNCTION save data in square array & display x

 **********************************************/

void update_data(){

	//Display X on game

		printf("x");
		gotoxy(y,x);

    //Save data into array

		if ((x == 8 ) && (y == 2) && (square[1] == '1')) {
		square[1] = 'x';
		position = '1' ;
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 8 ) && (y == 8) && (square[2] == '2')) {
		square[2] = 'x';
        position = '2';
        //Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 8 ) && (y == 14) && (square[3] =='3' )) {
		square[3] = 'x';
		position = '3';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 11 ) && (y == 2) && (square[4] == '4')) {
		square[4] = 'x';
		position = '4';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 11 ) && (y == 8) && (square[5] == '5')) {
		square[5] = 'x';
		position = '5';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 11 ) && (y == 14) && (square[6] == '6')) {
		square[6] = 'x';
		position = '6';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 14 ) && (y == 2) && (square[7] == '7')) {
		square[7] = 'x';
		position = '7';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 14 ) && (y == 8) && (square[8] == '8')) {
		square[8] = 'x';
		position = '8';
		//Call for game result
		checkwin();
         //send data to client
		send_data();
		}

		if ((x == 14 ) && (y == 14) && (square[9] == '9')) {
		square[9] = 'x';
		position = '9';
        //Call for game result
		checkwin();
        //send data to client
		send_data();

		}
}

/*********************************************

 //update date with upcomming data//
 data -> location,game status

 **********************************************/

 void update_game(char pos, char res){


     if (pos == '1'){
        square[1] = 'o';
        gotoxy(2,8);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '2'){
        square[2] = 'o';
        gotoxy(8,8);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '3'){
        square[3] = 'o';
        gotoxy(14,8);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '4'){
        square[4] = 'o';
        gotoxy(2,11);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '4'){
        square[4] = 'o';
        gotoxy(2,11);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '5'){
        square[5] = 'o';
        gotoxy(8,11);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '6'){
        square[6] = 'o';
        gotoxy(14,11);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '7'){
        square[7] = 'o';
        gotoxy(2,14);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '8'){
        square[8] = 'o';
        gotoxy(8,14);
        printf("o");
        gotoxy(y,x);
     }

    else if (pos == '9'){
        square[9] = 'o';
        gotoxy(14,14);
        printf("o");
        gotoxy(y,x);
     }


     gotoxy(28,28);
     printf("%c,%c", pos,result);
     gotoxy(y,x);

// Game if finish for display result, exit & rematch

     if ((res == '1') ||(res == '0')){
            result = res;
            result_Display(1);
        }

 }

/*********************************************

FUNCTION TO RETURN GAME STATUS
1 FOR GAME IS OVER WITH RESULT
2 GAME IS BEGIN NOW
3 FOR GAME IS IN PROGRESS
O GAME IS OVER AND NO RESULT
 **********************************************/

void checkwin()
{
	// Cheak for draw
		static int i = 0;

		i = i+1;

		if (i == 9){

			result = '0';
		}

	// Cheak for Win

    if (square[1] == square[2] && square[2] == square[3])
        result = '1';

    else if (square[4] == square[5] && square[5] == square[6])
        result = '1';

    else if (square[7] == square[8] && square[8] == square[9])
        result = '1';

    else if (square[1] == square[4] && square[4] == square[7])
        result = '1';

    else if (square[2] == square[5] && square[5] == square[8])
        result = '1';

    else if (square[3] == square[6] && square[6] == square[9])
        result = '1';

    else if (square[1] == square[5] && square[5] == square[9])
        result = '1';

    else if (square[3] == square[5] && square[5] == square[7])
        result = '1';


	// Status indicate Game is in progess
    else
        result = '3';
}

/*********************************************

 //definition of gotoxy function//
location -> (y,x)
 **********************************************/

void gotoxy(short x, short y)
{
 COORD pos ={x,y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*********************************************

 //Board Design & defalt information//

 **********************************************/

void board_design()
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");

    printf("Player 1 Symbol - (X) Name - %s \n(Wait Until Player 2 is login)\n\n",player_name1);

//Player 2 (O)
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[1], square[2], square[3]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[4], square[5], square[6]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[7], square[8], square[9]);

    printf("     |     |     \n\n");
}

/*********************************************

 //Wel_screen -> input user name //

 **********************************************/
void wel_screen(){
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");

    printf("Please enter Your Name : - ");
    scanf("%s", player_name1);

}
