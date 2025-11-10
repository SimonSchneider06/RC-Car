#include <Servo.h>;
#include <SPI.h>;
#include <nRF24L01.h>;
#include <RF24.h>;

//Servo
Servo myServo;

//Pins of H-Bridge
const int H_Activation = 4;
const int H_INP_A = 3;
const int H_INP_B = 2;

//NRF PINS
const int NRF_Zwei = 7;
const int NRF_Sechs = 8;

//Receiver
RF24 radio(NRF_Zwei,NRF_Sechs);  //Ce and CNS
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
  pinMode(H_Activation, OUTPUT);
  pinMode(H_INP_A, OUTPUT);
  pinMode(H_INP_B, OUTPUT);

  Serial.begin(9600);
  myServo.attach(5);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available()){
    // int gas;
    // int lenkung;
    // radio.read(&gas, sizeof(gas));  //linker joy-con
    // radio.read(&lenkung, sizeof(lenkung));  //rechter joy-con
    radio.read(&data,sizeof(Data_Package));
    Serial.println(data.gas);
    Serial.println(data.lenkung);
    //Serial.println(lenkung);
    myServo.write(data.lenkung);
    if(data.gas > 130){
      analogWrite(H_Activation,data.gas);
      digitalWrite(H_INP_A,HIGH);
      digitalWrite(H_INP_B, LOW);
      Serial.println("130+");
    }
    else if(data.gas < 120){
      analogWrite(H_Activation,255);
      digitalWrite(H_INP_A,LOW);
      digitalWrite(H_INP_B, HIGH);
      Serial.println("120-");
    }
    else if (data.gas >= 120 and data.gas <= 130){
      analogWrite(H_Activation,0);
    }

    }

  delay(30);
}
