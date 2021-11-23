/* Programa: Automatización de luces
 * Autores: Andrés Gonzalez y Andrés Carvajal
 * Fecha de inicio del proyecto: noviembre de 2021
 * Descripción del proyecto: ... TO-DO
 */

#include <FastLED.h>//Libreria que permite controlar el tiempo-Conteo de segundos
#include "BluetoothSerial.h"


//Variables para el manejo de la función de Bluethooth
#define TIEMPO_LECTURA_BLUETOOTH 500 // Se lee cada 500 ms para que el usuario no perciba un retardo
BluetoothSerial ESP_BT; //Object for Bluetooth
uint8_t incoming = 0;  // Almacena hasta 256 opciones de datos entrantes por el Bluetooth
uint8_t flag_incoming = 0; // Se pone en 1 cuando hay datos recibidos por el Bluetooth
uint8_t flagStart = 0; // Se pone en 1 cuando se recibe el comando de inicio
uint8_t flagPause = 0; // Se pone en 1 cuando se recibe el comando de pausar
uint8_t flagRestart = 0; // Se pone en 1 cuando se recibe el comando de reiniciar


//Pines de salida, controlan los relés
int LETRERO=15;//Se define el pin que controlará el funcionamiento del relé que controla el letrero
int LUCES=2;//Se define el pin que controlará el funcionamiento del relé que controla las luces del medio del salón
int VENTILADOR=4;//Se define el pin que controlará el relé al que irán conectados los ventiladores
int SPOTS=5;//Se define el pin que controlará el relé al que irán conectados los spots

//Variables relacionadas con el tiempo
int contMin=0;

//Funciones de estados
void ESTADO0();
void ESTADO1();
void ESTADO2();
void ESTADO3();
void parar();
void restart();

void setup() {
  //Se definen los pines inicialmente en alto
  pinMode(LETRERO, OUTPUT);
  pinMode(LUCES, OUTPUT);
  pinMode(VENTILADOR, OUTPUT);
  pinMode(SPOTS, OUTPUT);
  ESTADO0();
  //
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
          if(incoming==49){//Se pregunta si la llegada es del botón de inicio
            flagStart=1;
          }
          if(incoming==2){//Se pregunta si la llegada es del botón de pausa
            parar();         
          }
          
          if(incoming==3){//Se pregunta si la llegada es del botón de Reinicio
            restart();        
          }
    }else{
      //Serial.println("Nothing found"); // TO-DO DEBUG
      //flag_incoming = 0; TO-DO optional!?
    }
  }

  EVERY_N_SECONDS(1){
    if(flagStart==1){
      contMin ++;
      Serial.println(contMin);
      //Se llega al primer estado, cuando ha pasado el tiempo de calentamiento se apaga el LETRERO
      if(contMin==7){
        ESTADO1();   
      }
      //Se llega al segundo estado, cuando ha pasado el tiempo de 14 minutos se apagan las luces del medio
      if(contMin==14){
          ESTADO2();  
      }
      //Se llega al tercer estado, cuando ha pasado el tiempo de 21 minutos se encienden los ventiladores
     if(contMin==21){
        ESTADO3();      
     }
     //Al acabarse la clase se encienden todos los 
      if(contMin>59){
        contMin=0;
        flagStart=0;
        ESTADO0();
       }
    }
  }
}

void ESTADO0(){
  digitalWrite (LETRERO, LOW);
  digitalWrite (LUCES, LOW);
  digitalWrite (VENTILADOR, HIGH);
  digitalWrite (SPOTS, HIGH);
}

void ESTADO1(){
  digitalWrite (LETRERO, HIGH);
  digitalWrite (LUCES, LOW);
  digitalWrite (VENTILADOR, HIGH);
  digitalWrite (SPOTS, HIGH);
}

void ESTADO2(){
  digitalWrite (LETRERO, HIGH);
  digitalWrite (LUCES, HIGH);
  digitalWrite (VENTILADOR, HIGH);
  digitalWrite (SPOTS, HIGH);
}

void ESTADO3(){
  digitalWrite (LETRERO, HIGH);
  digitalWrite (LUCES, HIGH);
  digitalWrite (VENTILADOR, LOW);
  digitalWrite (SPOTS, HIGH);
}
//Con esta función detengo el contador de la clase
void parar(){
  flagRestart=0;
  flagStart=0;
  flagPause=0;
}

//Con esta función reinicio el contador de la clase
void restart(){
  contMin=0;
  flagRestart=0;
  flagStart=0;
  flagPause=0;
}
