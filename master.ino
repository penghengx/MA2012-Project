#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3
int stage3_input = 5;   //stage 3 pin 8
int stage3_output = 4;  //stage 3 pin 7
//trapdoor 6
//vaultdoor 7

char recv;
int n = 3;                                            //n = number of attempts
int i;                                                //bluetooth input in decimals
int limit = 4;                                                

bool lockdown = true;
bool reset_input = false;

String password = "1234";
String input;
String command;
String stage = "start";
String print_statement;

SoftwareSerial stage3(stage3_input, stage3_output);
SoftwareSerial blueToothSerial(RxD,TxD);              //bluetooth device acts as a serial communication device
                                                      //receive and transmit with respect to the Arduino board
void setup()
{
    Serial.begin(9600);                               //start serial monitor, for communicating with the PC
    pinMode(limit, INPUT);
    pinMode(RxD, INPUT);                              //set mode of receive pin (from bluetooth)
    pinMode(TxD, OUTPUT);                             //set mode of transmit pin (to bluetooth)

    blueToothSerial.begin(9600);                      //start the bluetooth serial "port"

    //stage3
    pinMode(stage3_input, INPUT);
    pinMode(stage3_output, OUTPUT);
    stage3.begin(9600);
    attachInterrupt(digitalPinToInterrupt(RxD), stage3_talking, HIGH);


}
void stage3_talking(){
  // Serial.println("helo000");
  int stage3_command;
  stage3_command = stage3.read();
  Serial.println(stage3_command);
  blueToothSerial.println(stage3_command);
}


void password_checker(String input){
  Serial.println(input);
  if (input == password){
    blueToothSerial.println("LOGIN SUCCESSFUL!");
    blueToothSerial.println("Enter /help to view options.");
    stage = "authenticated";
  }
  else{
    blueToothSerial.println("Wrong Password!");
    
    print_statement = "Tries left: " + String(n-1);
    blueToothSerial.println(print_statement);
    n -= 1;
    if (n == 0){
      blueToothSerial.println("ENTERING LOCKDOWN MODE!!!");
      stage = "lockdown";
    }
  }
}

void authenticated_options(String input){
  if (input == "/help"){
    blueToothSerial.println("/current: View current progress.");
    blueToothSerial.println("/lockdown: Triggers a system wide lockdown.");
    blueToothSerial.println("/open: Opens castle");
    blueToothSerial.println("/vault: Has the vault been touched.");
  }
  else if (input == "/current"){
    blueToothSerial.println("Position of smth");
  }     
  else if (input == "/lockdown"){
    blueToothSerial.println("Castle going into lockdown.");
  }
  else if (input == "/open"){
    blueToothSerial.println("Castle opening up.");

  }
  else if (input == "/vault"){
    blueToothSerial.println("Vault touch or not");
  }
  else{
    blueToothSerial.println("Invalid option. Please try again");
  }
}


void loop()
{  
  if (stage == "lockdown"){
    while (true){
    }
  }

  if (command == "connect" && stage == "start"){
    blueToothSerial.println("ENTER PASSWORD: ");
    stage = "password";
  }

  if(blueToothSerial.available()){
    i = blueToothSerial.read();
    if(i == 10){
      command = input;
      input = "";
      }
    else if (i == 13){
      if (stage == "password"){
        password_checker(input);
      }
      else if (stage == "authenticated"){
        Serial.println(input);
        authenticated_options(input);
      }
      }
    else{
      input += char(i);
    }
  }
}