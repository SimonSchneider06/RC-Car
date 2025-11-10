#include <SPI.h>;
#include <nRF24L01.h>;
#include <RF24.h>;

const int NRF_CE = 7;
const int NRF_CSN = 8;

const int X = A1;
const int Y = A4;

RF24 radio(NRF_CE, NRF_CSN);
const byte address[6] = "00001";

int gas;
int lenkung;

struct Data_Package {
  int gas;
  int lenkung;
};

Data_Package data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {
  // put your main code here, to run repeatedly:
  int x_val = analogRead(X);
  int y_val = analogRead(Y);
  
  data.gas = map(x_val,0,1023,0,255);
  data.lenkung = map(y_val,0,1023,60,120);

  Serial.println(data.gas);
  Serial.println(data.lenkung);
  //Serial.println(lenkung);  // rechter joy-con (Y) A4
  radio.write(&data, sizeof(Data_Package));
  //radio.write(&lenkung, sizeof(lenkung));
  //radio.write(data, sizeof(data));
  delay(15);
}
