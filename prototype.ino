#include <SPI.h>
#include <MFRC522.h>
#include<Servo.h>
Servo servo;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int servoMotor = 7;
int buzz = 8;
int led = 4;
int pos = 0;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  servo.attach(servoMotor);
  pinMode(buzz,OUTPUT);
  pinMode(led,OUTPUT);
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A4 2E C7 24" || content.substring(1) == "57 0C 66 C9") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(led,HIGH);
    delay(2000);
    digitalWrite(led,LOW);
    for(pos=0;pos<=180;pos++){
      servo.write(pos);
      delay(30);
 }
 }
 else  {
    Serial.println("Access denied");
    digitalWrite(buzz,HIGH);
    delay(1000);
    digitalWrite(buzz,LOW);
  }
} 
