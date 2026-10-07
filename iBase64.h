#pragma once

#ifndef _BASE64_H
#define _BASE64_H



template <typename intType> bool iBase64CharIntValue(const char c,intType &Value) 
{
switch(c)
{
case 'A':{Value=0;return true;}
case 'B':{Value=1;return true;}
case 'C':{Value=2;return true;}
case 'D':{Value=3;return true;}
case 'E':{Value=4;return true;}
case 'F':{Value=5;return true;}
case 'G':{Value=6;return true;}
case 'H':{Value=7;return true;}
case 'I':{Value=8;return true;}
case 'J':{Value=9;return true;}
case 'K':{Value=10;return true;}
case 'L':{Value=11;return true;}
case 'M':{Value=12;return true;}
case 'N':{Value=13;return true;}
case 'O':{Value=14;return true;}
case 'P':{Value=15;return true;}
case 'Q':{Value=16;return true;}
case 'R':{Value=17;return true;}
case 'S':{Value=18;return true;}
case 'T':{Value=19;return true;}
case 'U':{Value=20;return true;}
case 'V':{Value=21;return true;}
case 'W':{Value=22;return true;}
case 'X':{Value=23;return true;}
case 'Y':{Value=24;return true;}
case 'Z':{Value=25;return true;}
case 'a':{Value=26;return true;}
case 'b':{Value=27;return true;}
case 'c':{Value=28;return true;}
case 'd':{Value=29;return true;}
case 'e':{Value=30;return true;}
case 'f':{Value=31;return true;}
case 'g':{Value=32;return true;}
case 'h':{Value=33;return true;}
case 'i':{Value=34;return true;}
case 'j':{Value=35;return true;}
case 'k':{Value=36;return true;}
case 'l':{Value=37;return true;}
case 'm':{Value=38;return true;}
case 'n':{Value=39;return true;}
case 'o':{Value=40;return true;}
case 'p':{Value=41;return true;}
case 'q':{Value=42;return true;}
case 'r':{Value=43;return true;}
case 's':{Value=44;return true;}
case 't':{Value=45;return true;}
case 'u':{Value=46;return true;}
case 'v':{Value=47;return true;}
case 'w':{Value=48;return true;}
case 'x':{Value=49;return true;}
case 'y':{Value=50;return true;}
case 'z':{Value=51;return true;}
case '0':{Value=52;return true;}
case '1':{Value=53;return true;}
case '2':{Value=54;return true;}
case '3':{Value=55;return true;}
case '4':{Value=56;return true;}
case '5':{Value=57;return true;}
case '6':{Value=58;return true;}
case '7':{Value=59;return true;}
case '8':{Value=60;return true;}
case '9':{Value=61;return true;}
case '+':{Value=62;return true;}
case '/':{Value=63;return true;}
default :{return false;}
}
return false;
}



#endif  //_BASE64_H
















