#include "ESP8266.h"

#include <SoftwareSerial.h>

#include <String.h>

#include <LiquidCrystal.h>

#include <Servo.h>

//사용한 헤더 파일

Servo servo;
Servo servo_1;
Servo servo_2;

//물품 보관함 속 우산 여부를 체크하기 위한 변수
int servo_check = 0;
int servo_1_check = 0;
int servo_2_check = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A1,
    en = A2,
    d4 = 4,
    d5 = 6,
    d6 = 7,
    d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//문 개폐를 위한 서보모터 핀 설정
int servo_Pin = A0;
int servo_Pin_1 = A3;
int servo_Pin_2 = A4;
int min_value = 544;
int max_value = 2400;

#include <SPI.h>

#include <MFRC522.h>

#define SDA_PIN 10 // SDA핀 설정
#define RST_PIN 9 // RESET 핀 설정

MFRC522 rfid(SDA_PIN, RST_PIN); // rfid로 객체 생성

#define SSID "ssid"

#define PASSWORD "passwd"

#define SERVERIP "host_ip" //아마존 EC2 인스턴스 Public 주소

//통신핀 설정
#define BT_RXD 2
#define BT_TXD 3

SoftwareSerial mySerial(BT_RXD, BT_TXD);

void setup() {
    Serial.begin(9600);
    SPI.begin(); // Initiate  SPI bus
    rfid.PCD_Init(); // RFID 시작 
    lcd.begin(16, 2);
    lcd.print("Umbrella");
    lcd.setCursor(0, 1);
    lcd.print("Waiting Wifi...");

    Serial.setTimeout(5000);
    mySerial.begin(9600);
    Serial.println("ESP8266 connect");
    // set up the LCD's number of columns and rows:

    boolean connected = false;
    for (int i = 0; i < 10; i++) {
        if (connectWiFi()) { // connection WiFi 성공까지 대기
            connected = true;
            break;
        }
    }

    if (!connected) {
        while (1);
    }
    delay(5000);

    mySerial.println("AT+CIPMUX=0");

    lcd.clear();
}

//와이 파이 연결 AT 명령어 코드
boolean connectWiFi() {

    mySerial.println("AT+CWMODE=3");

    String cmd = "AT+CWJAP=\"";
    cmd += SSID;
    cmd += "\",\"";
    cmd += PASSWORD;
    cmd += "\"";
    mySerial.println(cmd);
    Serial.println(cmd);
    delay(3000);

    char check[100] = "OK";

    if (mySerial.find(check)) {
        Serial.println("OK, Connected to WiFi.");
        return true;
    } else {
        Serial.println("Can not connect to the WiFi.");
        return false;
    }
}



void loop(void) {

    lcd.clear();
    lcd.print("Waiting Tagging");

    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += SERVERIP;
    cmd += "\",80";
    Serial.println(cmd); //중요!

    Serial.println("Waiting for tagging...");
    mySerial.println(cmd);
    char mytemp2[16] = "Error";
    if (mySerial.find(mytemp2)) {
        Serial.println("TCP connect error");
        return;
    }



    // 카드를 읽으면 진행, 읽지못하면 리턴
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }

    // ID가 읽어지면 진행, 읽지못하면 리턴
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }
    Serial.print("UID tag :");

    String content = ""; // 문자열 자료형 content 선언 
    String ch = "";

    Serial.println(rfid.uid.size);
    for (byte i = 0; i < rfid.uid.size; i++) // tag를 읽고 출력하는 프로그램
    {
        // 0x10보다 작으면 " 0"을 아니면 " "을 출력
        byte j = 1;
        byte k = 0;
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");

        // 16진수로 출력
        Serial.print(rfid.uid.uidByte[i], HEX);

        // 문자열을 string에 추가
        content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(rfid.uid.uidByte[i], HEX));
    }
    for (byte j = 0; j < content[j] != 0; j++) {
        if (content[j] != ' ')
            ch.concat(content[j]);

    }


    Serial.println();
    Serial.print("Message : ");
    ch.toUpperCase(); // string의 문자를 대문자로 수정
    Serial.println(ch);




    char test[20];
    int temp = 100;

    cmd = "GET /RFID_tag.php?card_id=" + ch + "\r\n"; // 서버에 태깅 된 card_id 전송 
    mySerial.print("AT+CIPSEND=");
    mySerial.println(cmd.length());

    delay(1000);

    Serial.println(cmd);

    char mytemp[16] = ">";
    if (mySerial.find(mytemp)) {
        Serial.print(mytemp);
    } else {
        mySerial.println("AT+CIPCLOSE");
        Serial.println("connect timeout");
        delay(1000);
        return;
    }

    mySerial.print(cmd);



    while (Serial.available()) {
        char c = Serial.read();
        mySerial.write(c);
        if (c == '\r') mySerial.print('\n');
    }
    Serial.println("====");
    delay(1000);



    String response = "";
    int j = 0;
    while (mySerial.available()) {
        // output to the serial window
        char c = mySerial.read(); // read the next character.
        response += c;
    }
    
    Serial.print(response); // 서버로부터의 응답 출력
    int index = response.indexOf("+IPD");
    Serial.println(index);
    if (index == -1) {
        Serial.println("ipd error");
    } else {
        Serial.println(response[index + 7]); //모터 동작하기 위한 IPD 확인값 0,1,2,3 값 중 하나
        if (response[index + 7] == '0') {
            Serial.println("==등록되지 않은 사용자입니다==");
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.print("You are not");
            lcd.setCursor(0, 1);
            lcd.print("registerd !!");
            delay(5000);
        } else if (response[index + 7] == '1') { // 대여자
            //모터 동작



            if (servo_check == 0) {

                lcd.setCursor(0, 0);
                lcd.clear();
                lcd.print("Take an Umbrella");
                delay(1000);

                servo.attach(servo_Pin, min_value, max_value);
                servo.write(90);
                delay(1000);
                servo.write(180);
                delay(5000);
                servo.write(90);
                delay(1000);
                servo.detach();
                servo_check = 1;
                
                
            } else if (servo_1_check == 0) {

                lcd.setCursor(0, 0);
                lcd.clear();
                lcd.print("Take an Umbrella");
                delay(1000);

                servo_1.attach(servo_Pin_1, min_value, max_value);
                servo_1.write(90);
                delay(1000);
                servo_1.write(180);
                delay(5000);
                servo_1.write(90);
                delay(1000);
                servo_1.detach();
                servo_1_check = 1;
                
                
            } else if (servo_2_check == 0) {
                lcd.setCursor(0, 0);
                lcd.clear();
                lcd.print("Take an Umbrella");
                delay(1000);

                servo_2.attach(servo_Pin_2, min_value, max_value);
                servo_2.write(90);
                delay(1000);
                servo_2.write(180);
                delay(5000);
                servo_2.write(90);
                delay(1000);
                servo_2.detach();
                servo_2_check = 1;
            }



        } else if (response[index + 7] == '2') { // 반납자
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.print("Return Umbrella!");
            lcd.setCursor(0, 1);
            lcd.print("Thank you!");
            delay(1000);

            if (servo_check == 1) {
                servo.attach(servo_Pin, min_value, max_value);
                servo.write(90);
                delay(1000);
                servo.write(180);
                delay(5000);
                servo.write(90);
                delay(1000);
                servo.detach();
                servo_check = 0;
            } else if (servo_1_check == 1) {
                servo_1.attach(servo_Pin_1, min_value, max_value);
                servo_1.write(90);
                delay(1000);
                servo_1.write(180);
                delay(5000);
                servo_1.write(90);
                delay(1000);
                servo_1.detach();
                servo_1_check = 0;
            } else if (servo_2_check == 1) {
                servo_2.attach(servo_Pin_2, min_value, max_value);
                servo_2.write(90);
                delay(1000);
                servo_2.write(180);
                delay(5000);
                servo_2.write(90);
                delay(1000);
                servo_2.detach();
                servo_2_check = 0;
            }



        } else if (response[index + 7] == '3') { // 잔여 물품 없음
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.print("Sorry...");
            lcd.setCursor(0, 1);
            lcd.print("No Stock!");
            delay(5000);
        }
        
    }

}
