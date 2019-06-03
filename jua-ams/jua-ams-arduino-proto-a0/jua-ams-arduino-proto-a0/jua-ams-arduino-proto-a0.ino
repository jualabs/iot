#include <Wire.h>
#include <SdFat.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// pino A4 -> SDA do MPU
// pino A5 -> SCL do MPU
// endereco I2C do MPU6050
const int MPU = 0x68;

// Pino 4 -> TX do Gps
// Pino 3 -> RX do Gps
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

//Variaveis para armazenar valores do GPS e MPU
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// Pino 8 -> CS do SD
// Pino 11 -> MOSI do SD
// Pino 12 -> MISO do SD
// Pino 13 -> SCK do SD
SdFat sdCard;
SdFile dadosAcc;
SdFile dadosGPS;

const uint8_t chipSelect = 10;
const uint16_t period = 60000; // 60000 -> 1 minute
const uint16_t periodMPU = 100;
const uint16_t num_accel_samples = 10;

unsigned long time_now = 0;
unsigned long timeMPU = 0;

struct acc_samples_t {
  uint32_t ts;
  int32_t AcX;
  int32_t AcY;
  int32_t AcZ;
  int32_t Tmp;
  int32_t GyX;
  int32_t GyY;
  int32_t GyZ;
} acc_samples[num_accel_samples];

uint16_t cur_sample = 0;

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void lerGPS() {
  //Serial.println(F("lendo gps..."));
  //smartDelay(1000);
  //if (ss.available() > 0) {
   //Serial.print(F("tem dado..."));
    //gps.encode(ss.read());
    //Serial.println(F("OK!"));
      if (gps.location.isUpdated()) { 
        Serial.println(F("tem localizacao...")); 
        if (!dadosGPS.open("gps.txt", O_RDWR | O_CREAT | O_AT_END)) {
          sdCard.errorHalt(F("Erro na abertura do arquivo DADOS_GPS.TXT!"));
        }

        Serial.print(F("time = ")); Serial.print(gps.time.value()); // Raw time in HHMMSSCC format (u32)
        dadosGPS.print(gps.time.value());

        Serial.print(F(" | millis=")); Serial.print(millis()); 
        dadosGPS.print(";"); dadosGPS.print(millis());
        
        Serial.print(F(" | lat = ")); Serial.print(gps.location.lat(), 6);
        dadosGPS.print(";"); dadosGPS.print(gps.location.lat(), 6);
      
        Serial.print(F(" | lng = ")); Serial.println(gps.location.lng(), 6);
        dadosGPS.print(";"); dadosGPS.println(gps.location.lng(), 6);

        dadosGPS.close();
        
      }
    // }
  }

void lerMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU, 14, true);
    
  //Armazena o valor dos sensores nas variaveis correspondentes
  acc_samples[cur_sample].AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  acc_samples[cur_sample].AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acc_samples[cur_sample].AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  acc_samples[cur_sample].Tmp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  acc_samples[cur_sample].GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  acc_samples[cur_sample].GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  acc_samples[cur_sample].GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  acc_samples[cur_sample].ts = millis();

  Serial.print(F("millis = ")); Serial.print(acc_samples[cur_sample].ts); 
  // envia valor 'x' do acelerometro para a serial
  Serial.print(F(" | acc_x = ")); Serial.print(acc_samples[cur_sample].AcX);
  // envia valor 'y' do acelerometro para a serial
  Serial.print(F(" | acc_y = ")); Serial.print(acc_samples[cur_sample].AcY);
  // envia valor 'z' do acelerometro para a serial
  Serial.print(F(" | acc_z = ")); Serial.print(acc_samples[cur_sample].AcZ);
  // envia valor da temperatura para a serial
  Serial.print(F(" | tmp = ")); Serial.print(acc_samples[cur_sample].Tmp / 340.00 + 36.53);
  // envia valor 'x' do giroscopio para a serial
  Serial.print(F(" | gyr_x = ")); Serial.print(acc_samples[cur_sample].GyX);
  // envia valor 'y' do giroscopio para a serial
  Serial.print(F(" | gyr_y = ")); Serial.print(acc_samples[cur_sample].GyY);
  // envia valor 'z' do giroscopio para a serial
  Serial.print(F(" | gyr_z = ")); Serial.println(acc_samples[cur_sample].GyZ);
  cur_sample++;
  if(cur_sample == 10) {
    writeAccData();
    // reinicializa o buffer temporario
    cur_sample = 0;
  }
}

void writeAccData() {
  if (!dadosAcc.open("acc.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sdCard.errorHalt(F("Erro na abertura do arquivo acc.txt!"));
  }
  Serial.print(F("writing accelerometer data..."));
  // escreve todas as amostras coletadas
  for(uint16_t i = 0; i < cur_sample; i++) {
    dadosAcc.print(acc_samples[i].ts);
    // escreve valor 'x' do acelerometro no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].AcX);
    // escreve valor 'y' do acelerometro no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].AcY);
    // escreve valor 'z' do acelerometro no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].AcZ);
    // escreve valor da temperatura no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].Tmp/340.00+36.53);
    // escreve valor 'x' do giroscopio no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].GyX);
    // escreve valor 'y' do giroscopio no arquivo
    dadosAcc.print(F(";")); dadosAcc.print(acc_samples[i].GyY);
    // escreve valor 'z' do giroscopio no arquivo
    dadosAcc.print(F(";")); dadosAcc.println(acc_samples[i].GyZ);
  }
  Serial.println(F("OK"));
  // fecha o arquivo
  dadosAcc.close();
}

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);

  if(!sdCard.begin(chipSelect,SPI_HALF_SPEED)) {
    sdCard.initErrorHalt();
  }
  // inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  // verifica se passou o tempo entre leituras do GPS e escreve no arquivo
  if (millis() > time_now + period) {
    time_now = millis();
    lerGPS();
  }

  smartDelay(100);
  // verifica se passou o tempo para escrita dos dados temporarios do acelerometro no arquivo
  //if(millis() > timeMPU + periodMPU) {
  //  timeMPU = millis();
    lerMPU();  
  //}  
}
