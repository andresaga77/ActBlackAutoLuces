/* Programa: Automatización de luces
 * Autores: Andrés Gonzalez y Andrés Carvajal
 * Fecha de inicio del proyecto: noviembre de 2021
 * Descripción del proyecto: ... TO-DO
 */

#include <FastLED.h>//Libreria que permite controlar el tiempo-Conteo de segundos
#include "BluetoothSerial.h"


#define TIEMPO_LECTURA_BLUETOOTH 500 // Se lee cada 500 ms para que el usuario no perciba un retardo
BluetoothSerial ESP_BT; //Object for Bluetooth
uint8_t incoming = 0;  // Almacena hasta 256 opciones de datos entrantes por el Bluetooth
uint8_t flag_incoming = 0; // Se pone en 1 cuando hay datos recibidos por el Bluetooth



void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("ActBlack_Sabaneta"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  
}

void loop() {  
  // TO-DO: Activar una bandera en una interrupcion por tiempo para hacer esto menos frecuente  
  EVERY_N_MILLISECONDS(TIEMPO_LECTURA_BLUETOOTH){
    if (ESP_BT.available()){ // Se revisa si hay algún dato entrando por Bluetooth
          incoming = ESP_BT.read(); // Lee y almacena lo que se recibe por Bluetooth
          Serial.print("Received:"); Serial.println(incoming);
          flag_incoming = 1; // Se advierte que llegó un dato por Bluetooth
    }else{
      Serial.println("Nothing found"); // TO-DO DEBUG
      //flag_incoming = 0; TO-DO optional!?
    }
  }

  EVERY_N_SECONDS(1){
    contSeg ++;
    if(contSeg>59){
      contSeg=0;
      contMin ++;
    }
    if(contMin>59){
      contMin=0;
    }
  }
  }
}
