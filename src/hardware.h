#ifndef HARDWARE_H
#define HARDWARE_H

#include "daisy_seed.h"

using namespace daisy;
using namespace daisy::seed;

//Hardware
DaisySeed synth_hw;
UartHandler synth_uart;
MidiUsbHandler synth_midi;

static void synth_hardware_init() {
    synth_hw.Init();
    synth_hw.Configure();
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    synth_midi.Init(midi_cfg);
    UartHandler::Config uart_config;
    uart_config.baudrate      = 115200;
    uart_config.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_config.stopbits      = UartHandler::Config::StopBits::BITS_2;
    uart_config.parity        = UartHandler::Config::Parity::NONE;
    uart_config.mode          = UartHandler::Config::Mode::TX_RX;
    uart_config.wordlength    = UartHandler::Config::WordLength::BITS_8;
    uart_config.pin_config.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    uart_config.pin_config.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14
    synth_uart.Init(uart_config);
}

#endif