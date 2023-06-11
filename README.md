# Multilateration of Acoustic Sources

This project uses 3 microphones to locate sound-emitting objects in 2D.
As seen below, the sound source recorded by 2 microphones can lie on any point on a hyperbola that represents time-difference-of-arrival (TDoA) between them.
<img width="222" alt="image" src="https://github.com/MA12CO/Multilateration-of-acoustic-sources/assets/41839731/29a3df4b-4d35-41d8-a09e-b2f57f79e38e">

using 3 microphones create 2 hyperbolae that ideally intersect in the acoustic source location:
![noise periodic corr](https://github.com/MA12CO/Multilateration-of-acoustic-sources/assets/41839731/1c4e6011-b236-4134-8903-edb2931b1ccf)

I used 3 Adafruit™ MAX9814 electret microphone amplifiers attached to 3 ESP8266 development boards to create the microphone array. 
The nodes analyse sounds and once the wanted frequency is detected (using FFT). The nodes send their recorded sound signals to the PC to be processed via UDP protocol.

The samples are constantly recorded in a circular buffer (using a circular buffer library).

![image](https://github.com/MA12CO/Multilateration-of-acoustic-sources/assets/41839731/7a7a0d22-d43e-4649-ad1b-1d1e82876aed)

After syncing, the signals are cross-correlated to find the time-difference-of-arrival.
![noise periodic corr](https://github.com/MA12CO/Multilateration-of-acoustic-sources/assets/41839731/53cd19ef-a40c-4ae8-b908-f69c249e7cbd)

Based on TDoAs between the following pairs: mic1 - mic2, mic2 - mic3, mic1 - mic3, the trilateration algorithm finds the x and y coordinated of the acoustic object.

## Hardware and connection:
![image](https://github.com/MA12CO/Multilateration-of-acoustic-sources/assets/41839731/e2d58af8-f4d4-4d10-bafe-cfcb40fb5ac9)


