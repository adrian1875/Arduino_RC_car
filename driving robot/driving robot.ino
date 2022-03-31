#include <SoftwareSerial.h>
#define	RX_S 7
#define	TX_S 6
SoftwareSerial Esp12s =	 SoftwareSerial(RX_S, TX_S);  // SoftwareSerial로 사용할 핀을 설정하는 객체 생성 7번 6번 핀을 사용

uint8_t	PRINTPORT =	0; FILE fOUT = {0,};  // 0으로 초기화, 주소를 전달받아 저장한다 

// * 없어도 되나?

// fprintf(FILE* stream, const char* format, ...);  // 데이터를 형식에 맞추어 스트림에 쓴다.
// stream : 작업을 수행할 스트림의 FILE 객체
// format : C 문자열로 스트림에 써질 텍스트를 포함
// x값이 PRINTPORT의 값으로 전달이 된다. fprintf 첫 인자로 6번 라인에서 선언한 파일 포인터가 오고 뒤의 y에 AT명령어가 온다
#define	printr(x, y, args...) PRINTPORT=x,fprintf(&fOUT, y,	##args)  // 전저리기는 공백을 기준으로 치환함
#define	printg(x, y, args...) PRINTPORT=x,fprintf(&fOUT, y"\r\n", ##args)  // ##이 없는 경우 가변인자쪽에 아무것도 쓰지 않으면 컴파일 에러, x는 포트번호, y는 문자열 따라서 %d같은 것 사용 가능

// 초음파 센서로 입력받은 값을 시리얼과 esp로 보내기 위해 길이값을 전달한다
#define	AtCIPSENDEX(x) printg(3, "AT+CIPSENDEX=0,%d", x) 

static int putchr(char c, FILE*	stream)
{
	!PRINTPORT ? Serial.write(c):Esp12s.write(c);  // 비트단위의 데이터를 전송한다, 한번에 하나의 데이터만 전송이 가능, 0과 1만 리턴함
	                                                                        // 시리얼 통신을 쓸지 esp롤 데이터를 보낼지 결정함
	return(0);  // 출력이 성공적이면 0을 반환                                                   
}

#define DIST_S 200*58.2
const int EN_L = 9;
const int MC_L1 = 2;
const int MC_L2 = 3;
const int EN_R =10;
const int MC_R1 =5;
const int MC_R2 = 4;


 // 모터쪽으로 값을 보내줌, 함수식으로 불러서 중간에는 ;이 필요 제일 마지막은 있어고 그만 없어도 그만
#define	motor(en, m1, v1, m2, v2, rate)\ 
	digitalWrite(en, LOW);\
	digitalWrite(m1, v1);\
	digitalWrite(m2, v2);\
	analogWrite(en,	rate)

void stop (int side=0)  // 모터쪽의 신호를 모두 0으로 만들어서 모터를 정지시킴
{
	switch(side){  // 인자로 무엇이 들어와도 모두 정지신호를 보냄, 브릿지 회로의 특성상 시작전에 인에이블 신호를 off시켜서 단락의 위험(쇼트)을 막는다 
	case 1:	motor(EN_L,	MC_L1, LOW,	MC_L2, LOW,	0);	break;
	case 2:	motor(EN_R,	MC_R1, LOW,	MC_R2, LOW,	0);	break;
	default:
		motor(EN_L, MC_L1, LOW, MC_L2, LOW, 0);
		motor(EN_R, MC_R1, LOW, MC_R2, LOW, 0);
	}
}

void forward (int side=0, int rate=255)  // 전진, 좌우회전 신호를 모터로 보냄
{
	stop();  // (움직임의)최적화를 위해 정지를 함
	switch(side){
	case 1:	motor(EN_L,	MC_L1, HIGH, MC_L2,	LOW, rate);	break;  // 우선적으로 off시켰던 인에이블에 rate 값으로 최대 속도인 255를 줍니다. 
	case 2:	motor(EN_R, MC_R1, HIGH, MC_R2, LOW, rate);	break;
	default:
		motor(EN_L, MC_L1, HIGH, MC_L2, LOW, rate);
		motor(EN_R, MC_R1, HIGH, MC_R2, LOW, rate);
	}
}

void reverse (int side=0,int rate=255)  // 후진 신호를 줌
{

    stop();  // (움직임의)최적화를 위해 정지를 함
    switch(side){
    case 1: motor(EN_L, MC_L1, LOW, MC_L2, HIGH , rate); break;  // 우선적으로 off시켰던 인에이블에 rate 값으로 최대 속도인 255를 줍니다. 
    case 2: motor(EN_L, MC_L1, LOW, MC_L2, HIGH , rate); break;   
    default:
        motor(EN_L, MC_L1, LOW, MC_L2, HIGH, rate);
        motor(EN_R, MC_R1, LOW, MC_R2, HIGH, rate);
    }
}

// sonar를 위한 핀 세팅
int	TRIG1 =	13;
int	ECHO1 =	12;
void Sonar_init()
{
	pinMode(TRIG1, OUTPUT);
	pinMode(ECHO1, INPUT);
}

void star(int rate = 255)
{
    stop();
    forward();
    delay(2000);
    
    stop();
     forward(1);
     delay(1700);
     stop();
      forward();
    delay(2000);
    
    stop();
    forward(1);
   delay(1700);  
    stop();
      forward();
    delay(2000);
    
    
    stop();
     forward(1);
    delay(1700);   
    stop();
      forward();
    delay(2000);
   
    
    stop();
     forward(1);
     delay(1700);
    stop();
   //stop();
     forward();
     //delay(1400);
    
     //stop();
       //  forward();
   // delay(2000);
    
    
}

float sonarAlt = 0, son_result=0;
void Sonar_update()
{
	float duration, distance;
	digitalWrite(TRIG1,	HIGH);  // 10ms단위로 구형파 트리거 신호를 쏜다
	//delayMicroseconds(10);
	delay(10);
	digitalWrite(TRIG1,	LOW);
                                                                           
	duration = pulseIn(ECHO1, HIGH,DIST_S);  // 타이머를 사용하여 값을 읽어들임, 펄스가 나갔다 돌아오는 시간을 의미. HIGH면 핀이 HIGH가 될 때 까지 기다리다 타이머를 시작함 
	                                                       // 마지막에 입력하지 않은 timeout변수가 하나 있는데 이는 펄스를 기다릴 시간을 의미한다
	distance = ((float)(340.0 *	duration) /	10000.0) / 2.0;  // 거리 계산 공식 2.0은 왕복 거리를 즉정하기 때문에 2로 나는 것, 초와 미터를 마이크로 초와 cm로 바꾸기 위해서 10000으로 나눔
	sonarAlt = distance;
    
    float sum=0.0, avg=0.0;
    int  cnt=0;
    while(cnt<5)
    {
        if(sonarAlt>2000)
        {
            continue;   
        }
        else
        {
             sum += sonarAlt ;
             cnt++;
             break;
        }   
    }
    
    sonarAlt = (sum/5);
    Serial.println("AUTO");
    Serial.println(sonarAlt);
    cnt = 0;
    sum = 0;
  
      if(sonarAlt<10.0)
      {
        reverse();
          delay(1000);
           reverse(1);
           delay(1000);  
           stop();
         //Serial.println("AUTO");
         //Serial.println(sonarAlt);
         printg(0, "sonarAlt: %5d", sonarAlt);
   }
           stop();
         // Serial.println(sonarAlt);
    
}

int	testLED;//LED_BUILTIN(13)
void setup() {
	fdev_setup_stream(&fOUT, putchr, NULL, _FDEV_SETUP_WRITE);  
    // 사용자 제공 버퍼를 stdio 스트림으로 설정
    // 사용자 제공 버퍼 스트림을 사용, stdio 작업에 유효한 스트림으로 설정
    // 설정할 버퍼는 FILE 형식이어야 한다.
    // Put 함수 포인터가 제공되면 스트림이 쓰기 의도로 열립니다.
    // put으로 전달된 함수는 두 개의 인수를 가져야 하는데, 
    // 첫 번째는 장치에 쓸 문자이고 두 번째는 FILE에 대한 포인터이며,
    // 출력이 성공적이면 0을 반환하고, 문자를 장치로 보낼 수 없으면 0이 아닌 값을 반환한다.
    
    // fprintf를 사용하기 위해서 씀
    // &fOUT에 입력 할 때                                                                                                                                                                                                                                                                                               utchr이걸 써라 쓸 때 내가 정의한대로 쓴다 
    // printg와 printr 함수를 사용하기 위해서 필요 (초기화 한다)                                                                         
    // 시리얼로 데이터를 쓰기 위해서(사용자에게 보여주기 위해서) fdev_setup_stream함수가 필요함
    // 데이터를 보내기 위해서는 데이터 스트림이 필요한데 이는 FILE형태의 구조체이다
    // 보내기 위해 FILE 그리고 읽고 쓰기위한 함수의 포인터가 필요하다.
    // 초기화 하면 이후에 printr이나 printg를 사용할 수 있다

	// put your setup	code here, to run once:
	Serial.begin(115200);  // 시리얼 보우레이트를 설정
	//Esp12s.begin(9600);
	//printg(3, "AT+UART_DEF=115200,8,1,0,0");delay(5);
	Esp12s.begin(115200); // esp 보우레이트를 설정

    /// 0만 아니면 esp로 데이터를 전송, 0은  물리적 시리얼 포트에 전송
	printg(3, "ATE0");delay(5);  // 에코를 설정
	printg(3, "AT+CWMODE=3");delay(5);  // 와이파이 설정 1: Station(클라이언트) mode, 2: AP(서버) mode(외부에서 오는 것을 바로 시리얼로 보냄, 외부 명령을 바로 보내줌 ),  3: station + AP mode
	//printg(3, "AT+CWSAP=\"poly\",\"1234\",1,2"); delay(5);
	printg(3, "AT+CIPMUX=1"); delay(5);  // 다중 접속을 설정, 0: 싱글연결, 1 : 복수 연결
	printg(3, "AT+CIPSERVER=1,2726"); delay(5);  // TCP 서버 모드로 설정, 0: 서버를 삭제, 1: 서버를 시작, 2726: 내가 설정한 포트번호, 초기값은 333
	printg(3, "AT+CIPSTO=0"); delay(5);  // 통신모듈이 TCP서버로 동작시, 타임아웃 기간을 설정,  범위 0~7200 초
	printg(3, "AT+CIPMODE=0"); delay(5);  // 전송모드를 설정, 0: 일반모드, 1: 전송모드
	printg(0, "setup...");  // 시리얼에 디버깅을 위한 setup...을 출력

    // 모터 출력을 위한 핀 세팅
	pinMode(EN_L, OUTPUT);
	pinMode(MC_L1, OUTPUT);
	pinMode(MC_L2, OUTPUT);
	pinMode(EN_R, OUTPUT);
	pinMode(MC_R1, OUTPUT);
	pinMode(MC_R2, OUTPUT);
	stop();

	//pinMode(RX_S, INPUT);
	//pinMode(TX_S, OUTPUT);

	Sonar_init();

    // 아두이노 동작 확인을 위한 빌트인 LED설정
	pinMode(LED_BUILTIN, OUTPUT);
	testLED = HIGH;
	digitalWrite(LED_BUILTIN, testLED);
}

void loop()	{
  // put your main code	here, to run repeatedly:

   
   
    
	//printr(0, ".");
	Sonar_update();
	//sonarAlt = random(0, 5);

    

          
	static uint16_t	eTime =	0;
	uint16_t cTime = millis();
	if(cTime - eTime > 500){
		eTime =	cTime;

		AtCIPSENDEX(10 + 5 + 2); delay(5);  // 정해진 갯수만큼  문자를 보낼 거니까 길이를 알려줘요!! (여기서는 총 17자)
		printg(3, "sonarAlt: %5d", sonarAlt); delay(10);  // 초음파 센서에서 입력받은 sonarAlt(가변 인자) 값을 ESP로 보낸다, 

        printg(0, "sonarAlt: %5d", sonarAlt);  // 초음파 센서에서 입력받은 sonarAlt(가변 인자) 값을 시리얼로 보낸다.

		//printr(0,	".");
		testLED	= !testLED;	
		digitalWrite(LED_BUILTIN, testLED);
	}
#if 1
	while(Esp12s.available() > 0){  // 수신 버퍼에 뭐가 있나? 확인
		String cmd = Esp12s.readStringUntil(13);  // 아스키 코드로 13은 carriage return 
		cmd.trim();//+IPD,1,3:9
#if 0// 디버딩용, sw시리얼 포트에서 무엇이 들어오고 있다면 보드가 살아있는지 확인할 수 있는 코드(esp에서 아두이노로 보내는 것을 확인해서 esp와의 연결을 확인)
		printg(0, "recevie: %s", cmd.c_str());
#endif
		if(cmd.length()	> 0	&&  cmd.startsWith("+IPD"))  //  의미없이 들어오는 것을 걸러냄, cmd.length 자주씀
		{
#if 1
			printg(0, "revCmd: %s", cmd.substring(cmd.indexOf(':')+1).c_str());
#endif
			switch(cmd.substring(cmd.indexOf(':')+1).toInt()){
			case 1: forward(); break;
			case 2: forward(1); break;
			case 3: forward(2); break;
			case 4: stop(); break;
			case 5: reverse(); break;
            case 6: star(); break;
            //case 7: Auto(); break;
			}
		}
	}
#else
	if (Serial.available() > 0) {
		int cmd = Serial.read();
		//Serial.println(cmd);
		switch(cmd){
		case '1': forward(); break;
		case '2': forward(1); break;
		case '3': forward(2); break;
		case '4': stop(); break;
		case '5': reverse(); break;
        case '6': star(); break;
       // case '7': Auto(); break;
		}
	}
#endif
}
