#include "hardware.h"

MidiUsbHandler Hardware::synth_midi;
DaisySeed Hardware::synth_hw;
UartHandler Hardware::synth_uart;

void Hardware::Callback(void* data)
{
    /** Use system time to blink LED once per second (1023ms) */
    bool led_state = (System::GetNow() & 1023) > 511;
    /** Set LED */
    synth_hw.SetLed(led_state);
}

void Hardware::synth_hardware_init() {
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
    

    TimerHandle         tim5;
    TimerHandle::Config tim_cfg;

    /** TIM5 with IRQ enabled */
    tim_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
    tim_cfg.enable_irq = true;

    /** Configure frequency (30Hz) */
    auto tim_target_freq = 30;
    auto tim_base_freq   = System::GetPClk2Freq();
    tim_cfg.period       = tim_base_freq / tim_target_freq;

    /** Initialize timer */
    tim5.Init(tim_cfg);
    tim5.SetCallback(Hardware::Callback);

    /** Start the timer, and generate callbacks at the end of each period */
    tim5.Start();
}

void Hardware::Test() {

}


void Hardware::SerialDebugWriteString( const char txBuffer[],  int bufferSize){
    Hardware::synth_uart.PollTx((uint8_t *)&txBuffer[0],  bufferSize);
}