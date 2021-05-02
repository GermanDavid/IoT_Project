int val = 0, count = 0
int arr[] = new int [144]
long last_time = millis(), ten_min = 1000 * 60 * 10
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if (count == 144){
    // finish day
    count = 0;   
  }
  if(millis() - last_time >= ten_min){
    arr[count++] = val;// time stamp , vat, num_appl...
    last_time = millis();
  }
}
