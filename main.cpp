#include <bits/stdc++.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include "picture.h"

#pragma GCC diagnostic error "-Wuninitialized"

using namespace std;

typedef long long ll;

#define rep( bg, i, num ) for(ll (i)= (bg); (i) < (num); (i)++)
#define repd( num, i, ed ) for(ll (i)= (num)-1; (i) >= ed; (i)--)




const bool useThread= true;
int funcIn= 1;
struct PrintChar{
	int x, y;
	char c;
	PrintChar( int x2, int y2, int c2){
		x= x2, y= y2, c= c2;
	}
};

// A threadsafe-queue.
class SafeQueue
{
public:
	SafeQueue(void): q(), m(), c()
	{}

	~SafeQueue(void)
	{}

 	// Add an element to the queue.
 	void push(PrintChar t)
	{
		std::lock_guard<std::mutex> lock(m);
 		q.push(t);
 		c.notify_one();
	}

	// Get the "front"-element.
 	// If the queue is empty, wait till a element is avaiable.
 	PrintChar pop(void)
 	{
		std::unique_lock<std::mutex> lock(m);
		while(q.empty())
		{
			// release lock as long as the wait and reaquire it afterwards.
			c.wait(lock);
		}
		PrintChar val= q.front();
		q.pop();
		return val;
	}

	bool empty(){
		return q.empty();
	}
	
private:
	std::queue<PrintChar> q;
	mutable std::mutex m;
	std::condition_variable c;
};
SafeQueue qThread;

void gotoxy(int y, int x){		/*Phan nay copy tren mang*/
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x,y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}

void printChar(PrintChar p){
    gotoxy(p.x,p.y);
    printf("%c",p.c);
}

















void print(){
	while( funcIn ){
//		cout << !qThread.empty();
		while( !qThread.empty() ){
			printChar( qThread.pop() );
		}
	}
}
char before(char c, int i= 1){
	if( i == 0 || c < '!' || c > '~' )
		return c;
	return ( ( c-'!' + 94-(i%94) )%94 + '!' );
}
char wordBefore(char c, int i= 1){
	if( i == 0 )
		return c;
	bool upper= ( c <= 'Z' && c >= 'A' );
	if( upper )
		c+= 32;
	else if( !( c <= 'z' && c >= 'a' ) )	//	c la 1 ki tu dac biet
		return c;
	// now c is lower
	return ( ( c-'a' + 26-(i%26) )%26 + 'a' - upper*32 );
}
char sc[]= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
void strDown(string &s, int &l, int *a){
	int i= 0, j= 1;
	while(j){
		if( a[i] == 0 )
			a[i]= 10;
		else
			j= 0;
		a[i]-= 1;
		s[s.length()-1 -i]= sc[ a[i] ];
		i++;
	}
	if( l > 1 && a[l-1] == 0 )
		s[ s.length()-(l--) ]= ' ';
}
void strUp(string &s, int &l, int *a){
	int i= 0, j= 1;
	while(j){
		a[i]+= 1;
		if( a[i] == 10 )
			a[i]= 0;
		else
			j= 0;
		s[s.length()-1 -i]= sc[ a[i] ];
		i++;
	}
}
void fadeStringIn(string s, int x= 0, int y= 0, int t= 1, int delay= 50){
	string s2;
	ll l= s.length();
	rep(1,i,l+t){
		if( i < l+t )	Sleep(delay);
		s2= "";
		rep(max(0ll, i-t),j,min( i, l))
			s2+= before(s[j], max( t-i+j, 0ll) );
		rep(0,j,s2.length())
			qThread.push( PrintChar( x, y+max(0ll, i-t) +j, s2[j] ) );
	}
}
void fadeStringOut(string s, int x= 0, int y= 0, int t= 1, int delay= 50){
	string s2;
	ll l= s.length();
	rep(1,i,l+t+1){
		if( i < l+t )	Sleep(delay);
		s2= "";
		rep(max(0ll, i-t),j,l)
			s2+= before(s[j], max( i-j, 0ll) );
		rep(0,j,s2.length())
			qThread.push( PrintChar( x, y+max(0ll, i-t) +j, s2[j] ) );
		if( i-t > 0 )
			qThread.push( PrintChar( x, y+max(0ll, i-t) -1, (char)32 ) );
	}
}
void countDown(int from, int to= 0, int x= 0, int y= 0, int delay= 1000){
	string s= "";
	int sp= from+1, l= 0, a[18];
	while( sp > 0 ){
		a[l]= sp%10;
		sp/= 10;
		l++;
		s+= " ";
	}
	repd(l,i,0)
		s[l-i-1]= a[i]+'0';
	repd(from,i,to){
		strDown(s, l, a);
		fadeStringIn( s, x, y, 1, 0 );
		Sleep(delay);
	}
	strDown(s, l, a);
	fadeStringIn( s, x, y, 1, 0 );
}
void countUp(int from, int to= 0, int x= 0, int y= 0, int dNear= 1000, int dFar= 200, int range= 5){
	string s= "";
	int sp= to, sp2= from, l= 0, a[18];
	while( sp > 0 ){
		a[l]= sp2%10;
		sp2/= 10;
		sp/= 10;
		l++;
		s+= " ";
	}
	repd(l,i,0)
		s[l-i-1]= a[i]+'0';
	rep(from,i,to){
		strUp(s, l, a);
		fadeStringIn( s, x, y, 1, 0 );
		if( i-from <= range || to-i <= range )
			Sleep(dNear);
		else
			Sleep(dFar);
	}
}
/********************************************************************************************************/
void wallTR(int h, int w, string c){
	rep(0,i,w)
		fadeStringIn( c,0,i,1,10);
	rep(0,i,h)
		fadeStringIn( c,i,w-1,1,10);
}
void wallLB(int h, int w, string c){
	rep(0,i,h)
		fadeStringIn( c,i,0,1,10);
	rep(0,i,w)
		fadeStringIn( c,h-1,i,1,10);
}
void makeWall(int h, int w, string c= "#"){
	thread th1(wallTR, h, w, c);
	thread th2(wallLB, h, w, c);

	th1.join();
	th2.join();
	fadeStringIn( c,0,0,1,0);
}
/*********************************************************************************************************/
void makeCake(int delay= 50){
	rep(0,i,3){
		fadeStringIn(cakeBott[2-i], 27-i, 4, 1, 0);
		Sleep(delay);
	}
	rep(0,i,5){
		fadeStringIn(cakeBody[3  ], 24-i, 4, 1, 0);
		Sleep(delay);
	}
	rep(0,i,3){
		fadeStringIn(cakeBody[2-i], 19-i, 4, 1, 0);
		Sleep(delay);
	}
	rep(0,i,3){
		fadeStringIn(cakeHead[2-i], 16-i, 4, 1, 0);
		Sleep(delay);
	}
}
/************************************************************************************************************/
void writeHappy(){
	thread myThreads[5];
	rep(0,i,2)
		myThreads[i]= thread( fadeStringIn, wordFront[i], 21+i, 7, 1, 100 );
	rep(0,i,2)
		myThreads[i].join();
}
void writeBirthday(){
	thread myThreads[5];
	rep(2,i,5)
		myThreads[i]= thread( fadeStringIn, wordFront[i], 21+i, 7, 1, 100 );
	rep(2,i,5)
		myThreads[i].join();
}
void writeOnCake(){
	writeHappy();
	writeBirthday();
}
/**************************************************************************************************************/
int firelightX[3][3]= {{0,1,2},{0,1,2},{0,0,2}};
int firelightY[3][3]= {{2,0,2},{1,2,0},{0,2,0}};
class Candle{
private:
	bool fire;
	int x, y, firelight= 0, delay, leng;
	static int numCandle;
	void makeCandleTop(string c= ".", int delay= 0){
		if( fire && c == "." )
			c= strFire;
		fadeStringIn( c, x, y, 1, delay);
	}
	void makeCandleBody(int delayShow= 0, string c= candle[0]){
		rep(1,i,leng)
			fadeStringIn( c, x+i, y, 1, delayShow);
	}
public:
	Candle(){}
	Candle(int x2, int y2, int leng2= 7, bool _fire= false, int _delay= 50){
		fire= _fire;
		delay= _delay;
		x= x2;
		y= y2;
		leng= leng2;
		numCandle++;
	}
	void makeCandle(int delayShow= 0){
		makeCandleTop();
		makeCandleBody(delayShow);
	}
	void moveCandle(int x2, int y2){
		makeCandleTop(" ");
		makeCandleBody(0," ");
		x= x2;
		y= y2;
		makeCandle();
	}
	void lightUp(){
		fire= true;
		makeCandleTop();
	}
	void lightCandle(){
		rep(0,i,3)
			qThread.push( PrintChar( firelightX[firelight][i]+x-1, firelightY[firelight][i]+y-1, (char)32 ) );
		firelight= (firelight+1)%3;
		rep(0,i,3)
			qThread.push( PrintChar( firelightX[firelight][i]+x-1, firelightY[firelight][i]+y-1,
			light[firelightX[firelight][i]][firelightY[firelight][i]] ) );
		Sleep(delay);
	}
	int getX(){
		return x;
	}
	int getY(){
		return y;
	}
};
int Candle::numCandle = 0;
void showCandle( Candle c){
	c.makeCandle(200);
}
void movePosision( Candle *c, int step, int delay= 200){
	rep(0,i,step){
		c->moveCandle(c->getX()+1, c->getY());
		Sleep(delay);
	}
}
Candle c[4];
void getCandle(int x, int y){
	c[0]= Candle(2,7);
	c[1]= Candle(1,18);
	c[2]= Candle(1,29);
	c[3]= Candle(2,40);
	
	thread myThreads[5];
	rep(0,i,4){
		Candle *cand= &c[i];
		myThreads[i]= thread( showCandle, *cand );
	}
	rep(0,i,4)
		myThreads[i].join();
}
void plugCandle(){
	thread myThreads[5];
	rep(0,i,4){
		Candle *cand= &c[i];
		myThreads[i]= thread( movePosision, cand, 8, 200 );
	}
	Sleep(500);
	rep(0,i,4)
		myThreads[i].join();
}
void fire(){
	while( funcIn ){
		rep(0,i,4)
			c[i].lightCandle();
	}
}
/*********************************************************************************************************/





void t1(){
	fadeStringIn( "Do you like some candles?", 16, 12, 50);
	Sleep(3000);
	fadeStringOut( "Do you like some candles?", 16, 12, 30);
}
void t2(){
	fadeStringIn( "Here you are, look up!", 18, 13, 50);
	Sleep(2000);
	fadeStringOut( "Here you are, look up!", 18, 13, 20);
}
void t3_1(){
	rep(0,i,6){
		fadeStringIn( "Trang ", 16, 30-i, 1, 10);		
		Sleep(250);
	}
}
void t3_2(){
	fadeStringIn( "Le Thi ", 16, 18, 30);
}
void t3(){
	fadeStringIn( "Happy birthday to you", 16, 12, 50);
	Sleep(1500);
	fadeStringIn( "Trang", 16, 30, 30);
	Sleep(5000);
	fadeStringOut( "Happy birthday to ", 16, 12, 50);
	thread th3_1( t3_1 );
	thread th3_2( t3_2 );
	th3_1.join();
	th3_2.join();
}
void t4(){
	fadeStringIn( "00 - 00 - 0000", 18, 17, 1);
	countUp(0,16,18,17,200,50);
	countUp(0,3,18,23,500);
	countUp(0,1997,18,27,100,1,20);
	Sleep(2000);
	countUp(1997,2018,18,27,100,1,20);	
}
void runThread1(){
	thread th1(t1);
	Sleep(3000);
	thread th2(t2);
	Sleep(4000);
	getCandle(1, 2);
	Sleep(2000);
	thread th3(t3);
	th1.join();
	plugCandle();
	th2.join();
	thread th4(t4);
	th3.join();
	th4.join();
}
string ss[10];
void makePoem(){
	ss[0]= "Chuc mung em gai sinh nhat!";
	ss[1]= "Em bay gio da, trang tuan doi muoi";
	ss[2]= "Tuoi va xinh xan nhu hoa";
	ss[3]= "Them mot tuoi moi, lai cang dep hon";
	
	ss[4]= "Tuoi cuoi, vui ve, on hoa";
	ss[5]= "Them phan dam tham se la tot hon";
	ss[6]= "Nam nay em da khon roi";
	ss[7]= "Anh mung em se ngoi ngoi sac xuan!";
}
void showPoem(){
	thread myThreads[10];
	rep(0,i,4){
		string ssp= ss[i];
		myThreads[i]= thread( fadeStringIn, ss[i], 2+2*i, 60-(i%2)*5, 30, 50 );
		Sleep(2000);
	}
	Sleep(1000);
	rep(4,i,8){
		myThreads[i]= thread( fadeStringIn, ss[i], 4+2*i, 60-(i%2)*5, 30, 50 );
		Sleep(2000);
	}
	rep(0,i,8)
		myThreads[i].join();
}
void runThread2(){
	rep(0,i,4)
		c[i].lightUp();
	makePoem();
	showPoem();
}

void runNormal(){
	fadeStringIn( "I love you babe!", 5, 7, 10);
	fadeStringIn( "And you are everything to me!", 7, 3, 10);
}

void showName(){
	fadeStringIn( "Product by :", 21, 73, 30);
	thread myThreads[8];
	rep(0,i,7)
		myThreads[i]= thread( fadeStringIn, name[i], 20+i, 90, 1, 100 );
	rep(0,i,7)
		myThreads[i].join();
}



void test(){
	fadeStringOut("Oh no! I just wanna be with you!", 0, 0, 20, 100);
}

string s;
const int SO_THREAD= 2;
int main(){
	initPicture();
	thread printThread( print );

	try{
		system("start HPBD.mp3");
	}
	catch(exception e){
	}
	
	fadeStringIn( "                                ", 0,0,1,0);
	fadeStringIn( "Are you ready?", 0, 50 );
	getchar();
	countDown(5,0,0,66);
	
	makeWall(30, 120);
	makeCake();
	writeOnCake();
	
	runThread1();
	thread thFire( fire );
	runThread2();
	showName();
	
	
	thFire.join();
	printThread.join();
	funcIn--;
	return 0;
}

