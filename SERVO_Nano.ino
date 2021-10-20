
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

int steps = 11;       // pin step 
int direccion = 12;   // pin direccion
int posicionPLC = 3;    //salida a PLC
float posicion =0;
int vuelta=0;
int consignaAp;
int AuxConsignaAp =0;
float servo;
float auxServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int vel;
float velConsigna;
int posAux=0;
int posAux2=0;
float posPLC =0;


//Estas 5 matricez pintan las barras en el display 
byte zero[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte one[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte two[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte three[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte four[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
}; 

void setup() {                
  
  Serial.begin(9600); //baud rate, señales por segundo
 /* Timer1.initialize(10000);  // 150 ms
  Timer1.attachInterrupt(interrupt); // Activa la interrupcion y la asocia a ISR_Blink */
  pinMode(steps, OUTPUT);   // inicializamos pin como salidas.
  pinMode(direccion, OUTPUT); 
  //pinMode(posicionPLC,OUTPUT);

   // inicializamos el LCD y lo asociamos las 5 matrices a un numero
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);


}
 
void loop() {

consignaAp = analogRead(A0); // en A0 conectamos la salida del PLC
vel = consignaAp - AuxConsignaAp;
AuxConsignaAp = consignaAp;//almaceno valor anterior de consigna
//velConsigna = -3.91 *sqrt((pow(vel,2))) + 5000; // mientras mayor es la dif de consiga mas rapido me muevo, mov proporcional
velConsigna = 3000;
//Serial.println(velConsigna);
//servo = (1.18*consignaAp) - 600; 

//auxServo = servo*-1; // obtengo los pulsos sin signo para consigna

//if (servo <0 && consignaAp <530 && posAux>0){ // giramos sentido antihorario, cerramos
if (consignaAp <480 && posAux>0){   
digitalWrite(direccion, LOW); 
//for (int x = 0; x < auxServo ; x++){ // Giramos 200 pulsos para hacer una vuelta completa
digitalWrite(steps, HIGH);
delayMicroseconds(9000); //1000 vmax
digitalWrite(steps, LOW);
//delayMicroseconds(2900); //1000 vmax
//delayMicroseconds(10);
posicion = posicion - 1.8; 
//}
}

//if (servo>0 && consignaAp >580 && posAux<100){// sentido horario, abrimos
if (consignaAp >580 && posAux<100){
//for (int x = 0; x < servo ; x++){ // Giramos 200 pulsos para hacer una vuelta completa 
digitalWrite(direccion, HIGH); 
digitalWrite(steps, HIGH);
delayMicroseconds(9000);
digitalWrite(steps, LOW);
//delayMicroseconds(2850); 
posicion = posicion + 1.8;
//}  
}
//posAux = (posicion*5)/108 ;//6 vueltas
posAux = (posicion*5)/72 ;//4vueltas

if(posAux>=20 && posAux<=90){
posAux2 = posAux-10 ; 
}else{
posAux2 = posAux ;  
}
lcd.setCursor(6,0);
lcd.print(posAux2);
lcd.print(" %   ");
updateProgressBar(posAux, 100, 1);

//Serial.println(consignaAp);  
 //posPLC = posAux*(51/20);
//analogWrite(posicionPLC,posPLC);
/*lcd.setCursor(6,0);
lcd.print(posAux2);
lcd.print(" %   ");
updateProgressBar(posAux, 100, 1);*/

}
/*
void interrupt()
{   
analogWrite(posicionPLC,posPLC);
}  
*/


 void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)
 {
  
    double factor = totalCount/80.0;       //cada columna tiene 5 secciones, es decir tenemos 80 secciones totales (16x5=80)
    int percent = (count+1)/factor;
    int number = percent/5;
    int remainder = percent%5;
    if(number > 0)
    {
       lcd.setCursor(number-1,lineToPrintOn);
       lcd.write(5);
    }
   
       lcd.setCursor(number,lineToPrintOn);
       lcd.write(remainder);   
 }
