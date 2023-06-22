#include <iostream>
#include <vector>
#include <map>
#include <string>
#include<algorithm>
#include<map>
#include <cstdlib>
#include <unistd.h>
#include <time.h> 
using namespace std;
map<int,string>rooms;
class Current_state{//Current status's class
	public:
	int health=100;//Set HP
	int equipment[100];//Set up the situation for carrying equipment
	int have_travel[15]={0};//Determine if you have entered the room
	int alive[100];//Set the survival state of each role
	int princess_thought=1;//Set the princess's mood
	int room_monster;//Record the coordinates of the monster's room
	int room_princess;//Record the coordinates of the princess's room
	int if_princess;//Determine whether to bring the princess
	int if_monster=1;//Determine whether the monster exists
	int equipment_number=0;//Set the amount of equipment
	int poisoning=0;//Determine whether poisoning
}; 
class Room_build{
	public:
	int room_connect[15][15];//Room connection array
	void castle_map();//Map generating function
	int lobby_type();
	void build_room();
	void room_connection();
	void room_monstertype();
	int room_princesstype(int last_room);
	int room_Gaschamber();
	int room_HouseofLuck1();
	int room_Equipmenthouse();
	int room_HouseofTests();
	int room_SwampHouse();
	int room_Jail();
	int room_HouseofLuck2();
	int room_Sanatorium();
	int room_LittleMonstersHouse();
	int room_VixenHouse();
	int room_lobbyhouse();
};
//Define state and room objects
Current_state now_state;
Room_build now_room;
int main(){
	now_room.build_room();//Build rooms and connect them 
	//cout<<now_state.room_monster<<" "<<now_state.room_princess<<endl;
	int next_room=now_room.room_connect[10][now_room.lobby_type()];
	int current_room=10;
	for(int i=0;i<100;i++){
		now_state.alive[i]=1;//The initial roles are set to alive
	}
	while(1){
		cout<<endl;
		if(now_state.princess_thought==0){//You can't take the princess when she's at zero
				cout<<"The princess doesn't go with you, you come back alone......"<<endl;
				sleep(2);
				cout<<"Game over"<<endl;
				break;
			}
		if(now_state.poisoning){//Decreased HP during poisoning
			now_state.health-=now_state.poisoning;
		    cout<<"Current Health: "<<now_state.health<<endl;
		    cout<<endl;
		}
		if(now_state.health<=0){//End the game when health is less than 0
			cout<<"Your health is 0"<<endl;
			sleep(2);
			cout<<"It's a pity that you dead!"<<endl;
			break;
			cout<<endl;
		}
		if(next_room==11){//Enter the monster's room
			now_room.room_monstertype();
			break;
		}
		else if(next_room==12||next_room==200){//Enter the princess's room
			if(next_room==200){
				int temp=now_room.room_princesstype(current_room);
				next_room=3;
				continue;
			}//From the princess's room back to the previous room
			int temp2=now_room.room_princesstype(current_room);
			next_room=current_room;
			current_room=now_state.room_princess;	
		}
		else if(next_room==0){//Enter the Gaschamber
			int temp=now_room.room_Gaschamber();
			if(temp>=100){
				continue;
			}
			current_room=0;
			next_room=now_room.room_connect[0][temp];
		}
		else if(next_room==1){//Enter the House of Luck1
			int temp=now_room.room_HouseofLuck1();
			if(temp>=100){
				continue;
			}
			current_room=1;
			next_room=now_room.room_connect[1][temp];
		}
		else if(next_room==2){//Enter the Equipment house
			int temp=now_room.room_Equipmenthouse();
			if(temp>=100){
				continue;
			}
			current_room=2;
			next_room=now_room.room_connect[2][temp];
		}
		else if(next_room==3){//Enter the House of Tests
			int temp=now_room.room_HouseofTests();
			current_room=3;
			if(temp>=100){
				next_room=temp; 
				continue;
			}
			next_room=now_room.room_connect[3][temp];
		}
		else if(next_room==4){//Enter the Swamp House
			int temp=now_room.room_SwampHouse();
			if(temp>=100){
				continue;
			}
			current_room=4;
			next_room=now_room.room_connect[4][temp];
		}
		else if(next_room==5){//Enter the Swamp Jail
			int temp=now_room.room_Jail();
			if(temp>=100){
				continue;
			}
			current_room=5;
			next_room=now_room.room_connect[5][temp];
		}
		else if(next_room==6){//Enter the House of Luck2
			int temp=now_room.room_HouseofLuck2();
			if(temp>=100){
				continue;
			}
			current_room=6;
			next_room=now_room.room_connect[6][temp];
		}
		else if(next_room==7){//Enter the Sanatorium
			int temp=now_room.room_Sanatorium();
			if(temp>=100){
				continue;
			}
			current_room=7;
			next_room=now_room.room_connect[7][temp];
		}
		else if(next_room==8){//Enter the Little Monster's House
			int temp=now_room.room_LittleMonstersHouse();
			if(temp>=100){
				continue;
			}
			current_room=8;
			next_room=now_room.room_connect[8][temp];
		}
		else if(next_room==9){//Enter the Vixen House
			int temp=now_room.room_VixenHouse();
			if(temp>=100){
				continue;
			}
			current_room=9;
			next_room=now_room.room_connect[9][temp];
		}
		else if(next_room==10||next_room==100){
			if(now_state.if_princess==1){
				if(now_state.alive[1]==1){
				//If you take the princess back to the hall and the princess survives, the mission is successful
					cout<<"You succeed to save the princess"<<endl;
					cout<<"Congrulations!!!"<<endl;
				}
				else if(now_state.alive[1]==0){
				//If you take the princess back to the hall and the princess dead, the mission is failed
					cout<<"The princess has been dead, you come back alone......"<<endl;
				}
				break;
			}
			int temp=now_room.room_lobbyhouse();
			if(temp>=100){
				continue;
			}
			current_room=10;
			next_room=now_room.room_connect[10][temp];
		}
	}
	return 0;
}
void Room_build::build_room(){
	srand((unsigned)time(NULL));//Generate random numbers and set random princess rooms and monster rooms 
	now_state.room_monster=rand()%14;
	now_state.room_princess=rand()%14;
	while(now_state.room_monster==now_state.room_princess||now_state.room_princess==9||now_state.room_monster==9){
		now_state.room_princess=rand()%14;
	} 
	room_connection();//Connecting rooms
}
void Room_build::room_connection(){
	//Connect rooms according to the map
	room_connect[3][1]=room_connect[10][4]=0;
	room_connect[8][1]=room_connect[5][4]=room_connect[9][3]=1;
	room_connect[8][4]=room_connect[10][3]=room_connect[9][2]=2;
	room_connect[4][4]=room_connect[0][2]=3;
	room_connect[3][3]=room_connect[10][2]=4;
	room_connect[1][3]=room_connect[7][1]=5;
	room_connect[8][2]=6;
	room_connect[8][3]=room_connect[5][2]=7;
	room_connect[6][1]=room_connect[1][2]=room_connect[7][4]=room_connect[2][3]=8;
	room_connect[2][1]=room_connect[1][4]=9;
	room_connect[2][4]=room_connect[4][1]=room_connect[0][3]=10; 
	if(now_state.room_monster==0){//Connect the monster's room
		room_connect[3][4]=11;
	}
	else if(now_state.room_monster==1){
		room_connect[0][4]=11;
	}
	else if(now_state.room_monster==2){
		room_connect[0][1]=11;
	}
	else if(now_state.room_monster==3){
		room_connect[10][1]=room_connect[9][4]=11;
	}
	else if(now_state.room_monster==4){
		room_connect[9][1]=11;
	}
	else if(now_state.room_monster==5){
		room_connect[1][1]=11;
	}
	else if(now_state.room_monster==6){
		room_connect[5][1]=11;
	}
	else if(now_state.room_monster==7){
		room_connect[5][3]=11;
	}
	else if(now_state.room_monster==8){
		room_connect[7][3]=11;
	}
	else if(now_state.room_monster==9){
		room_connect[7][2]=room_connect[6][3]=11;
	}
	else if(now_state.room_monster==10){
		room_connect[6][2]=11;
	}
	else if(now_state.room_monster==11){
		room_connect[2][2]=room_connect[4][3]=11;
	}
	else if(now_state.room_monster==12){
		room_connect[4][2]=11;
	}
	else if(now_state.room_monster==13){
		room_connect[3][2]=11;
	}
	
	if(now_state.room_princess==0){//Connect the princess's room
		room_connect[3][4]=12;
		room_connect[12][3]=3;
	}
	else if(now_state.room_princess==1){
		room_connect[0][4]=12;
		room_connect[12][3]=0;
	}
	else if(now_state.room_princess==2){
		room_connect[0][1]=12;
		room_connect[12][2]=4;
	}
	else if(now_state.room_princess==3){
		room_connect[10][1]=room_connect[9][4]=12;
		room_connect[12][4]=4;
		room_connect[12][3]=9;
		room_connect[12][2]=10;
	}
	else if(now_state.room_princess==4){
		room_connect[9][1]=12;
		room_connect[12][2]=9;
	}
	else if(now_state.room_princess==5){
		room_connect[1][1]=12;
		room_connect[12][2]=1;
	}
	else if(now_state.room_princess==6){
		room_connect[5][1]=12;
		room_connect[12][2]=5;
	}
	else if(now_state.room_princess==7){
		room_connect[5][3]=12;
		room_connect[12][4]=5;
	}
	else if(now_state.room_princess==8){
		room_connect[7][3]=12;
		room_connect[12][4]=7;
	}
	else if(now_state.room_princess==9){
		room_connect[7][2]=room_connect[6][3]=12;
	}
	else if(now_state.room_princess==10){
		room_connect[6][2]=12;
		room_connect[12][1]=6;
	}
	else if(now_state.room_princess==11){
		room_connect[2][2]=room_connect[4][3]=12;
		room_connect[12][3]=6;
		room_connect[12][4]=4;
		room_connect[12][1]=2;
	}
	else if(now_state.room_princess==12){
		room_connect[4][2]=12;
		room_connect[12][1]=4;
	}
	else if(now_state.room_princess==13){
		room_connect[3][2]=12;
		room_connect[12][1]=3;
	}
}
void Room_build::castle_map(){//Generate castle map
	cout<<endl;
	//Print maps in different princess and monster room's situation
	if(now_state.room_monster==0&&now_state.room_princess==1){
		cout<<"monster's house\t"<<"prin's house\t"<<endl;
	}
	else if(now_state.room_princess==0&&now_state.room_monster==1){
		cout<<"prin's house\t"<<"monster's house\t"<<endl;
	}
	else if(now_state.room_monster==0){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==0){
		cout<<"prin's house\t"<<endl;
	}
	else if(now_state.room_monster==1){
		cout<<"\t\tmonster's house\t"<<endl;
	}
	else if(now_state.room_princess==1){
		cout<<"\t\tprin's house\t"<<endl;
	}
	if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
		if(now_state.room_monster==13){
			cout<<"monster's house\t";
		}
		else if(now_state.room_princess==13){
			cout<<"prin's house\t";
		}
		else{
			cout<<"\t\t";
		}
	}
	cout<<"House of Tests\t"<<"Gas Chamber\t";
	if(now_state.room_monster==2){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==2){
		cout<<"prin's house\t"<<endl;
	}
	else{
		cout<<endl;
	}
	if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
		if(now_state.room_monster==12){
			cout<<"monster's house\t";
		}
		else if(now_state.room_princess==12){
			cout<<"prin's house\t";
		}
		else{
			cout<<"\t\t";
		}
	}
	cout<<"Swamp House\t"<<"lobby_house\t";
	if(now_state.room_monster==3){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==3){
		cout<<"prin's house\t"<<endl;
	}
	else{
		cout<<endl;
	}
	if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
		cout<<"\t\t";
	}
	if(now_state.room_monster==11){
		cout<<"monster's house\t";
	}
	else if(now_state.room_princess==11){
		cout<<"prin's house\t";
	}
	else{
		cout<<"\t\t";
	}
	cout<<"Equipment House\t"<<"prin's house\t";
	if(now_state.room_monster==4){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==4){
		cout<<"prin's house\t"<<endl;
	}
	else{
		cout<<endl;
	}
	if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
		if(now_state.room_monster==10){
			cout<<"monster's house\t";
		}
		else if(now_state.room_princess==10){
			cout<<"prin's house\t";
		}
		else{
			cout<<"\t\t";
		}
	}
	cout<<"House of Luck2\t"<<"LMon's House\t"<<"House of Luck1\t";
	if(now_state.room_monster==5){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==5){
		cout<<"prin's house\t"<<endl;
	}
	else{
		cout<<endl;
	}
	if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
		cout<<"\t\t\t\t";
	}
	else{
		cout<<"\t\t";
	}
	cout<<"Sanatorium\t"<<"Jail_room\t";
	if(now_state.room_monster==6){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==6){
		cout<<"prin's house\t"<<endl;
	}
	else{
		cout<<endl;
	}
	if(now_state.room_monster==8||now_state.room_princess==7||now_state.room_monster==7||now_state.room_princess==8){
		cout<<"\t\t";
		if(now_state.room_monster==13||now_state.room_princess==13||now_state.room_monster==12||now_state.room_princess==12||now_state.room_monster==10||now_state.room_princess==10){
			cout<<"\t\t";
		}
	}
	if(now_state.room_monster==8&&now_state.room_princess==7){
		cout<<"monster's house\t"<<"prin's house\t"<<endl;
	}
	else if(now_state.room_princess==8&&now_state.room_monster==7){
		cout<<"prin's house\t"<<"monster's house\t"<<endl;
	}
	else if(now_state.room_monster==8){
		cout<<"monster's house\t"<<endl;
	}
	else if(now_state.room_princess==8){
		cout<<"prin's house\t"<<endl;
	}
	else if(now_state.room_monster==7){
		cout<<"\t\tmonster's house"<<endl;
	}
	else if(now_state.room_princess==7){
		cout<<"\t\tprin's house"<<endl;
	}
	cout<<"note :"<<endl;//Output map notes and hints
	cout<<"prin'house means princess'house;"<<endl;
	cout<<"LMon's House means Little Monster's House"<<endl;
	cout<<"Strangely enough,there are two princess'house"<<endl;
}
int Room_build::lobby_type(){
	//Output game start prompt
	cout<<"In an ancient legend, the princess was caught by a monster in a castle."<<endl;
	sleep(2);
	cout<<"A warrior sets out on the road to saving the princess."<<endl;
	sleep(2);
	cout<<"But the truth is often not as simple as it seems"<<endl;
	for(int i=0;i<5;i++){
		cout<<"------------------"<<endl; 
	}
	sleep(2);
	cout<<"Game starts!"<<endl;
	sleep(2);
	cout<<endl;
	cout<<"Basic rules and information:"<<endl;
	cout<<"Your initial health is 100, and if your health drops to 0 or you encounter the monster, the adventure starts"<<endl;
	sleep(2);
	cout<<endl;
	cout<<"Tips :Doing the same thing in different states often results differently"<<endl;
	 sleep(2);
	cout<<endl;
	cout<<"Now you are in the lobby of the castle."<<endl;
	sleep(2);
	if(now_state.room_monster==3||now_state.room_princess==3){//Output exit prompt
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		cout<<"There are 3 exits: west, north and south."<<endl;
		cout<<"Please enter your command(go west/go north/go south):"<<endl;
	}
	while(1){//Enter the command and select the next room number to return
		string command;
		getline(cin,command);
		if((now_state.room_monster==3||now_state.room_princess==3)&&command=="go east"){
			//Add random princess or monster rooms
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}	
}
void Room_build::room_monstertype(){//Build a monster room
	cout<<"You are now in the house where the monster is."<<endl;
	sleep(2);
	cout<<"You were torn apart by the power of the monster!!!"<<endl;
	sleep(2);
	cout<<"Game Over"<<endl;
}
int Room_build::room_princesstype(int last_room){//Build a princess room
	now_state.if_princess=1;//Set it to having found the princess
	cout<<"You are now in the house named princess"<<endl;
	sleep(2);
	if(now_state.have_travel[1]){//Determine if you've been in the princess's room
		cout<<"You have already been to this room."<<endl;
	}
	else{//If you come to the princess's room for the first time, you have a conversation with the princess
        cout<<"The princess whispered: "<<endl;
        sleep(2);
        cout<<"  No one has been here for a long time."<<endl;
        sleep(2);
        cout<<"You say: "<<endl;
        sleep(2);
        cout<<"  My Royal Highness, I have come to save you, come with me."<<endl;;
        sleep(2);
		cout<<"The princess say: "<<endl;
        sleep(2);
		cout<<"  Save? It's hard to save my heart"<<endl;
		sleep(2);
		cout<<"You say: "<<endl;
		sleep(2);
		cout<<"  Anyway, please come with me and leave now"<<endl;
		sleep(2);
		cout<<"You leave with the princess, but you find that the princess is in a state of extreme emotional instability"<<endl;
		cout<<endl;
		cout<<"You need to take the princess back to the lobby and leave."<<endl;
	}
	now_state.have_travel[1]=1;//Set to already been in the room
	sleep(2);
	cout<<"You returned to the previous room"<<endl;
	cout<<endl;
	return last_room;//Return to the previous room
}
int Room_build::room_Gaschamber(){//Build a Gaschamber
	cout<<"You are now in the Gas chamber."<<endl;
	sleep(2);
	if(now_state.equipment[0]==1){//If the mask is equipped, it will not be poisoned
		cout<<"Your mask fends off the gas"<<endl;
	}
	else{//If the mask isn't equipped, tou will be poisoning
		cout<<"Since you inhaled poisonous gas, you were poisoned"<<endl;
		cout<<"Each time you enter the next room, your health will be -10(The poisoning effect can be stacked)"<<endl;
		now_state.poisoning+=10;//Increased poisoning levels
	}
	sleep(2);//Enter the next room
	if((now_state.room_monster==1&&now_state.room_princess==2)||(now_state.room_monster==1&&now_state.room_princess==2)){
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		if(now_state.room_monster==1||now_state.room_princess==1){
			cout<<"There are 3 exits: west, north and south."<<endl;
			cout<<"Please enter your command(go west/go north/go south):"<<endl;
		}
		else if(now_state.room_monster==2||now_state.room_princess==2){
			cout<<"There are 3 exits: east, west and south."<<endl;
			cout<<"Please enter your command(go east/go west/go south):"<<endl;
		}
		else{
			cout<<"There are 2 exits: west, south."<<endl;
			cout<<"Please enter your command(go west/go south):"<<endl;
		}
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if((now_state.room_monster==2||now_state.room_princess==2)&&command=="go east"){
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if((now_state.room_monster==1||now_state.room_princess==1)&&command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_HouseofLuck1(){//Build House of Luck1
	cout<<"You are now in House of Luck1."<<endl;
	sleep(2);
	cout<<"You see the map of the castle and it will be a big help"<<endl;
	sleep(2);
	castle_map();//Show the map of castle
	cout<<endl;
	if(now_state.room_monster==5||now_state.room_princess==5){//Enter the next room
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		cout<<"There are 3 exits: west, north and south."<<endl;
		cout<<"Please enter your command(go west/go north/go south):"<<endl;
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if((now_state.room_monster==5||now_state.room_princess==5)&&command=="go east"){
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_Equipmenthouse(){//Build the equipment house
	cout<<"You are now in Equipment House."<<endl;
	sleep(2);
	cout<<"Now there are three pieces of equipment (role unknown) in the room."<<endl;
	cout<<"Oddly enough, they exude a human scent"<<endl;
	sleep(2);
	cout<<"Please select the sword/shield/mask/mirror,enter(sword/shield/mask/mirror)"<<endl;
	while(1){//Choose the equipment you wanted
		string command;
		getline(cin,command);
		if(command=="sword"&&now_state.equipment[1]==0){//Choose the sword
			if(now_state.equipment_number<=2){//Determine if you have the strength to hold it
				now_state.equipment[1]=1;
				now_state.equipment_number++;//Equip the sword and update the amount of equipment
				cout<<"Your get the sword scussessfully!"<<endl;
				break;
			}
			else{
				cout<<"You already have two pieces of equipment, and you don't have the strength to take the third"<<endl;
			}
		}
		if(command=="shield"&&now_state.equipment[2]==0){//Choose the shield
			if(now_state.equipment_number<=2){//Determine if you have the strength to hold it
				now_state.equipment[2]=1;
				now_state.equipment_number++;//Equip the shield and update the amount of equipment
				cout<<"Your get the shield scussessfully!"<<endl;
				break;
			}
			else{
				cout<<"You already have two pieces of equipment, and you don't have the strength to take the third"<<endl;
			}
		}
		if(command=="mask"&&now_state.equipment[0]==0){//Choose the mask
			if(now_state.equipment_number<=2){//Determine if you have the strength to hold it
				now_state.equipment[0]=1;
				now_state.equipment_number++;//Equip the mask and update the amount of equipment
				cout<<"Your get the mask scussessfully!"<<endl;
				break;
			}
			else{
				cout<<"You already have two pieces of equipment, and you don't have the strength to take the third"<<endl;
			}
		}
		if(command=="mirror"&&now_state.equipment[3]==0){//Choose the mirror
			if(now_state.equipment_number<=2){//Determine if you have the strength to hold it
				now_state.equipment[3]=1;
				now_state.equipment_number++;//Equip the mirror and update the amount of equipment
				cout<<"Your get the mirror scussessfully!"<<endl;
				break;
			}
			else{
				cout<<"You already have two pieces of equipment, and you don't have the strength to take the third"<<endl;
			}
		}
		cout<<"The command was entered incorrectly,pick failed"<<endl;
		break;
	}
	if(now_state.room_monster==11||now_state.room_princess==11){//Enter the next room
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		cout<<"There are 3 exits: east, north and south."<<endl;
		cout<<"Please enter your command(go east/go north/go south):"<<endl;
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		if((now_state.room_monster==11||now_state.room_princess==11)&&command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_HouseofTests(){//Build the room House of Tests
	cout<<"You are now in House of Tests."<<endl;
	sleep(2);
	cout<<"Please enter the name of the castle and if you enter it correctly, you will go directly to the princess's room or lobby"<<endl;
	while(1){//Input your answer
		string command;
		getline(cin,command);
		if(command=="pain"&&now_state.if_princess==1){//Determine whether the answer you entered is correct
			cout<<"Enter correctly!!"<<endl;
			return 100;//Return to the lobby if you have taken the princess with you
		} 
		else if(command=="pain"&&now_state.if_princess==0){
			cout<<"Enter correctly!!"<<endl;
			return 200;//If you don't have the princess with you, go to the princess's room
		} 
		else {
			cout<<"Enter mistakenly!!"<<endl;
			break;
		}
	}
	sleep(2);//Enter the next room
	if((now_state.room_monster==0&&now_state.room_princess==13)||(now_state.room_monster==13&&now_state.room_princess==0)){
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		if(now_state.room_monster==0||now_state.room_princess==0){
			cout<<"There are 3 exits: east, north and south."<<endl;
			cout<<"Please enter your command(go east/go north/go south):"<<endl;
		}
		else if(now_state.room_monster==13||now_state.room_princess==13){
			cout<<"There are 3 exits: east, west and south."<<endl;
			cout<<"Please enter your command(go east/go west/go south):"<<endl;
		}
		else{
			cout<<"There are 2 exits: east, south."<<endl;
			cout<<"Please enter your command(go east/go south):"<<endl;
		}
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		if((now_state.room_monster==13||now_state.room_princess==13)&&command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if((now_state.room_monster==0||now_state.room_princess==0)&&command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_SwampHouse(){//Build the Swamp House
	cout<<"You are now in Swamp House."<<endl;
	sleep(2);
	cout<<"You're stuck in a swamp and it takes 20 seconds to break free"<<endl;
	for(int i=0;i<10;i++){//Pause ten seconds
		cout<<i+1<<endl;
		sleep(1);
	}
	if((now_state.room_monster==11&&now_state.room_princess==12)||(now_state.room_monster==12&&now_state.room_princess==11)){
		//Enter the next room
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		if(now_state.room_monster==11||now_state.room_princess==11){
			cout<<"There are 3 exits: east, north and south."<<endl;
			cout<<"Please enter your command(go east/go north/go south):"<<endl;
		}
		else if(now_state.room_monster==12||now_state.room_princess==12){
			cout<<"There are 3 exits: east, west and north."<<endl;
			cout<<"Please enter your command(go east/go west/go north):"<<endl;
		}
		else{
			cout<<"There are 2 exits: east, north."<<endl;
			cout<<"Please enter your command(go east/go north):"<<endl;
		}
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		if((now_state.room_monster==12||now_state.room_princess==12)&&command=="go west"){
			return 2;
		}
		if((now_state.room_monster==11||now_state.room_princess==11)&&command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_Jail(){//Build the jail
	cout<<"You are now in jail."<<endl;
	sleep(2);
	cout<<"There are many beasts imprisoned here, but there are no human inside"<<endl;
	sleep(2);
	cout<<"One of the prisoners attacked you"<<endl;
	sleep(2);
	if(now_state.equipment[2]==1){//If equipped with a shield, take no damage
		cout<<"Your shield withstood this attack"<<endl;
	}
	else{//If not equipped with shield, health decreases by 30
		cout<<"You were attacked by a prisoner"<<endl;
		now_state.health-=30;
		cout<<"Your current health: "<<now_state.health<<endl;
	}
	sleep(2);//Enter the next room
	if((now_state.room_monster==6&&now_state.room_princess==7)||(now_state.room_monster==7&&now_state.room_princess==6)){
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		if(now_state.room_monster==6||now_state.room_princess==6){
			cout<<"There are 3 exits: east, west and north."<<endl;
			cout<<"Please enter your command(go east/go west/go north):"<<endl;
		}
		else if(now_state.room_monster==7||now_state.room_princess==7){
			cout<<"There are 3 exits: west, north and south."<<endl;
			cout<<"Please enter your command(go west/go north/go south):"<<endl;
		}
		else{
			cout<<"There are 2 exits: west, north."<<endl;
			cout<<"Please enter your command(go west/go north):"<<endl;
		}
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if((now_state.room_monster==6||now_state.room_princess==6)&&command=="go east"){
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if((now_state.room_monster==7||now_state.room_princess==7)&&command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_HouseofLuck2(){//Build the House of Luck2
	cout<<"You are now in House of Luck2."<<endl;
	sleep(2);
	cout<<"You can play dice once with the caretaker, and if you have more points, he will tell you some clues about the castle"<<endl;
	sleep(2);
	srand((unsigned)time(NULL));//Generate random number seeds
	int player_num=rand()%7;
	int npc_num=rand()%7;//Generate a random number of dice
	cout<<"your points is "<<player_num<<endl;
	cout<<"the caretaker's points is "<<npc_num<<endl;
	if(player_num>npc_num){//If your count is high, trigger the cue
		cout<<"You win!"<<endl;
		sleep(2);
		cout<<"He told you slowly :"<<endl;
		sleep(2);
		cout<<"  The princess often wept towards another room. "<<endl;
		sleep(2);
		cout<<"  After the princess was captured by the monster, the monster never came out of his house again."<<endl;
		sleep(2);
		cout<<"  It is a paindul castle with a name a kind of mood."<<endl;
		sleep(2);
		cout<<"  But the name was changed recently."<<endl;
	}
	else{
		cout<<"You failed!!"<<endl;
	}
	sleep(2);//Enter the next room
	if((now_state.room_monster==10&&now_state.room_princess==11)||(now_state.room_monster==11&&now_state.room_princess==10)){
		cout<<"There are 3 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north):"<<endl;
	}
	else{
		if(now_state.room_monster==10||now_state.room_princess==10){
			cout<<"There are 2 exits: east and west."<<endl;
			cout<<"Please enter your command(go east/go west):"<<endl;
		}
		else if(now_state.room_monster==11||now_state.room_princess==11){
			cout<<"There are 2 exits: east and north."<<endl;
			cout<<"Please enter your command(go east/go north):"<<endl;
		}
		else{
			cout<<"There are 1 exits: east."<<endl;
			cout<<"Please enter your command(go east):"<<endl;
		}
	}
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		if((now_state.room_monster==10||now_state.room_princess==10)&&command=="go west"){
			return 2;
		}
		if((now_state.room_monster==11||now_state.room_princess==11)&&command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}

int Room_build::room_LittleMonstersHouse(){//Build the Little Monster's House
	cout<<"You are now in Little Monster's House."<<endl;
	sleep(2);
	if(now_state.if_princess==1&&now_state.alive[0]==0&&now_state.equipment[2]==0){
		//If you take the princess to the little monster's room and the little monster is dead, and you don't have a shield
		cout<<"The princess showed a look of great pain."<<endl;
		sleep(2);
		cout<<"She suddenly pulled out the long sword at your waist and thrust it at you"<<endl;
		cout<<"Your current health:"<<now_state.health-100<<endl;
		now_state.health-=100;//Princess kills you. Health -100
		sleep(2);
		cout<<"Your consciousness fades and you hear the princess say with losing her's nerve: I'm sorry."<<endl;
		return 0;
	}
	else if(now_state.if_princess==1&&now_state.alive[0]==0&&now_state.equipment[2]==1){
		//If you take the princess to the little monster's room and the little monster is dead, and you have a shield
		now_state.princess_thought=0;//The princess's mood goes to 0
		cout<<"The princess showed a look of great pain."<<endl;
		sleep(2);
		cout<<"She suddenly pulled out the long sword at your waist and thrust it at you"<<endl;
		sleep(2);
		cout<<"Your shield is blocking the attack."<<endl;
		sleep(3);
		cout<<"In your confusion, the princess calmed down and said to you: "<<endl;
		cout<<"  Just go away and leave me alone here."<<endl;
		sleep(2);
		cout<<"You have to stumble to the side exit"<<endl;
		now_state.if_princess=0;//The princess is gone from you
		return 200;//Returns the result
	}
	else if(now_state.if_princess==1&&now_state.alive[0]==1&&now_state.equipment[1]==1){
		//If you take the princess to the little monster's room and the little monster is alive, and you have a shield
		cout<<"The princess looked at the little monster with a strange expression."<<endl;
		sleep(2);
		cout<<"Please choose whether to kill the Little Monster"<<endl;
		sleep(2);
		cout<<"Please enter your command(yes/no)"<<endl;
		while(1){//Choose whether or not to kill the little monster
			string command;
			getline(cin,command);
			if(command=="yes"){
				cout<<"The princess stops you and says :Let's go right now."<<endl;
				sleep(2);
				cout<<"Please choose whether to kill the Little Monster still"<<endl;
				sleep(2);
				cout<<"Please enter your command(yes/no)"<<endl;
				while(1){//Choose whether or not to kill the little monster still
					string tcommand;
					getline(cin,tcommand);
					if(command=="yes"){
						cout<<"You kill it successfully"<<endl;
						cout<<"The princess showed a look of great pain."<<endl;
						sleep(2);
						cout<<"She plunged her body into the sword"<<endl;
						now_state.alive[0]=0;//Set the little monster to die
						sleep(2);
						cout<<"You don't know why, You'll have to take the princess to be treated"<<endl;
						break;
					}
					else if(command=="no"){
						break;
					}
					else{
						cout<<"The command was entered incorrectly, please re-enter it"<<endl;
					}
				}
				break;
			}
			else if(command=="no"){
				break;
			}
			else{
				cout<<"The command was entered incorrectly, please re-enter it"<<endl;
			}
		}
	}
	
	else if(now_state.equipment[1]==1){//If you have a sword
		if(now_state.alive[0]==1){//Choose whether or not to kill the little monster 
			cout<<"You have a sword now"<<endl;
			cout<<"Please choose whether to kill the Little Monster"<<endl;
			sleep(2);
			cout<<"Please enter your command(yes/no)"<<endl;
			while(1){
				string command;
				getline(cin,command);
				if(command=="yes"){
					cout<<"You kill it successfully"<<endl;
					sleep(2);
					if(now_state.equipment[2]==0){//If you don't have a shield, Health is -40
						cout<<"You were injured in battle and currently have health:"<<endl;
						cout<<"Your current health:"<<now_state.health-40<<endl;
						now_state.health-=40;
					}
					now_state.alive[0]=0;//Set the little monster to die
					break;
				}
				else if(command=="no"){
					break;
				}
			}
		}
		else{
			cout<<"The little monster has been killed by you, and its blood smells of human blood"<<endl;
		} 
		
	}
	cout<<"You run away quickly"<<endl;
	sleep(2);//Enter the next room
	cout<<"There are 4 exits: east, west, north and south."<<endl;
	cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		else if(command=="go west"){
			return 2;
		}
		else if(command=="go south"){
			return 3;
		}
		else if(command=="go north"){
			return 4;
		}
		else{
			cout<<"The command was entered incorrectly, please re-enter it"<<endl;
		}
		
	}
	cout<<endl;
}
int Room_build::room_VixenHouse(){//Build the fake princess's House
	cout<<"You are now in the house named princess"<<endl;
	sleep(2);
	if(now_state.equipment[3]==1&&now_state.if_monster==1){//If you have a mirror, show the monster as it really is
		cout<<"You accidentally discover that the princess in the mirror is a monster when you're ready to unshackle it"<<endl;
		sleep(2);
		cout<<"You realize this is a princess in the form of a monster"<<endl;
		sleep(2);
		cout<<"When you are ready to leave, vaguely hear the monster muttering: pain"<<endl;
	}
	else if(now_state.if_monster==1){//If you don't have a mirror, you let the monster get away
		cout<<"When you unshackled the princess, she suddenly disappears."<<endl;
		sleep(2);
		cout<<"You don't know what's going on, so you keep going"<<endl;
	}
	else{//Second entry, get some clues
		cout<<"The second time you came here, and nothing here."<<endl;
		sleep(2);
		cout<<"But you find a mask made of monster's skin that resembles a princess."<<endl;
		if(now_state.if_princess){
			cout<<"The princess looked at the mask with a complicated expression with fear, disgust, pain......"<<endl;
		}
	}
	sleep(2);
	now_state.if_monster=0;
	if((now_state.room_monster==3&&now_state.room_princess==4)||(now_state.room_monster==4&&now_state.room_princess==3)){
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		if(now_state.room_monster==4||now_state.room_princess==4){
			cout<<"There are 3 exits: east, west and south."<<endl;
			cout<<"Please enter your command(go east/go west/go south):"<<endl;
		}
		else if(now_state.room_monster==3||now_state.room_princess==3){
			cout<<"There are 3 exits: west, north and south."<<endl;
			cout<<"Please enter your command(go west/go north/go south):"<<endl;
		}
		else{
			cout<<"There are 2 exits: west, south."<<endl;
			cout<<"Please enter your command(go west/go south):"<<endl;
		}
	}
	now_state.have_travel[10]=1;//Settings have entered the room
	while(1){//Input the command and choose the next room 
		string command;
		getline(cin,command);
		if((now_state.room_monster==4||now_state.room_princess==4)&&command=="go east"){
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if((now_state.room_monster==3||now_state.room_princess==3)&&command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}
}
int Room_build::room_Sanatorium(){//Build the Sanatorium
	cout<<"You are now in Sanatorium."<<endl;
	sleep(2);
	if(now_state.if_princess&&now_state.alive[1]==0){//If you carry the dying princess, restore her
		now_state.alive[1]=1;
		cout<<"The princess was treated"<<endl;
		sleep(2);
	}
	cout<<"You was treated"<<endl;
	now_state.health=100;//Restores all health for you
	cout<<"current health :"<<now_state.health<<endl;
	sleep(2);//Enter the next room
	if(now_state.room_monster==8||now_state.room_princess==8){
		cout<<"There are 3 exits: east, north and south."<<endl;
		cout<<"Please enter your command(go east/go north/go south):"<<endl;
	}
	else{
		cout<<"There are 2 exits: east and north."<<endl;
		cout<<"Please enter your command(go east/go north):"<<endl;
	}
	while(1){//Input the command and choose the next room
		string command;
		getline(cin,command);
		if(command=="go east"){
			return 1;
		}
		if((now_state.room_monster==8||now_state.room_princess==8)&&command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}	
}
int Room_build::room_lobbyhouse(){//BUild the lobby
	cout<<"You are coing back to the Lobby."<<endl;
	cout<<"But you haven't found the princess yet, keep going!"<<endl;
	sleep(2);//Enter the next room
	if(now_state.room_monster==3||now_state.room_princess==3){
		cout<<"There are 4 exits: east, west, north and south."<<endl;
		cout<<"Please enter your command(go east/go west/go north/go south):"<<endl;
	}
	else{
		cout<<"There are 3 exits: west, north and south."<<endl;
		cout<<"Please enter your command(go west/go north/go south):"<<endl;
	}
	while(1){//Input the command and choose the next room
		string command;
		getline(cin,command);
		if((now_state.room_monster==3||now_state.room_princess==3)&&command=="go east"){
			return 1;
		}
		if(command=="go west"){
			return 2;
		}
		if(command=="go south"){
			return 3;
		}
		if(command=="go north"){
			return 4;
		}
		cout<<"The command was entered incorrectly, please re-enter it"<<endl;
	}		
}
    

