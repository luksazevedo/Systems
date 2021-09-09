#include <SimpleModbusSlave.h>
#include <Thermistor.h>  //Biblioteca de temperatura
#include <PID_v1.h> // Biblioteca de controle



//#define  OUTPUTS 
#define PIN_OUTPUT 5
#define OUT_A1_PIN 7
#define OUT_B1_PIN 8



//////////////// registradores ///////////////////
enum 
{     
  
  Leitura,     
  Comand_PWM,
  Valor_Elipselig,
  Setpoint_Elipse,
  KP_Elipse,
  Ki_Elipse,
  Kd_Elipse,
          
  HOLDING_REGS_SIZE 
  
};

unsigned int  holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////



void setup()
{
  

  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);

  
  modbus_update_comms(9600, SERIAL_8N1, 1);
  
  pinMode(OUT_A1_PIN, OUTPUT);
  pinMode(OUT_B1_PIN, OUTPUT);
  pinMode(PIN_OUTPUT, OUTPUT);

 
  //pinMode(5, OUTPUT);//pino para sinal pwm ponte-h

  
}

void loop()
{


  modbus_update();
  double Setpoint, Input, Output;
  double Kp,Ki,Kd;
  //double Kp=6.62, Ki=0.389, Kd=0;
  Setpoint = holdingRegs[Setpoint_Elipse];
  double Kp = holdingRegs[KP_Elipse];
  double Ki = holdingRegs[Ki_Elipse];
  double Kd = holdingRegs[Kd_Elipse];
  
  PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);



  //initializa as variaveis
  Thermistor temp(0); 
  Input = temp.getTemp();
  //Setpoint = holdingRegs[Setpoint_Elipse];

  // PID on
  myPID.SetMode(AUTOMATIC)
  
  
  holdingRegs[Leitura] = temp.getTemp(); 
 
  Input = temp.getTemp();

  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
  if(holdingRegs[Valor_Elipselig]==1){
     digitalWrite(OUT_A1_PIN, HIGH);
     digitalWrite(OUT_B1_PIN, LOW);
     
  }
  else{
    digitalWrite(OUT_A1_PIN, LOW);
    digitalWrite(OUT_B1_PIN, LOW);
    analogWrite(PIN_OUTPUT,0);
  }
 
  
   
}
