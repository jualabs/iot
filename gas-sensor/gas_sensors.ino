#include "DHT.h"

#define DHTTYPE DHT11

#define PIN_DHT11 A5
#define PIN_MQ2 A0 //define pin sensor MQ-2

#define VRL_VALOR 5 //resistência de carga
#define RO_FATOR_AR_LIMPO 9.83 //resistência do sensor em ar limpo 9.83 de acordo com o datasheet
                                                     
#define ITERACOES_CALIBRACAO 50    //numero de leituras para calibracao
#define ITERACOES_LEITURA 5     //numero de leituras para analise

#define GAS_LPG 0  
#define GAS_CO 1
#define SMOKE 2

DHT dht(PIN_DHT11, DHTTYPE);

float LPGCurve[3]  =  {2.3,0.20,-0.47};   //curva LPG aproximada baseada na sensibilidade descrita no datasheet {x,y,deslocamento} baseada em dois pontos 
                                          //p1: (log200, log1.6), p2: (log10000, log0.26)
                                          //inclinacao = (Y2-Y1)/(X2-X1)
                                          //vetor={x, y, inclinacao}

float COCurve[3]  =  {2.3,0.72,-0.34};    //curva CO aproximada baseada na sensibilidade descrita no datasheet {x,y,deslocamento} baseada em dois pontos 
                                          //p1: (log200, 0.72), p2(log10000, 0.15)
                                          //inclinacao = (Y2-Y1)/(X2-X1)
                                          //vetor={x, y, inclinacao}
                                                   
                                                    
float SmokeCurve[3] ={2.3,0.53,-0.44};    //curva LPG aproximada baseada na sensibilidade descrita no datasheet {x,y,deslocamento} baseada em dois pontos 
                                          //p1: (log200, 0.53), p2: (log10000, -0.22)
                                          //inclinacao = (Y2-Y1)/(X2-X1)
                                          //vetor={x, y, inclinacao}
                                          
float Ro = 10; 

void setup()
{
  Serial.begin(9600);                             
  Serial.print("Calibrando o sensor MQ2...\n");                
  Ro = MQCalibration(PIN_MQ2);      //calibra o sensor MQ2                                  
  Serial.print("Finalizando a calibracao...\n"); 
  Serial.print("Calibracao concluida...\n"); 
  Serial.print("Valor de Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
}

void loop()
{
   Serial.print("Temperatura: ");
   Serial.print(dht.readTemperature());
   Serial.print("ºC");
   Serial.println(" ");
   Serial.print("Humididade: ");
   Serial.print(dht.readHumidity());
   Serial.println("%");
   Serial.println("");
     
   Serial.print("LPG:"); 
   Serial.print(getQuantidadeGasMQ(leitura_MQ2(PIN_MQ2)/Ro,GAS_LPG) );
   Serial.print( "ppm     " );
   Serial.print("CO:"); 
   Serial.print(getQuantidadeGasMQ(leitura_MQ2(PIN_MQ2)/Ro,GAS_CO) );
   Serial.print( "ppm    " );
   Serial.print("SMOKE:"); 
   Serial.print(getQuantidadeGasMQ(leitura_MQ2(PIN_MQ2)/Ro,SMOKE) );
   Serial.print( "ppm    " );
   Serial.println("");
   delay(200);
}

float calcularResistencia(int tensao)   //funcao que recebe o tensao (dado cru) e calcula a resistencia efetuada pelo sensor. O sensor e a resistência de carga forma um divisor de tensão. 
{
  return (((float)VRL_VALOR*(1023-tensao)/tensao));
}

float MQCalibration(int mq_pin)   //funcao que calibra o sensor em um ambiente limpo utilizando a resistencia do sensor em ar limpo 9.83
{
  int i;
  float valor=0;

  for (i=0;i<ITERACOES_CALIBRACAO;i++) {    //sao adquiridas diversas amostras e calculada a media para diminuir o efeito de possiveis oscilacoes durante a calibracao
    valor += calcularResistencia(analogRead(mq_pin));
    delay(500);
  }
  valor = valor/ITERACOES_CALIBRACAO;        

  valor = valor/RO_FATOR_AR_LIMPO; //o valor lido dividido pelo R0 do ar limpo resulta no R0 do ambiente

  return valor; 
}

float leitura_MQ2(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<ITERACOES_LEITURA;i++) {
    rs += calcularResistencia(analogRead(mq_pin));
    delay(50);
  }

  rs = rs/ITERACOES_LEITURA;

  return rs;  
}

int getQuantidadeGasMQ(float rs_ro, int gas_id)
{
  if ( gas_id == 0 ) {
     return calculaGasPPM(rs_ro,LPGCurve);
  } else if ( gas_id == 1 ) {
     return calculaGasPPM(rs_ro,COCurve);
  } else if ( gas_id == 2 ) {
     return calculaGasPPM(rs_ro,SmokeCurve);
  }    

  return 0;
}

int  calculaGasPPM(float rs_ro, float *pcurve) //Rs/R0 é fornecido para calcular a concentracao em PPM do gas em questao. O calculo eh em potencia de 10 para sair da logaritmica
{
  return (pow(10,( ((log(rs_ro)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

