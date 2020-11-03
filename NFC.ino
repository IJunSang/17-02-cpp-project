/*
 역할 : 아두이노에 RFID모듈을 달아 컴퓨터와 시리얼 통신을 함으로서, 모듈에 카드를 대면 아두이노에서 카드의 ID번호를 받아
 시리얼 통신으로 컴퓨터에 ID번호를 넘겨주면, 컴퓨터는 ID번호를 받아 직원 개체에 저장한다.
 본 RC522 RFID 모듈은 SPI통신을 사용한다.
 따라서 아두이노에서 지정된 SPI전용 핀에 연결해야 하며 이 소스에 연결된 핀은 다음과 같다.
 RST핀 : 9번
 SS핀: 10번
 MOSI핀 : 11번
 MISO핀 : 12번
 SCK핀 : 13번
 */
 
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rc522(SS_PIN, RST_PIN);                  // rc522객체를 만들어 모듈에서 카드를 읽게 되면 정보를 이 객체에 담게 된다
void setup() {
  Serial.begin(9600);                           //시리얼 통신 시작, 보드레이트 9600
  SPI.begin();         
  rc522.PCD_Init();                              // MFRC522모듈로부터 값을 읽어오기 시작한다
 
void loop() {
  if(!rc522.PICC_IsNewCardPresent()) return;     // Look for new cards
  if(!rc522.PICC_ReadCardSerial()) return;       // Select one of the cards
  //모듈로부터 카드가 인식되면 rc522객체에 담겨있는 id정보를 serial통신으로 넘겨준다
  Serial.print(rc522.uid.uidByte[0]);
  Serial.print(rc522.uid.uidByte[1]);
  Serial.print(rc522.uid.uidByte[2]);
  Serial.print(rc522.uid.uidByte[3]);

  delay(1000);
}

