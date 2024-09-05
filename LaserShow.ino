// See LICENSE file for details
// Copyright 2016 Florian Link (at) gmx.de

#include "Laser.h"
#include "Drawing.h"
#include "Cube.h"
#include "Objects.h"
#include "Logo.h"

// Create laser instance (with laser pointer connected to digital pin 5)
Laser laser(5);

char buf[50];
const char pin[2] = ",";
char* token;
int i;
// 設定laser的解析度

char readch[34];
int read_len;
String inputString;//定義一個字符串用來存放讀取的數據
char inChar;

void setup()
{
	Serial.begin(115200);
	// Serial.println("Welcome to Laser Control");
	laser.init();
}
void square(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);

	laser.sendto(ax, ay);
	laser.on();
	laser.sendto(bx, by);
	laser.sendto(cx, cy);
	laser.sendto(dx, dy);
	laser.sendto(ax, ay);

}
void send2p(int x, int y) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);
	laser.on();
	laser.sendto(x, y);
}
int ch2in(char* ch) {
	/* 將輸入的char轉變為int*/
	int len = strlen(ch);
	int i, j, in(0);
	int k;
	for (i = 0; i <= len - 1; ++i) {
		j = len - 1 - i;
		k = (int)(ch[j]) - 48;
		in = (k)*pow(10, i) + in;
	}
	return in;
}
void strchrn(char* dest, char* src, char a, char b) {
	//將字串str內a和b之間的字元取出
	//dest輸出字串&緩衝區
	//src輸入的字串
	//a第一個尋找的字元
	//b第二個尋找的字元
	char* m = strchr(src, a) + 1;//strchr從特定字元後的字串內容
	char* n = strchr(src, b);
	int len = strlen(m) - strlen(n);//計算字串長度
	strncpy(dest, m, len);
	dest[len] = '\0';
}
int strcut2in(char* src, char a, char b) {
	//將字串char分割後並轉成int型態
	char* dest;
	int outint;
	char* m = strchr(src, a) + 1;
	char* n = strchr(src, b);
	int len = strlen(m) - strlen(n);//計算字串長度
	strncpy(dest, m, len);
	dest[len] = '\0';
	outint = ch2in(dest);
	return outint;
}
void Stopall() {
	laser.off();
	laser.sendto(1000, 1000);
}
void laseronoff() {
	// 用於打單點的雷射，藉由快速開關來顯示
	laser.on();
	laser.off();
}
void pro_range() {
	laser.setScale(1);
	laser.setOffset(2048, 2048);

	laser.sendto(0, 0);
	laseronoff();
	laser.sendto(0, 2047);
	laseronoff();
	laser.sendto(2047, 2047);
	laseronoff();
	laser.sendto(2047, 0);
	laseronoff();
	// laser.sendto(1023, 1023);
	// laseronoff();
}
void circle(int center_x, int center_y, int radius) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);
	laser.on();
	// 計算圓周座標
	int step = 5; // 擬合的圓周要精準
	float angle = (360 / step) * PI / 180;
	for (int i = 0; i < step; i++) {
		int circle_x = radius * cos(angle * i) + center_x;
		int circle_y = radius * sin(angle * i) + center_y;
		laser.sendto(circle_x, circle_y);
	}
}
void star(int center_x, int center_y, int radius) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);
	laser.on();
	int step = 5;
	int star_x[5] = {0,0,0,0,0};
	int star_y[5] = {0,0,0,0,0};
	int star_list[11] = { 0,1,2,3,4,0,2,4,1,3,0 };
	size_t star_list_size = sizeof(star_list) / sizeof(int);
	float angle = (360 / step) * PI / 180;

	for (int i = 0; i < step; i++) {
		star_x[i] = radius * cos(angle * i) + center_x ;
		star_y[i] = radius * sin(angle * i) + center_y ;    
	}
  for (int j = 0; j < (int)star_list_size; j++) {
    int k = star_list[j];
		laser.sendto(star_x[k], star_y[k]);    
	}
	

}
void block(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);

	laser.sendto(ax, ay);
	laser.on();
  laser.sendto(cx, cy);
  laser.off();
	laser.sendto(bx, by);
	laser.on();
	laser.sendto(dx, dy);
  laser.off();

}
void test220809_01() {
	//序列阜測試，讀取資料並直接印出
	if (Serial.available()) {
		int readlen = 50;
		buf[readlen] = Serial.read();
		Serial.write(buf[readlen]);
	}
}

// 221225新增
int char2int(char input_char) {
	// char to int
	int out_int = input_char - '0';
	return out_int;
}
int char2int_dec(char* input_char, int char_len_star) {
	//將輸入的四個字元轉換成整數
	// input_char : 輸入的字串陣列
	// char_len_star : 要被轉換的數值起始陣列位置
	// out_int : 被轉換的數值，輸出型態 int
	int out_int;
	char intput_char02[4];
	for (int i = 0; i < 4; i++) {
		intput_char02[i] = input_char[char_len_star + i];
		int out_int;
	}
	out_int = char2int(intput_char02[0]) * 1000 + char2int(intput_char02[1]) * 100 + char2int(intput_char02[2]) * 10 + char2int(intput_char02[3]);
	return out_int;
}
int char2int_type(char* input_char, int char_len_star) {
	//將輸入的兩個字元轉換成整數
	// input_char : 輸入的字串陣列
	// char_len_star : 要被轉換的數值起始陣列位置
	// out_int : 被轉換的數值，輸出型態 int
	int out_int;
	char intput_char02[2];
	for (int i = 0; i < 2; i++) {
		intput_char02[i] = input_char[char_len_star + i];
		int out_int;
	}
	out_int = char2int(intput_char02[0]) * 10 + char2int(intput_char02[1]);
	return out_int;
}
void pro_two_point() {
	laser.setScale(1);
	laser.setOffset(2048, 2048);

	laser.sendto(850, 1200);
	laseronoff();
	laser.sendto(1750, 1200);
	laseronoff();
}
void loop() {
}
void two_points(int ax, int ay, int bx, int by) {
	laser.setScale(1);
	laser.setOffset(2048, 2048);

	laser.sendto(ax, ay);
  laseronoff();
	laser.sendto(bx, by);
  laseronoff();

}
void serialEvent() {
	while (Serial.available()) {
		char inChar = (char)Serial.read();  //每次讀取一個字節
		delayMicroseconds(100);  //延時等待下一個數據接收到(單位1ms)
		inputString += inChar;  //將接收的字節拼接到字符串
	}
	inputString.toCharArray(readch, 34);

	//Serial.println("Arduino : Get!");  //串口返回打印接收到的數據
	//切割字串為各個點位置
	int type = char2int_type(readch, 0);
	int x1 = char2int_dec(readch, 2);
	int y1 = char2int_dec(readch, 6);
	int x2 = char2int_dec(readch, 10);
	int y2 = char2int_dec(readch, 14);
	int x3 = char2int_dec(readch, 18);
	int y3 = char2int_dec(readch, 22);
	int x4 = char2int_dec(readch, 26);
	int y4 = char2int_dec(readch, 30);
	//Serial.print("type : ");
	//Serial.println(type);

	inputString = ""; //清空接收的數據

	switch (type) {
	case 0:
		Stopall();
		break;
	case 1:
		/*  //用於檢查接收點的情況
		Serial.println("laser send to");
		Serial.print(x1);
		Serial.print(",");
		Serial.println(y1);
		*/
		while (1) {
			send2p(x1, y1);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
		//Serial.write('H');// 檢查碼
		break;
	case 2:
		/* //用於檢查接收點的情況
		Serial.println("laser send to");
		Serial.print(x1);
		Serial.print(",");
		Serial.println(y1);
		Serial.print(x2);
		Serial.print(",");
		Serial.println(y2);
		Serial.print(x3);
		Serial.print(",");
		Serial.println(y3);
		Serial.print(x4);
		Serial.print(",");
		Serial.println(y4);
		*/
		while (1) {
      int add=95;
			square(x1+add, y1, x2+add, y2, x3+add, y3, x4+add, y4);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}

		}
		break;
	case 3:
		while (1) {
			pro_range();
			if (Serial.available()) {
				break;
			}
		}
		break;

	case 4:
		while (1) {
			pro_two_point();
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
	case 5:
		while (1) {
			circle(x1, y1, x2);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
	case 6:
		while (1) {
			star(x1, y1, x2);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
  case 8:
  //2p1
		while (1) {
			two_points(335,920,680,920);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
  case 9:
  //2p2
		while (1) {
			two_points(570,740,900,740);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
  case 10:
  //2p3
		while (1) {
			two_points(1275,870,1680,870);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
  case 7:
		while (1) {
      int add_2=95;
			block(x1+add_2, y1, x2+add_2, y2, x3+add_2, y3, x4+add_2, y4);
      //square(x1, y1, x2, y2, x3, y3, x4, y4);
			Serial.println("N");// 回傳檢查碼
			if (Serial.available()) {
				break;
			}
		}
	}
}
	


