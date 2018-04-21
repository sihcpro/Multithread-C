#include <bits/stdc++.h>
#include <string>
using namespace std;

string cakeHead[5], cakeBott[5], cakeBody[5], wordFront[5], candle[5], strFire, light[3], name[8];
void initPicture(){
	cakeHead[0]=  "            ________________            ";
	cakeHead[1]=  "     _,-*''`                `''*-,_     ";
	cakeHead[2]=  " _-*'                              '*-_ ";
	cakeBody[0]=  "((                                    ))";
	cakeBody[1]=  "||-_                                _-||";
	cakeBody[2]=  "||  '*--,,,__________________,,,--*'  ||";
	cakeBody[3]=  "||                                    ||";
	cakeBott[0]=  "(|                                    |)";
	cakeBott[1]=  " `-_                                _-` ";
	cakeBott[2]=  "    '*--,,,__________________,,,--*'    ";
	
	
	wordFront[0]= "      ||_|  //\\  |[) |[) \\\\/";
	wordFront[1]= "      || | //--\\ ||  ||  //"; wordFront[1][13]= wordFront[1][14]= (char)196;
	wordFront[2]= "           ____";
	wordFront[3]= "|[) || |[)  ||  ||_| ||\\  //\\  \\\\/";
	wordFront[4]= "|[) || ||\\  ||  || | ||/ //--\\ //";
	
	candle[0]=    " "; candle[0][0]= (char)186;
	strFire  =    " "; strFire[0]= (char)168;
	
	light[0] =    "\\|/";
	light[1] =    " O  "; light[1][0]= light[1][2]= (char)196;
	light[2] =    "/|\\";
	
	name[0]  =    " ___                 ___ ";
	name[1]  =    "// \\\\  ||  ||   ||  // \\\\";
	name[2]  =    "\\\\     ||  ||   ||  ||   ";
	name[3]  =    " \\\\    ||  ||===||  ||   ";
	name[4]  =    "  \\\\   ||  ||   ||  ||   ";
	name[5]  =    "   ||  ||  ||   ||  ||   ";
	name[6]  =    "\\\\_//  ||  ||   ||  \\\\_//";
}




