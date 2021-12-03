#include<SPI.h>
#include<MFRC522.h>
#include <Ethernet.h>
#include <Servo.h>
Servo servo;
#define SS_PIN 9 //FOR RFID SS PIN BECASUSE WE ARE USING BOTH ETHERNET SHIELD AND RS-522
#define RST_PIN 4
#define No_Of_Card 3
int servoMotor = 7;
int buzz = 8;
int led = 5;
int pos = 0;
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x66, 0x05 };
char server[] = "api.pushingbox.com";   //YOUR SERVER
IPAddress ip(192, 168, 0, 108);
EthernetClient client;     
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
byte id[No_Of_Card][4]={
  {164,46,199,36},             //RFID NO-1
  {87,12,102,201},             //RFID NO-2
  {170,26,205,189}             //RFID NO-3
};
byte id_temp[3][3];
byte i;
int j=0;


// the setup function runs once when you press reset or power the board
void setup(){
    servo.attach(servoMotor);
    pinMode(buzz,OUTPUT);
    pinMode(led,OUTPUT);
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    for(byte i=0;i<6;i++)
    {
      key.keyByte[i]=0xFF;
    }
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      Ethernet.begin(mac, ip);
    }
    delay(1000);
    Serial.println("connecting...");
 }

// the loop function runs over and over again forever
void loop(){
    int m=0;
    if(!rfid.PICC_IsNewCardPresent())
    return;
    if(!rfid.PICC_ReadCardSerial())
    return;
    for(i=0;i<4;i++)
    {
     id_temp[0][i]=rfid.uid.uidByte[i]; 
               delay(50);
    }
    
     for(i=0;i<No_Of_Card;i++)
    {
            if(id[i][0]==id_temp[0][0])
            {
              if(id[i][1]==id_temp[0][1])
              {
                if(id[i][2]==id_temp[0][2])
                {
                  if(id[i][3]==id_temp[0][3])
                  {
                    Serial.print("your card no :");
                    for(int s=0;s<4;s++)
                    {
                      Serial.print(rfid.uid.uidByte[s]);
                      Serial.print(" ");
                     
                    }
                    Serial.println("\nValid Person");
                    Sending_To_spreadsheet();
                    j=0;
                              
                              rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();
                              digitalWrite(led,HIGH);
                            delay(2000);
                            digitalWrite(led,LOW);
                            for(pos=0;pos<=180;pos++){
                              servo.write(pos);
                              delay(30);
                            }
                              return; 
                  }
                }
              }
            }
     else
     {j++;
      if(j==No_Of_Card)
      {
        Serial.println("Not a valid Person");
        Sending_To_spreadsheet();
        j=0;
        digitalWrite(buzz,HIGH);
      delay(1000);
      digitalWrite(buzz,LOW);
      }
     }
    }
    
       // Halt PICC
    rfid.PICC_HaltA();
  
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
    
 }

 void Sending_To_spreadsheet()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /pushingbox?devid=v183273AA325F892&allowed_members=");     //YOUR URL
    if(j!=No_Of_Card)
    {
      client.print('1');
//      Serial.print('1');
    }
    else
    {
      client.print('0');
    }
    
    client.print("&Member_ID=");
    for(int s=0;s<4;s++)
                  {
                    client.print(rfid.uid.uidByte[s]);
                                  
                  }
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
