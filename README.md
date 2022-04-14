# 2021_2022_Grindal_Mariez

## Objective of the project
> The objective of the projetc is to design and build a PCB shield including a low-tech graphite strain sensor coupled to an analog electronic circuit that communicate data via a microcontroller to an Android application. The aim of this project is to get the resistance from a graphite sensor to measure the deformation, to analyze the results obtained and to describe the characteristics of the sensor in a datasheet.

## Main steps of the project
* [Graphite sensor conception](#graphite-sensor)
* [Electrical conception and simulation on LTSpice](#electronic-conception-and-test-on-lt-spice)
* [PCB conception and printing](#pcb-conception-on-kicad)
* [Arduino code](#arduino-code)
* [Application ANDROID](#application-android)
* [Test bed and critical analysis](#test-bed-and-critical-analysis)
* [Datasheet](#datasheet)

## Graphite sensor conception
> The first step is to make the graphite sensor. We just cut a piece of paper and put some graphite on it with a 2B pencil as you can see on the image below.
> <p align="center">
> <img src="https://user-images.githubusercontent.com/95586528/163388353-763bedf1-8f6d-4edf-8d04-aa12be2c5277.jpg" width="350" />
    
## Electronic conception and test on LT-Spice
> The second step is to build the electrical circuit adapted to our case. The aim of this part is to chose the good components to have the good filters and amplifiers because the signal from the sensor is very low and can be interfered by noise (from the circuit or external noise). We also have to take into account the chracteritic of the arduino uno card we are gonna use:
> * Resolution 10 to 12 bits (1024 à 4096 points)
> * Full scale (VRef) : 1.1 V to 5.0 V
> * Max source impedance 1 kOhm to 10 kOhm
> * Max sampling frequency 15 kHz to 2.4 MHz
> 
> Graphite sensor is supplied by 5V and gives resistance values between 20MΩ and 100MΩ. Direct measurement of a 100 nA current does not seem feasible. We have to amplifiy the signal and to filter it.
> The final electrical circuit chosen is the following:
> <p align="center">
> <img src="https://user-images.githubusercontent.com/95586528/163395406-638d8381-9cf3-4b6e-bb71-4973761aaf2c.png" width="500" />
> 
> The circuit characteritics are: 
> * R5 at the input protects the operational amplifier against electrostatic discharges (ESD), and together with C1 makes up a filter for voltage noises
> * C1 with R1 makes up a filter for the current noise
> * R2 is replaceable, in order to allow an adaptaion of the full scale range (digital potentiometer)
> * C4 with R3 makes up the active filter
> * C2 with R6 makes up the passive output filter
> * C3 filters out the power supply noise
> 
> Sensor resistance can be calculated by the equation below:
> <p align="center">
> <img src="https://user-images.githubusercontent.com/95586528/163400193-6fde259e-b5ec-43f1-a274-3244ab76b3d7.png" width="250" />
> 
> For each 
>
>
>
> Check the operation in nominal conditions
> 
> Impact of the LTC1050C amplifier offset acceptable
> 
> Cutoff frequency of each low-pass filter stage 
> 
> Global attenuation of a 50 Hz current noise
> 
> Global attenuation of a current noise occurring at the aliasing limit frequency (in the case of Arduino Uno ADC)
> 
> Simulation with :
> * a sensor, supplied with 5 volts, the conductance of which varies from 10 nS to 20 nS in 0.5 s
> * a 230V RMS, 50Hz noise source, coupled with the input node through a 1pF capacitor
>
<img src="https://user-images.githubusercontent.com/95586528/160655085-99379685-04d5-4e0f-9d88-0ecf0dfbc2ce.png" width="1000" />

    
## Graphite sensor
> explain
> 
## Arduino code
* **Amplifying circuit with precision zero drift op-amp LTC 1050**
    > explain
* **I2C OLED display SBC-OLED01**
    > explain
* **Bluetooth module HC-05** 
    > explain
* **Digital potentiometer MCP41050**
    > explain
* **Rotary encoder KY-040**
    > test
## Application code
> The Android application is made with the web application integrated development environment MIT App Inventor.
> The app communicates over bluetooth with the HC-05 module mounted on the Arduino shield, and allows to measure
> and trace resistance and voltage over time.
> Main features:
> * Display measured voltage and resistance numerically and traced as a function of time
> * Update gain if modified on the shield
> * Save measured data points in a text file found in the paths "/savedRfile.txt" and "/savedVfile.txt"
> 
## PCB conception on KiCad
> Once the electrical circuit is done, we have to make the PCB conception to get the final circuit. First, we did the schematic into KICAD. Then, we build the PCB plane by adding the references and dimensions of each components. The final step is to print the PCB card with different processes.
> Circuit conception was made on KICAD. After doing the schematic, we draw the PCB with the previous dimensions :
> * Track length = 0.9 mm
> * Isolation width = 0.7 mm
> * Pad dimensions for OLED, Bluetooth module and rotary encoder = 2 mm * 2.5 mm
> * Drill size for OLED, bluetooth module, rotary encoder, AOP and digital potentiometer = 1 mm
> * Pad size for AOP and digital potentiometer = 1.8 mm * 2.5 mm
> * Drill size for resistances and capacities = 2 mm
> * Pad size for resistances and capacities = 0.8 mm

<img src="https://user-images.githubusercontent.com/95586528/160405697-4437e986-bbf9-4ef6-8740-eef16d24775e.png" width="500" > <img src="https://user-images.githubusercontent.com/95586528/161790320-d46b58c9-d5bd-42f4-93e7-d98bd206e1a1.png" width="500" >

> Then, PCB was printed with different processes :
> * Mask printed on transparent film (PCB plan from KICAD) 
> * The mask is placed on a plate of copper convered with photo-sensitive film. The mask and the plate are insolated with UV. The black part of the mask protects the photo-sensitive resin from UV, and the clear zone of the mask reveals copper beacause photo-sensitive resin reacts with UV.
> * PCB plate is washed with a developper solution (oxydied water) that reveals the PCB circuit
> * PCB plate is placed in oxyde bath to remove the unwanted copper not protected by photo-sensitive zone left.
> * PCB is washed and left to dry.

## Application ANDROID
    
## Test bed and critical analysis
    
## Datasheet
