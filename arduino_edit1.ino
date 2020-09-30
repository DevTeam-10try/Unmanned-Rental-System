#include "ESP8266.h"
#include <SoftwareSerial.h>  
#include <String.h> 

#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 10   // SDA핀 설정
#define RST_PIN 9   // RESET 핀 설정

MFRC522 rfid(SDA_PIN, RST_PIN);   // rfid로 객체 생성

#define SSID        "wifi이름"  /// wifi 값 바꾸기!!!!!

#define PASSWORD    "비밀번호"  

#define SERVERIP   "ip" //!!!!!!인스턴스 주소 계속 바뀌니까 중지하고 킬때마다 이 값 변경해주기!!!!!!

#define BT_RXD 2 
#define BT_TXD 3 
SoftwareSerial mySerial(BT_RXD, BT_TXD); 
    
void setup() { 
  Serial.begin(9600);    
  SPI.begin();      // Initiate  SPI bus
  rfid.PCD_Init();   // RFID 시작
 /////////////////////////////////////////////////////////////////////////  
  Serial.setTimeout(5000);  
  mySerial.begin(9600);   
  Serial.println("ESP8266 connect");  
  
    
   boolean connected=false;  
   for(int i=0;i<10;i++)  
   {  
       if(connectWiFi())  
       {  
         connected = true;  
         break;  
       }  
   }  
     
   if (!connected){while(1);}  
   delay(5000);  
    
   mySerial.println("AT+CIPMUX=0");  
}

boolean connectWiFi()  
{  
   mySerial.println("AT+CWMODE=3");  
     
   String cmd="AT+CWJAP=\"";  
   cmd+=SSID;  
   cmd+="\",\"";  
   cmd+=PASSWORD;  
   cmd+="\"";  
   mySerial.println(cmd);  
   Serial.println(cmd);  
   delay(3000);  

   char check[100] = "OK";
   
   if(mySerial.find(check))
   {  
     Serial.println("OK, Connected to WiFi.");  
     return true;  
   }  
   else  
   {  
     Serial.println("Can not connect to the WiFi.");  
     return false;  
   }  
 } 

      
      
void loop(void)    
{     
  //Serial.print("in the Loop");    
  //Serial.println("DONE");    
    
  String cmd = "AT+CIPSTART=\"TCP\",\"";  
     cmd += SERVERIP;  
     cmd += "\",80";  
     Serial.println(cmd); //중요!
     Serial.println("Waiting for tagging...");
     mySerial.println(cmd);  
     char mytemp2[16] = "Error";
     if(mySerial.find(mytemp2))  
    {  
      Serial.println( "TCP connect error" );  
      return;  
    }  



  // 카드를 읽으면 진행, 읽지못하면 리턴
 if ( ! rfid.PICC_IsNewCardPresent())
 {
   return;
 }

 // ID가 읽어지면 진행, 읽지못하면 리턴
 if ( ! rfid.PICC_ReadCardSerial())
 {
   return;
 }
  Serial.print("UID tag :");

 String content= ""; // 문자열 자료형 content 선언 
 String ch="";

Serial.println(rfid.uid.size);
 for (byte i = 0; i < rfid.uid.size; i++) // tag를 읽고 출력하는 프로그램
 {
   // 삼항 연산자. 16(0x10)보다 작으면 " 0"을 아니면 " "을 출력
   byte j =1; byte k=0;
   Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
   
   // 16진수로 출력
   Serial.print(rfid.uid.uidByte[i], HEX);
   
   // 문자열을 string에 추가
   content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
   content.concat(String(rfid.uid.uidByte[i], HEX));
 }
 for (byte j=0; j<content[j]!=0;j++)
 {
    if (content[j] != ' ')
      ch.concat(content[j]);
     
  }
  

 Serial.println();
 Serial.print("Message : ");
 ch.toUpperCase(); // string의 문자를 대문자로 수정
 Serial.println(ch);



     
    char test[20];  
    int temp = 100;

     cmd = "GET /RFID_tag.php?card_id="+ch+"\r\n";  // !!!!수정!!!!
     //cmd = "GET /test.html";  // !!!!수정!!!!
     mySerial.print("AT+CIPSEND=");  
     mySerial.println(cmd.length());  

    delay(1000);
  
     Serial.println(cmd);  

       char mytemp[16] = ">";
     if(mySerial.find(mytemp))  
     {  
       Serial.print(mytemp);  
       }else  
       {  
         mySerial.println("AT+CIPCLOSE");  
         Serial.println("connect timeout");  
         delay(1000);  
         return;  
       }  
       
       mySerial.print(cmd); 

       
              
       while (Serial.available())  
       {  
         char c = Serial.read();  
         mySerial.write(c);  
         if(c=='\r') mySerial.print('\n');  
       }  
       Serial.println("====");  
       delay(1000); 
       
       
       
        String response = "";
while (mySerial.available()) {
      // output to the serial window
      char c = mySerial.read(); // read the next character.
      response += c;
    }
Serial.print(response);

       
        
}   
