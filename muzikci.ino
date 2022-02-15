#include <Servo.h>
#include <Stepper.h>

//NOTALAR
#define DO 0
#define RE_b 20
#define RE 40
#define MI_b 60
#define MI 80
#define FA 100
#define SOL_b 120
#define SOL 140
#define LA_b 160
#define LA 181
#define SI_b 183
#define SUS -1
#define END -2

//SURELER 
#define CIFT 8
#define TEK 4
#define YARIM 2
#define CEYREK 1

//DURUM
#define DURUM_ACIK 1
#define DURUM_KAPALI 0

Servo servo_yukseklik;
Stepper stepmotor(200, 8, 9, 10, 11);

int mevcutNota = 0;
long bitis = 0;
int tempo = 0;
int durum = DURUM_KAPALI;
long butonSonBasmaZamani = 0;
int sarkino = 0;




int tas_okul_marsi[] = {SOL,TEK, SOL,TEK, SOL,YARIM, SOL,YARIM, SOL,TEK, 
MI_b,YARIM, RE,YARIM, RE,YARIM, MI,YARIM, DO,CIFT, 
DO,YARIM, DO,CEYREK, DO,TEK, DO,YARIM, RE,YARIM, MI_b,TEK, 
MI_b,YARIM, RE,YARIM, MI,YARIM, FA,YARIM, SOL,CIFT, 
LA_b,YARIM, LA,CEYREK, LA,YARIM, LA,YARIM, LA,YARIM, SOL,YARIM, FA,TEK, 
MI_b,YARIM, RE,YARIM, MI,YARIM, FA,YARIM, SOL,CIFT, 
DO,YARIM, DO,YARIM, DO,YARIM, RE,YARIM, MI_b,YARIM, FA,YARIM, SOL,TEK, 
RE,YARIM, FA,YARIM, MI_b,YARIM, RE,YARIM, DO,CIFT, 
LA_b,YARIM, LA,CEYREK, LA,YARIM, LA,YARIM, LA,YARIM, SOL,YARIM, FA,TEK, 
MI_b,YARIM, RE,YARIM, MI,YARIM, FA,YARIM, SOL,CIFT, 
DO,YARIM, DO,CEYREK, DO,YARIM, RE,YARIM, MI_b,YARIM, FA,YARIM, SOL,TEK, 
RE,YARIM, FA,YARIM, MI_b,YARIM, RE,YARIM, DO,CIFT, 
LA_b,YARIM, LA,CEYREK, LA,YARIM, LA,YARIM, LA,YARIM, SOL,YARIM, FA,TEK, 
MI_b,YARIM, RE,YARIM, MI,YARIM, FA,YARIM, SOL,CIFT, 
DO,YARIM, DO,CEYREK, DO,YARIM, RE,YARIM, MI_b,YARIM, FA,YARIM, SOL,TEK, 
RE,YARIM, FA,YARIM, MI_b,YARIM, RE,YARIM, DO,CIFT, 
DO,YARIM, DO,CEYREK, DO,YARIM, RE,CEYREK, MI_b,TEK, SUS,TEK, 
RE,TEK, RE,YARIM, RE,CEYREK, MI_b,CEYREK, FA,CIFT, 
SOL,YARIM, SOL,CEYREK, SOL,CEYREK, SOL,YARIM, MI_b,YARIM, RE,CEYREK, DO,TEK, 
SOL,YARIM, FA,YARIM, MI_b,YARIM, RE,YARIM, MI,CEYREK, DO,CIFT, 
DO,YARIM, DO,CEYREK, DO,YARIM, RE,CEYREK, MI_b,TEK, SUS,TEK, 
RE,TEK, RE,YARIM, RE,CEYREK, MI_b,CEYREK, FA,CIFT, 
SOL,YARIM, SOL,CEYREK, SOL,CEYREK, SOL,YARIM, MI_b,YARIM, RE,CEYREK, DO,TEK, 
SOL,YARIM, FA,YARIM, MI_b,YARIM, RE,YARIM, MI,CEYREK, DO,CIFT, 
SUS,CIFT, SUS,CIFT, SUS,CIFT, END};

int pirates_of_caribean[] = {DO,CEYREK,MI,YARIM,FA,YARIM,FA,YARIM,FA,CEYREK,SOL,YARIM,LA_b,YARIM,LA_b,
YARIM,LA_b,YARIM,LA,YARIM,SOL,YARIM,SOL,YARIM,FA,YARIM,MI,YARIM,FA,YARIM,
SUS,CIFT,END};

int neseli_gunler[] = {DO,CIFT,
RE,YARIM,
MI,TEK,
DO,TEK,
MI,TEK,
DO,TEK,
MI,CIFT,

RE,CIFT,
MI,YARIM,
FA,YARIM,
FA,YARIM,
MI,YARIM,
RE,YARIM,
FA,CIFT,

MI,CIFT,
FA,YARIM,
SOL,TEK,
MI,TEK,
SOL,TEK,
MI,TEK,
SOL,CIFT,

LA,CIFT,
DO,YARIM,
RE,YARIM,
MI,YARIM,
FA,YARIM,
SOL,YARIM,
LA,YARIM,
LA,YARIM,

LA,CIFT,
SUS,TEK,
LA,YARIM,
LA,YARIM,
LA,TEK,
FA,TEK,
LA,TEK,
SOL,TEK,
LA,TEK,
SOL,TEK,
MI,TEK,
DO,TEK,SUS,CIFT,SUS,CIFT,END  };

int test[] = {DO,YARIM,RE_b,YARIM,RE,YARIM,MI_b,YARIM,MI,YARIM,FA,YARIM,SOL_b,YARIM,SOL,YARIM,LA_b,YARIM,LA,YARIM,LA,YARIM,
LA_b,YARIM,SOL,YARIM,SOL_b,YARIM,FA,YARIM,MI,YARIM,MI_b,YARIM,RE,YARIM,RE_b,YARIM,DO,YARIM,SUS,CIFT,END};




void stopM(int brakeLevel)
{  
  analogWrite(6,brakeLevel); 
}


void setup() {
  Serial.begin(115200);  

  pinMode(6,OUTPUT);
  analogWrite(6,255);
     
  servo_yukseklik.attach(5);
  servo_yukseklik.write(90);  
  delay(3000);
  servo_yukseklik.write(105);
  delay(100);
  servo_yukseklik.write(90);  
  stepmotor.setSpeed(125);

  attachInterrupt(digitalPinToInterrupt(2), butonBaslatDurdur, RISING);
  attachInterrupt(digitalPinToInterrupt(3), butonMuzikDegistir, RISING);  
  Serial.println("acilis");
  delay(100);
  
}

void tempoHesapla()
{
  int voltaj = analogRead(A0);
  tempo = map(voltaj, 0, 1023, 30, 130);    
}

void cal(int notalar[])
{
  Serial.print("cal()");
  int i=0;
  while ( notalar[i]!=END && durum == DURUM_ACIK)
  {
    Serial.println( String(notalar[i]) + ", " + String(notalar[i+1]) );
    notaCal(notalar[i],notalar[i+1]);
    i = i+2;
    
  }
}
void notaCal(int nota, int vurus){
  
  tempoHesapla();
  int sure = (int) (1000 * (60.0/tempo) * (vurus/4.0));  
 
  if (nota==SUS){
    while (bitis>0 && bitis>millis())
      delay(1); 
      
    bitis = millis() + sure;
    return;
  }  
  
  int adim = (nota - mevcutNota) ; 
  Serial.println(String(mevcutNota)+"->"+String(nota)+" adim:" + String(adim)+" vurus:"+String(vurus)+" tempo:"+String(tempo)+" süre:"+String(sure) +" bitis:"+String(bitis)  );
  analogWrite(6,150);   
  stepmotor.step(adim);delay(30);stopM(45);
  int counter=0;
  
  while (bitis>0 && bitis>millis()){
    delay(1); 
    counter++;
    if (counter%20==0)
    Serial.print('.');//Serial.print(millis());Serial.print('-');
  }
  Serial.println('+');
  
  servo_yukseklik.write(107);
  bitis = millis() + sure;
  delay(115);
  servo_yukseklik.write(95);delay(100);
  mevcutNota = nota;
}


void butonBaslatDurdur()
{
  if ((millis()-butonSonBasmaZamani)<500) 
  {    
    return;
  }
  durum = (durum + 1) % 2;
  butonSonBasmaZamani = millis();
  Serial.println(" --DURUM: "+String(durum));
}

void butonMuzikDegistir()
{
  if ((millis()-butonSonBasmaZamani)<500) 
  {    
    return;
  }
  butonSonBasmaZamani = millis();
  sarkino = (sarkino + 1) % 4;
  Serial.println(" --SARKI NO:"+String(sarkino));

}



void loop() {
  
  delay(1);
  if (durum == DURUM_ACIK){
    if (sarkino==0)
      cal(tas_okul_marsi);
    else if (sarkino==1)
      cal(pirates_of_caribean);
    else if (sarkino==2)
      cal(neseli_gunler);  
    else if (sarkino==3)     
      cal(test);
  }
  else{
    stopM(0);
  }


  
}

