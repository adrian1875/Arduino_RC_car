const int EN_L = 9;
const int MC_L1 = 2;
const int MC_L2 = 3;
const int EN_R =10;
const int MC_R1 =5;
const int MC_R2 = 4;

#define motor(en, m1,v1, m2, v2, rate) \
    digitalWrite(en, LOW);\
    digitalWrite(m1, v1);\
    digitalWrite(m2, v2);\  
    analogWrite(en, rate)  // 여기는 ;필요없음

  
void forward(int side = 0, int rate = 255)
{
       switch(side){
        case 2 : motor(EN_L,  MC_L1,  HIGH,  MC_L2,  LOW,  rate);  break;   // 오른쪽
        case 1 : motor(EN_R, MC_R1,  HIGH,  MC_R2,  LOW,  rate);  break;  // 왼쪽
        default:  
                 motor(EN_L,  MC_L1,  HIGH,  MC_L2,  LOW,  rate);  
                 motor(EN_R, MC_R1,  HIGH,  MC_R2,  LOW,  rate);
       }
}

  
void reverse(int side = 0, int rate = 255)
{

    motor(EN_L,  MC_L1,  LOW,  MC_L2,  HIGH,  rate);  
    motor(EN_R, MC_R1,  LOW,  MC_R2,  HIGH,  rate);
       
}

void stop(int side = 0,int rate = 0)
{
    motor(EN_L,  MC_L1,  LOW,  MC_L2,  LOW,  rate);  
    motor(EN_R, MC_R1,  LOW,  MC_R2,  LOW,  rate);   
}

void star(int rate = 255)
{
    stop();
    forward();
    delay(2000);
    
    stop();
     forward(1);
     delay(1900);
     stop();
      forward();
    delay(2000);
    
    stop();
    forward(1);
   delay(1900);  
    stop();
      forward();
    delay(2000);
    
    
    stop();
     forward(1);
    delay(1900);   
    stop();
      forward();
    delay(2000);
   
    
    stop();
     forward(1);
     delay(1900);

   //stop();
    // forward(1);
     //delay(1400);
    
     //stop();
       //  forward();
   // delay(2000);
    
     
}

 
 void setup() {
    pinMode(EN_L, OUTPUT);
    pinMode(MC_L1, OUTPUT);
    pinMode(MC_L2, OUTPUT);
    pinMode(EN_R, OUTPUT);
    pinMode(MC_R1, OUTPUT);
    pinMode(MC_R2, OUTPUT);


 }



void loop() {
    
    // put your main code here, to run repeatedly:




   
   //star();
    
    //reverse();
  //delay(1000);
    //forward(0);
    digitalWrite(10,LOW);
    digitalWrite(10,HIGH);
    digitalWrite(5,HIGH);
    delay(2000);
   //forward(1);   
    //delay(1700);
   //stop();
    delay(100000);
}
