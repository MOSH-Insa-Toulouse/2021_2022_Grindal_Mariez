# 2021_2022_Grindal_Mariez

Granulometric sensor project

## Objective of the project
> "Design and build a PCB shield including a low-tech graphite strain sensor coupled to an analog electronic circuit that communicate data via a microcontroller to an Android application"

## Main steps
* [Electrical conception and simulation on LTSpice](#electronic-conception-and-test-on-lt-spice)
* [Graphite sensor conception](#graphite-sensor)
* [PCB conception and printing](#pcb-conception-on-kicad)
* [Circuit conception](#application-code)
* [Arduino code]()
* [Test]()
* [Datasheet writing]()

## Electronic conception and test on LT-Spice
> With the LTSPICE simulator, we want to confirm the electrical circuit.
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
> Circuit conception was made on KICAD. After doing the schematic, we draw the PCB with the previous dimensions :
> * Track length = 0.9 mm
> * Isolation width = 0.7 mm
> * Pad dimensions for OLED, Bluetooth module and rotary encoder = 2 mm * 2.5 mm
> * Drill size for OLED, bluetooth module, rotary encoder, AOP and digital potentiometer = 1 mm
> * Pad size for AOP and digital potentiometer = 1.8 mm * 2.5 mm
> * Drill size for resistances and capacities = 2 mm
> * Pad size for resistances and capacities = 0.8 mm

<img src="https://user-images.githubusercontent.com/95586528/160405697-4437e986-bbf9-4ef6-8740-eef16d24775e.png" width="400" />
<img src="https://user-images.githubusercontent.com/95586528/161790320-d46b58c9-d5bd-42f4-93e7-d98bd206e1a1.png" width="400" />

> Then, PCB was printed with different processes :
> * Mask printed on transparent film (PCB plan from KICAD) 
> * The mask is placed on a plate of copper convered with photo-sensitive film. The mask and the plate are insolated with UV. The black part of the mask protects the photo-sensitive resin from UV, and the clear zone of the mask reveals copper beacause photo-sensitive resin reacts with UV.
> * PCB plate is washed with a developper solution (oxydied water) that reveals the PCB circuit
> * PCB plate is placed in oxyde bath to remove the unwanted copper not protected by photo-sensitive zone left.
> * PCB is washed and left to dry.
