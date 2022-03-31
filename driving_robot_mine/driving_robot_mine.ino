#include <SoftwareSerial.h>
#define RX_S 7
#define TX_S 6
SoftwareSerial Esp12s = SoftwareSerial(RX_S, TX_S);  // 소프트웨어 시리얼 포트 사용

// 출력용 함수 만들기
int FILE fOUT = {0,};  PRINTPORT=0;
#define printr (x,y, args...) PRINTPORT=x,  fprintf(&fOUT, y , ##args) // ## 은 토큰화 한다. 토큰??, x : 포트 번호
#define printg (x,y, args...) PRINTPORT=x,  fprintf(&fOUT, y"\r\n" , ##args)  // y: 문자열
#define AtCIPSENDEX(x) printg(3, "AT+CIPSENDEX=0, %d" ,x)  

static int putchr(char c, FILR*stream)
{
    !PRINTPORT ? Serial.write(c) : Esp12s.write(c);  // 물리 시리얼 포트 : 소프트웨어 시리얼 포트
    return 0;    
}

const int EN_L = 10;
const int MC_L1 = 4;
const int MC_L2 = 5;
const int EN_R =9;
const int MC_R1 =2;
const int MC_R2 =  3;

#define motor(en, m1,v1, m2, v2, rate) \
    digitalWrite(en, LOW);\
    digitalWrite(m1, v1);\
    digitalWrite(m2, v2);\  
    analogWrite(en, rate)  // 여기는 ;필요없음


void stop(int side = 0)
{
    switch(side){
        case 1 : motor(EN_L,  MC_L1,  LOW,  MC_L2,  LOW,  0);  break; 
        case 2 : motor(EN_R, MC_R1,  LOW,  MC_R2,  LOW,  0);  break;
        default:  
                 motor(EN_L,  MC_L1,  LOW,  MC_L2,  LOW,  0);  
                 motor(EN_R, MC_R1,  LOW,  MC_R2,  LOW,  0);
    } 
}
    

void forward(int side = 0, int rate = 255)
{
       stop();
       
       switch(side){
        case 2 : motor(EN_L,  MC_L1,  HIGH,  MC_L2,  LOW,  rate);  break;   // 오른쪽
        case 1 : motor(EN_R, MC_R1,  HIGH,  MC_R2,  LOW,  rate);  break;  // 왼쪽
        default:  
                 motor(EN_L,  MC_L1,  HIGH,  MC_L2,  LOW,  rate);  
                 motor(EN_R, MC_R1,  HIGH,  MC_R2,  LOW,  rate);
       }
}

void reverse(int rate = 255)
{
       stop();
       
       motor(EN_L,  MC_L1,  LOW,  MC_L2,  HIGH,  rate);  
       motor(EN_R, MC_R1,  LOW,  MC_R2,  HIGH,  rate);
}

int testLED
 void setup() {\
 // https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
    fdev_setup_stream(&fOUT, putchr, NULL, _FDEV_SETUP_WRITE);  // 메모리 주소, 입력, 출력,플레그

    Serial.begin(115200);
    Esp12s.begin(115200);

    printg(3, "ATE0"); delay(5);  // 명령을 수행하기 위해서 지연이 필요
    printg(3, "AT+CWMODE=3"); delay(5);
    printg(3, "AT+CIPMUX=1"); delay(5);
    printg(3,"AT+CIPSERVER=1, 1009"); delay(5);  // 서버로 사용하겠다
    printg(3, "AT+CIPSTO=0"); delay(5);  // 쉬지 않고 돌아라
    printg(3,"AT+CIPMODE=0"); delay(5);
    printg(0, "setup...");  // 물리적 시리얼에서 셋업이 끝났다 확인하는 용(디버깅을 위해 첨가함)
    
    pinMode(EN_L, OUTPUT);
    pinMode(MC_L1, OUTPUT);
    pinMode(MC_L2, OUTPUT);
    pinMode(EN_R, OUTPUT);
    pinMode(MC_R1, OUTPUT);
    pinMode(MC_R2, OUTPUT);
    stop();

    pinMode(LED_BUILTIN, OUTPUT);
    testLED = HIGH;
    digitalWrite(LED_BUILTIN, testLED);
}

int sonarAlt;
void loop() {
    // put your main code here, to run repeatedly:

    sonarAlt = random(1, 300);

    static int etime = 0;
    int cTime = millis(); // 현제 시간을 밀리초 단위로 가져옴

    // 데이터를 내보냄(0.5초마다)
    if(cTime - eTime>500)
    {
        eTime=cTime;
        printg(0,"sonrAlt: %5d", sonarAlt);

        AtCIPSENDEX(10+5+2); delay(5);
        printg(0,"sonrAlt: %5d", sonarAlt);  delay(10);
        
        testLED = !testLED;
        digitalWrite(LED_BUILTIN, testLED
    }

    while(Esp12s.available()>0)
    {
        String cmd = Esp12s.readStringUntil(13);  // 괄호 안의 값이 읽힐 때까지 읽어들임, 아스키 코드 13 : \r
        cmd.trim()  // +IPD,1,3 : 9
        if(cmd.length()> 0 && cmd.startWith("+IPD"))  // startWith 
        {
            switch(cmd.substring(cmd.indexOf(':')+1).toInt()){
                case '1' :  forward(); break;
                case '2' :  forward(1); break;
                case '3' :  forward(2); break;        
                case '4' :  stop(); break;        
                case '5' :  reverse(); break;          
           }          
        }
    }
#if 0
    if(Serial.available()>0){  // 버퍼에 뭐가 있다는 의미
        int cmd =Serial.read();
        Serial.println(cmd);  // 통신이 잘 되는지 확인
        switch(cmd){
            case '1' :  forward(); break;
            case '2' :  forward(1); break;
            case '3' :  forward(2); break;        
            case '4' :  stop(); break;        
            case '5' :  reverse(); break;        
        }
    }
#endif
}
