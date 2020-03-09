#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11) ; //TX, RX

String readvoice;
int sensor_1 = A0;
int sensor_2 = A1;
int sensor_3 = A2;
int buffer_state = 0;
int flag;
char path[] = {'a', 's', 's', 'a', 's', 'a', 's'};


void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  String voice_input = bluetooth_voice();

  if (voice_input.length()) {

    Serial.println(voice_input);
    voice_operation(voice_input);
  }

  if (Serial.available()) {
    char x = (char)Serial.read();

    if (x == 'q') {
      buffer_state = 0;
      Serial.println("\n Initialed to follow Path : \n");
    }

    int i = 0;
    while (buffer_state == 0) {
      //      Serial.println("In buffer");
      if (MLS() && !LTS() && !RTS()) {
        Serial.print("\n Going " + controller('w'));
      } else if (MLS()) {
        Serial.print("\n\n Node Found - Going : ");
        Serial.print(controller(path[i]) + "\n");
        i++;
        delay(1000);
      } else {
        Serial.println("\n Main line sensor not found");
        controller('f');
      }
      delay(500);

      if ((char)Serial.read() == 'e') {
        buffer_state = 1;
        controller('f');
        Serial.println("Buffer Exit");
      }
    }
  }
}

String controller(int x) {
  switch (x) {
    case 'w':
      digitalWrite(3, HIGH);
      digitalWrite (4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      delay(100);

      return "Forward";
      break;

    case 's':
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      delay(100);

      return "Backward";
      break;

    case 'a':
      flag = 0;
      while (1) {
        digitalWrite (3, HIGH);
        digitalWrite (4, LOW);
        digitalWrite (5, LOW);
        digitalWrite (6, LOW);
        if (!MLS()) {
          flag = 1;
        }
        if (MLS() && flag == 1) {
          controller('f');
          break;
        }
      }
      return "Left";
      break;

    case 'd':
      flag = 0;
      while (1) {
        digitalWrite (3, LOW);
        digitalWrite (4, LOW);
        digitalWrite (5, HIGH);
        digitalWrite (6, LOW);
        if (!MLS()) {
          flag = 1;
        }
        if (MLS() && flag == 1) {
          controller('f');
          break;
        }
      }
      return "right";
      break;

    case 'f':
      digitalWrite(3, LOW);
      digitalWrite (4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      delay(100);

      return "stop";
      break;

    default:
      buffer_state = 1;
      digitalWrite(3, LOW);
      digitalWrite (4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      delay(100);

      return "stop";
      break;
  }
}

String bluetooth_voice() {
  char message[50] = "";
  int string_length = 0;
  String string = "";

  while (BT.available()) { //Check if there is an available byte to read
    delay(10); //Delay added to make thing stable
    char c = BT.read();
    message[string_length] = c;
    string_length++;

    if (string_length > 0) {
      string = message;
    }
  }

  return string;
}

void voice_operation(String string) {
  if (string == "forward") {
    while (1) {
      String voice_input = bluetooth_voice();
      controller('w');

      if (voice_input.length()) {
        Serial.println("Stop");
        break;
      }
    }
  }
  if (string == "backward") {
    while (1) {
      String voice_input = bluetooth_voice();
      controller('s');

      if (voice_input.length()) {
        Serial.println("Stop");
        break;
      }
    }
  }
  if (string == "left") {
    controller('a');
  }
  if (string == "right") {
    controller('d');
  }
  if (string == "stop") {
    controller('f');
  } else {
    controller('f');
  }
}

bool MLS() {
  // Mid Line Sensor
  //    Serial.println(analogRead(sensor_1));
  while (analogRead(sensor_1) > 500) {
    return true;
  }
  return false;
}
bool LTS() {
  // Left Turn Sensor
  //    Serial.println(analogRead(sensor_2));
  while (analogRead(sensor_2) > 500) {
    return true;
  }
  return false;
}
bool RTS() {
  // Right Turn Sensor
  //    Serial.println(analogRead(sensor_3));
  while (analogRead(sensor_3) > 500) {
    return true;
  }
  return false;
}
