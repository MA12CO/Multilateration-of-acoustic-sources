// Communication Variables and Declarations 
#include <ESP8266WiFi.h> 
#include "ESPAsyncUDP.h" 
//WiFi Credentials 
const char * ssid = "*****"; 
const char * password = "*****"; 
const char * id = "m1"; 
AsyncUDP udp; 


// Circular Buffer 
#include <CircularBuffer.h> 
#define SAMPLES 2048 CircularBuffer<int, SAMPLES> Buffer; 

// FFT Variables and Declarations 
#include <arduinoFFT.h> 
#define FFTSAMPLES 256 
#define SAMPLING_F 8000 
arduinoFFT FFT = arduinoFFT(); 
unsigned long microSeconds; 
double vReal[FFTSAMPLES]; 
double vImag[FFTSAMPLES]; 
int i; 

// The frequency range of interest
int Fmin = 800; 
int Fmax = 900; 


// Sampling Variables and Declarations 
unsigned int samplingPeriod = round(1000000/SAMPLING_F); 
char payload[256];

 
#define LED D0 

void setup(){
    Serial.begin(115200); 
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) { 
        Serial.println("WiFi Failed"); 
        while(1) { 
            delay(1000); 
            } 
        } 

    if(udp.connect(IPAddress(192,168,1,100), 1234)) { 
        Serial.println("UDP connected"); 
        Serial.print("UDP Listening on IP: "); 
        Serial.println(WiFi.localIP()); 
        udp.onPacket([](AsyncUDPPacket packet) { }); 
        } 

    pinMode( LED, OUTPUT); 

    delay(3000); 
} 


void loop(){ 
    if ( FFTDetect() > Fmin && FFTDetect() < Fmax ){ 
        recordSample(); 
        Serial.print("frequency detected is: "); 
        Serial.println(FFTDetect()); 
        Serial.println("sending to server..."); 
        sendBuffer(); 
    } 
    
    digitalWrite(LED,LOW); 
} 


double FFTDetect(){
    for (i = 0; i < FFTSAMPLES; i++) { 
        microSeconds = micros(); 
        vReal[i] = analogRead(A0); 
        vImag[i] = 0; 
        while (micros() < (microSeconds + samplingPeriod)) { 
        } 
    }

    FFT.Windowing(vReal, FFTSAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); 
    FFT.Compute(vReal, vImag, FFTSAMPLES, FFT_FORWARD); 
    FFT.ComplexToMagnitude(vReal, vImag, FFTSAMPLES); 
    double peak = FFT.MajorPeak(vReal, FFTSAMPLES, SAMPLING_F); 
    Serial.println(peak); 
    return peak; 
} 
    

void recordSample(){ 
    for (i = 0; i < SAMPLES; i++) { 
        microSeconds = micros(); 
        normalBuffer.push(analogRead(A0)); 

        while (micros() < (microSeconds + samplingPeriod)) {
        } 
    } 
} 


void sendBuffer(){ 
    for (i = 0; i < SAMPLES; i++) { 
        //Writing data to a comma-separated string 
        sprintf(payload, "%s , %f", id, normalBuffer[i]); 
        //send via UDP tp port 1234 
        udp.broadcastTo(payload, 1234);
        delay(50); 
    } 
    
    digitalWrite(LED,HIGH); 
    delay(10000); 
    digitalWrite(LED,LOW); 
}