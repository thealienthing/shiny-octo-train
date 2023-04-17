# UVU MCS Graduate Paper
### Candidate: Benjamin Stoneking
### Advisor: Dr. Frank Jones

# Abstract
The project focuses on implementing a real time digital audio synthesizer from the ground up on a micro controller. The aim is to leverage all the strengths of the microcontroller such as interupt service routines, timers and hardware interupts to produce a system capable of acting as a reliable musical instrument for the user. The topic is interesting as an exercise in implementing an audio data processing pipeline that involves many signal processing components such as oscillators, pitch control, gain mixing and attenuation, envelope processing and filtering to produce a variety of different sounds. The project represents building a complex system capable of being embedded in a limited processing space with options to extend its capabilities. The final project yielded a synthesizer meeting the fundamental requirements to be called a subtractive synthesizer: It can produce sound based off input from a midi instrument, can manipulate the signal gain to create an instrument capable volume dynamics. It can process the audio through high and lowpass filters to change the timbre to sound similar to real world instruments like guitars, pianos and string instruments and it has a hardware interface consisting of knobs, digital encoders and screen allowing the user to directly manipulate the sounds in real time. A computer software accessory application can communicate with the system to be able to adjust parameters, save and load user configurations for later use. Although the project meets minimum requirements for being a synthesizer, it has much work to be done. Many lessons were learned: the process of prototyping and implementing hardware peripherals can be fraught with frustrating bugs and failures. Embedded low level software has a much more involved and timeconsuming process for debugging, with regression tests being an absolute necessity. A lot of the funcionality of electronic musical instruments that we expect is a harder system to implement that expected. The lines of code for this system ended up around 2250 which is much lower than what is typically required of the project. Much of the reason for the smaller count of lines of code was because 1) Much time getting the system working required many if not tens of hours writing drivers to interface with hardware without the system regressing 2) More lines of code is not a good idea in an embedded system as it causes the firmware builds to grow and the microprocessors flash memory cannot contain all the instructions and 3) 50-100 hours of time was taken away from software development for prototyping hardware on breadboard and then designing and implementing permanent hardware fixtures with many setbacks and revisions required.

Keywords: audio, systhesis, embedded, microcontroller, real-time, midi, hardware, filters, envelopes


Begin with a proper thesis statement, which may be several paragraphs. A proper thesis statement answers the
following 4 questions:
(1) What problem are you trying to solve?
(2) why is it interesting?
(3) What are your major results?
(4) What are your conclusions?

# Introduction

The problem I wanted to solve was rather simple: The acquisition of musical gear is common obsession that plagues millions of musicians around the world. The cost of gear for performance or music production quickly drains the wallets of musicians who already struggle to keep their bank accounts at a non-zero value and for many musicians, this thought crosses their mind at least once: "Can I just make my own X and save some money?". If you ask anyone who has gone down the road of making their own digital software and music hardware, they will tell you the answer is "Yes you can just make your own X. No you won't save any money. You will spend more". Over the years, I've researched this very topic and found this conclusion to be true but with a small caveat: You _can_ in fact save money by making your audio effect of electronic instrument as a digital system rather than an electrical circuit of discreet analog components. This caveat has it's own caveat: if time is money, then every hour of time spent in implementing your system is implicitly adding to the price tag of your system.

I've known these facts for a long time and have seen it in action as my education has had me walk through the process of making yet another database management system, yet another virtual machine. Unsuprisingly, my database hasn't replaced MySQL and my virtual machine hasn't replaced the JVM. Still, so much knowledge and understanding of how computers work was gained from these experiences. I applied this same principle to my experience as a musician with the goal of my masters project: Make yet another digital synthesizer on an embedded microcontroller. The purpose is to learn how implement a complex realtime embedded system, develop a digital audio pipeline, make a hardware interface to control it and indulge my fantasy of making my own piece of musical gear. In order to maximize the learning potential of this project and focus in on the process rather than the final destination, I decided to set a few restrictions:

1. The firmware must be implemented in a low level language such as C/C++.
2. I can only use the standard C/C++ libraries and a Hardware Abstraction Layer library to minimize time wasted fussing with getting hardware peripherals communicating with the microcontroller.
3. No libraries means no DSP libraries. Every bit of audio data music be generated by instructions hand crafted by me. Although I can reference other digital audio codebases, I avoided it as much as possible to allow myself a chance solve the problem on my own for better or for worse.


The resultant product of my project turned out to be satisfactory but fell miles short of where I hoped it would be. I believe my project currently is in a place where it can be considered something that meets the basic requirements to be called a subtractive synthesizer. It's relatively stable, but with a few tweaks stands to be a studio or stange performance ready instrument for musicians.

# Concepts

## A quick word on audio synthesis and features required
Audio synthesis is a rather broad topic and a project goal of "make a synthesizer" is very vague. There are so many styles and methods of audio synthesis. Since this is my first jump into the world of real time data processing, I decided to make my synthesizer as classic as possible. The system I designed is a __subtractive__ synthesizer: fundamental waveforms are combined and then passed through a series of filters to strip away certain frequencies to create new sounds. This method is a relatively primitive yet classic means mimicking real world instruments and dates back nearly a century. Subtractive synthesis was made especially popular in the 60's and 70's for its ability to bring new and imaginative sounds to rock and jazz music. 

In order to make a digital subtractive synthesizer that meets the bare minimum requirements, I needed to implement the following features for my microcontroller:
1. Oscillators - produces an audio signal by repeating some pattern such as a sine wave at a set frequency
2. MIDI - __Music Instrument Data Interface__ or MIDI is the standard way that musicians communicate with electronic instruments and software. A common electronic piano keyboard usually has a MIDI out jack on it allowing the user to control other devices using the keys, knobs and buttons on their keyboard. My system needs to be able to receive generic MIDI messages commanding it to turn notes on and off in real time
3. In addition to MIDI capability, the system must have 8 voices, meaning it can play up to 8 notes at once. 8 is a rather arbitrary number but is generally an acceptable amount for most keyboard players. 
4. Filters - After oscillators, filters are the most critical component in a subtractive synthesizer as it allows the user to shape and mold their basic fundamental frequencies into new sounds.
5. Envelopes - After Oscillators and Filters, envelopes represent a critical component to a synthesizer. Envelopes are gates that may be triggered by arbitrary signal sources such as a key press on the keyboard and over time, the envelope gate will open and close over time. Most commonly, an envelope is applied to the output gain of a synthesizer allowing control of the volume which adds a great deal of strength to the synthesizers ability to mimick other instruments.






















