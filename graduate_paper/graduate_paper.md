UVU MCS Graduate Paper Template
CANDIDATE FIRST LAST, Candidate
ADVISOR FIRST LAST, Advisor
Your abstract here: 250 words or less
(1) What problem are you trying to solve?
(2) why is it interesting?
(3) What are your major results?
(4) What are your conclusions?
(5) What did you learn?
(6) How many lines of code is your project?

# UVU MCS Graduate Paper
### Candidate: Benjamin Stoneking
### Advisor: Dr. Frank Jones

# Abstract
The project focuses on implementing a real time digital audio synthesizer from the ground up on a micro controller. The aim is to leverage all the strengths of the microcontroller such as interupt service routines, timers and hardware interupts to produce a system capable of acting as a reliable musical instrument for the user. The topic is interesting as an exercise in implementing an audio data processing pipeline that involves many signal processing components such as oscillators, pitch control, gain mixing and attenuation, envelope processing and filtering to produce a variety of different sounds. The project represents building a complex system capable of being embedded in a limited processing space with options to extend its capabilities. The final project yielded a synthesizer meeting the fundamental requirements to be called a subtractive synthesizer: It can produce sound based off input from a midi instrument, can manipulate the signal gain to create an instrument capable volume dynamics. It can process the audio through high and lowpass filters to change the timbre to sound similar to real world instruments like guitars, pianos and string instruments and it has a hardware interface consisting of knobs, digital encoders and screen allowing the user to directly manipulate the sounds in real time. A computer software accessory application can communicate with the system to be able to adjust parameters, save and load user configurations for later use. Although the project meets minimum requirements for being a synthesizer, it has much work to be done. Many lessons were learned: the process of prototyping and implementing hardware peripherals can be fraught with frustrating bugs and failures. Embedded low level software has a much more involved and timeconsuming process for debugging, with regression tests being an absolute necessity. A lot of the funcionality of electronic musical instruments that we expect is a harder system to implement that expected. The lines of code for this system ended up around 2250




