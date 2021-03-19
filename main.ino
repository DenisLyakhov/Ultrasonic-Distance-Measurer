#include <TM1637Display.h>

const int buttonPort = 13;
const int sensorTrigger = 12;
const int sensorEcho = 11;
const int displayClk = 10;
const int displayDio = 9;

// Odstęp migającego napisu
const int tickInterval = 35;

int btnState, time = 0;

TM1637Display display(displayClk, displayDio);

// Postać napisu (cztery poziome kreski "----")
const uint8_t defaultDisplay[] = {
  SEG_G,
  SEG_G,
  SEG_G,
  SEG_G
};

// Funkcja wyświetlania migającego napisu
void displayDefaultState(){

  time = 0;

  btnState = 0;

  while(true){

    if(time < tickInterval){
      display.setSegments(defaultDisplay);
    } else if(time < tickInterval*2){
      display.clear();
    } else {
      time = 0;
    }

    // Jeżeli przycisk został zwolniony,
    // następuje wyjście z funkcji
    if(digitalRead(buttonPort) == HIGH){
     btnState = 1;
    } else if (btnState == 1){
     btnState = 0;
     break;
    }

   time++;
    
  }
}

// Konfiguracja
void setup() {
  
  Serial.begin(9600);
  
  Serial.print("Starting configuration");
  
  pinMode(buttonPort, INPUT);
  pinMode(sensorEcho, INPUT);
  
  display.setBrightness(0x0a);
  
  pinMode(sensorTrigger, OUTPUT);
  digitalWrite(sensorTrigger, LOW);
  
}

// Pętla główna
void loop() {

  // Migajacy napis...
  displayDefaultState();

  float mean = 0; 

  for(int i = 0; i < 20; i++){

    digitalWrite(sensorTrigger, HIGH);
    delay(10);
    digitalWrite(sensorTrigger, LOW);

    // Pobieramy kolejny pomiar czujnika
    mean += pulseIn(sensorEcho, HIGH)*0.034/2;
        
  }

  // Obliczenie średniej
  mean = mean / 2.0;

  // Wyświetlenie wyniku
  display.showNumberDecEx(mean, 0x20, true,  4, 0); 

  delay(100);

  // Oczekiwanie na naciśnięcie i zwolnienie przycisku
  while(true){
    if(digitalRead(buttonPort) == HIGH){
      btnState = 1;
     } else if (btnState == 1){
      break;
     }
  }
  
  delay(1);
  
}
