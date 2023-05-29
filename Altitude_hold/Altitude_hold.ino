#define SBUS_HEADER 0X0F
#define SBUS_FOOTER 0X00

int buffer_sbus;
int prev_buffer_sbus;
int data_rx[20];
int data_tx[25];
int channels[16];
bool aux1;
bool aux2;
bool frame_lost;
bool failsafe;

int rx_index;
bool header_detected_sbus = 0;

void setup() {
  Serial.begin(9600);
  initSbus();
}

void loop() {
  readSbus();
    Serial.println(channels[0]);
    Serial.println(channels[1]);
    Serial.println(channels[2]);
    Serial.println(""); 
}

void initSbus(){
  Serial2.setRX(5);
  Serial2.setTX(4);
  Serial2.begin(100000,SERIAL_8E2);
}

void readSbus(){
  if(Serial2.available()){
    prev_buffer_sbus = buffer_sbus;   
    buffer_sbus = Serial2.read();
    if(!header_detected_sbus){
      if(buffer_sbus == 0x0F && prev_buffer_sbus == SBUS_FOOTER){
        data_rx[0] = SBUS_HEADER;
        data_rx[24] = SBUS_FOOTER;
        header_detected_sbus = true;
        rx_index = 1;
      }
    }
    else{
      data_rx[rx_index] = buffer_sbus;
      rx_index++;
      if(rx_index > 23){
        header_detected_sbus = 0;
      }
    }
  }
      channels[0] = ((data_rx[1] | data_rx[2] << 8) & 0x07FF);
      channels[1] = ((data_rx[2] >> 3 | data_rx[3] << 5) & 0x07FF);
      channels[2] = ((data_rx[3] >> 6 | data_rx[4] << 2 | data_rx[5] << 10) & 0x07FF);
      channels[3] = ((data_rx[5] >> 1 | data_rx[6] << 7) & 0x07FF);
      channels[4] = ((data_rx[6] >> 4 | data_rx[7] << 4) & 0x07FF);
      channels[5] = ((data_rx[7] >> 7 | data_rx[8] << 1 | data_rx[9] << 9) & 0x07FF);
      channels[6] = ((data_rx[9] >> 2 | data_rx[10] << 6) & 0x07FF);
      channels[7] = ((data_rx[10] >> 5 | data_rx[11] << 3) & 0x07FF);
      channels[8] = ((data_rx[12] | data_rx[13] << 8) & 0x07FF);
      channels[9] = ((data_rx[13] >> 3 | data_rx[14] << 5) & 0x07FF);
      channels[10] = ((data_rx[14] >> 6 | data_rx[15] << 2 | data_rx[16] << 10) & 0x07FF);
      channels[11] = ((data_rx[16] >> 1 | data_rx[17] << 7) & 0x07FF);
      channels[12] = ((data_rx[17] >> 4 | data_rx[18] << 4) & 0x07FF);
      channels[13] = ((data_rx[18] >> 7 | data_rx[19] << 1 | data_rx[20] << 9) & 0x07FF);
      channels[14] = ((data_rx[20] >> 2 | data_rx[21] << 6) & 0x07FF);
      channels[15] = ((data_rx[21] >> 5 | data_rx[22] << 3) & 0x07FF);
      aux1 = (data_rx[23] & 0x01);
      aux2 = ((data_rx[23] & 0x02) >> 1);
      frame_lost = ((data_rx[23] & 0x04) >> 2);
      failsafe = ((data_rx[23] & 0x08) >> 3);
}
