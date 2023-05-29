#define HEADER_LIDAR 0X59

int data_lidar[9];
int buffer_lidar;
int prev_buffer_lidar;

int distance;
int strength;
int temperature;
int check_sum;

bool header_detected_lidar = false;
int lidar_index = 0;

void initLidar(){
  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200,SERIAL_8N1);
}

void readLidar(){
  while(Serial1.available()){
    prev_buffer_lidar = buffer_lidar;
    buffer_lidar = Serial1.read();

    if(header_detected_lidar){
      data_lidar[lidar_index] = buffer_lidar;
      lidar_index++;
      if(lidar_index > 8){
        header_detected_lidar = false;
      }
    }
    else{
      if(prev_buffer_lidar == HEADER_LIDAR && buffer_lidar == HEADER_LIDAR){
        data_lidar[0] = HEADER_LIDAR;
        data_lidar[1] = HEADER_LIDAR;
        header_detected_lidar = true;
        lidar_index = 2;
      }
      else{
        header_detected_lidar = false;
      }
    }
    check_sum = (data_lidar[0] + data_lidar[1] + data_lidar[2] + data_lidar[3] + data_lidar[4] + data_lidar[5] + data_lidar[6] + data_lidar[7]) & 0xFF;
    if (check_sum == data_lidar[8]){
      distance = data_lidar[2] + (data_lidar[3] << 8);
      strength = data_lidar[4] + (data_lidar[5] << 8);
      temperature = (data_lidar[6] + (data_lidar[7] << 8))/8 -256;
    }
  }
}


void setup1(){
  initLidar();
}

void loop1(){
  readLidar();
}
