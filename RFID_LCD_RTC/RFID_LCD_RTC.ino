
#include <Wire.h>  // Lo trae Arduino IDE
#include <LiquidCrystal_I2C.h> // Incluimos la libreria del LCD

//RFID
#include <SPI.h>
#include <MFRC522.h>

//RTC
#include <Wire.h> 
#include "RTClib.h"
RTC_DS1307 RTC;


int segundo;
int minuto;
int hora;

// Seteamos la direccion LCD a 0x27 o 0x3F for un display de 20 caracteres 4 lineas
// Seteamos los pines sobre el chip I2C usados para las conexiones del LCD:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Seteamos la direccion I2C

///////////////////////////////////////// VARIABLES DE TARJETAS //////////////////////

String tarjeta = "37 47 CB 3B";
String tag2 = "46 7E 71 F9";
String tag3 = "05 A8 1C C3";
String tarjeta1 = "37 47 CB 3B";

///////////////////////////////////////// VARIABLES DE TARJETAS //////////////////////

String nombre = "Andres Roval";
String curso = "8vo A";
String cedula = "1717847972";
String telefono = "0969494623";

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 6 //define green LED pin
#define LED_R 7 //define red LED

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



void setup()   
{

// Carga de datos RTC
Wire.begin(); // Inicia el puerto I2C
RTC.begin(); // Inicia la comunicaci¢n con el RTC
RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora (Comentar una vez establecida la hora)
  
// Carga de datos RFID

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  Serial.println("Put your card to the reader...");
  Serial.println();

  lcd.begin(20,4);         // Inicializamos el lcd para 20 caracteres 4 lineas, Encendemos el backlight

// ------- Rapidos 3 blinks para el backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
    lcd.backlight(); // terminamos con el backlight on
    delay(3000);
    menu();
}

void loop()  
{

// Carga de la hora
 DateTime now = RTC.now();          // Obtiene la fecha y hora del RTC
 Serial.print(now.year(), DEC);  // A§o
 Serial.print('/');
 Serial.print(now.month(), DEC); // Mes
 Serial.print('/');
 Serial.print(now.day(), DEC);   // Dia
 Serial.print(' ');
 Serial.print(now.hour(), DEC);  // Horas
 Serial.print(':');
 Serial.println();
 Serial.print(now.minute(), DEC); // Minutos
 Serial.print(':');
 Serial.print(now.second(), DEC); // Segundos
 Serial.println();
 
  lcd.setCursor(0,2);
  lcd.print("D: ");
  lcd.print(now.year(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print(" ");
  lcd.setCursor(0,3);
  lcd.print("T: ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC); 
  delay(1000); // La informaci¢n se actualiza cada 1 seg.


Serial.print("Set Hora");

////////////////////////////////// Horarios ////////////////////////////////

int h1=23;   int m1=33;   int s1=0;

// Horario dos
int h2=23;   int m2=34;   int s2=0;

////////////////////////////////// Horarios ////////////////////////////////

int hora = (now.hour());
int minuto = (now.minute());

if (((hora==h1) && (minuto==m1)) || ((hora==h2)&&(minuto==m2))) {
   activar_timbre();
  }else{
    horaMal();
    }
}

void menu(){
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Universidad Israel");
  lcd.setCursor(0,1);
  lcd.print("Sistema de acceso");
  delay(3000);
  }

void horaMal(){

   Serial.println("Estamos en el timbre");
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
  if (content.substring(1) == tarjeta) //change here the UID of the card/cards that you want to give access
  {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    lcd.clear();
    delay(200);
    lcd.setCursor(0,2);
    delay(1000);   
    lcd.print("HORA INCORRECTA");
    delay(1000);
    lcd.clear();
    digitalWrite(LED_R, LOW);
    menu();
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    lcd.clear();
    delay(200);
    lcd.setCursor(0,2);
    delay(1000);   
    lcd.print("HORA INCORRECTA");
    delay(1000);
    lcd.clear();
    digitalWrite(LED_R, LOW);
    menu();
  } 
  

  }

void activar_timbre(){

   Serial.println("Estamos en el timbre");
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
  if (content.substring(1) == tarjeta) //change here the UID of the card/cards that you want to give access
  {

    lcd.clear();
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);


    lcd.setCursor(0,2); //Start at character 0 on line 0
    lcd.print("ACCESO AUTORIZADO");
    delay(1000);
    lcd.clear();
    
    lcd.setCursor(0,0); //Empiezo en la posicion 6 (caracter) sobre la linea 0
    lcd.print("Name: " + nombre);
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("Curso: " + curso);
    delay(500);  
    lcd.setCursor(0,2);
    lcd.print("Cedula: " + cedula);
    lcd.setCursor(0,3);
    delay(500);   
    lcd.print("Telefono: " + telefono);
    delay(5000);
    digitalWrite(LED_G, LOW);

    lcd.clear();
    menu();
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    lcd.clear();
    delay(200);
    lcd.setCursor(0,2);
    delay(1000);   
    lcd.print("ACCESO NO AUTORIZADO");
    delay(1000);
    lcd.clear();
    digitalWrite(LED_R, LOW);
    menu();
  } 
  
  }
