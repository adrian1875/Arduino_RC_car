const int EN_L = 9;
const int MC_L1 = 2;
const int MC_L2 = 3;
const int EN_R =10;
const int MC_R1 =4;
const int MC_R2 =  3;

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

void stop (int   side=0)  // 모터쪽의 신호를 모두 0으로 만들어서 모터를 정지시킴
{
    switch(side){  // 인자로 무엇이 들어와도 모두 정지신호를 보냄, 브릿지 회로의 특성상 시작전에 인에이블 신호를 off시켜서 단락의 위험(쇼트)을 막는다 
    case 1: motor(EN_L, MC_L1, LOW, MC_L2, LOW, 0); break;
    case 2: motor(EN_R, MC_R1, LOW, MC_R2, LOW, 0); break;
    default:
        motor(EN_L, MC_L1, LOW, MC_L2, LOW, 0);
        motor(EN_R, MC_R1, LOW, MC_R2, LOW, 0);
    }
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
stop();
    delay(1000);
    
} 
