#include "system_cfg.h"

#include "drivers/pulse_detector/pulse_detector.h"
#include "drivers/pulse_detector/pulse_detector_cfg.h"

#include <stdbool.h>

#include <avr/io.h>

/****************************************************************************\
 * Initializes Pulse Detector
 *
 * Context: User
 *
\****************************************************************************/
void PulseDetector_Init(void)
{
   /* Configures channel A if enabled */
   #ifdef PULSE_DETECTOR_CHANNEL_A_CONFIG
   #if (PULSE_DETECTOR_CHANNEL_A_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)

   /* Configures pins as input - pullup */
   DDRD &= ~(1 << DDD2);
   PORTD |= 1 << PORTD2;

   /* Disables interrupt */
   EIMSK &= ~(1 << INT0);

   /* Configures edge interrupt */
   #if (PULSE_DETECTOR_CHANNEL_A_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_RISING_EDGE)
   EICRA |= (1 << ISC01) | (1 << ISC00);
   #elif (PULSE_DETECTOR_CHANNEL_A_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_FALLING_EDGE)
   EICRA &= ~(1 << ISC00);
   EICRA |= 1 << ISC01;
   #elif (PULSE_DETECTOR_CHANNEL_A_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_ANY_EDGE)
   EICRA &= ~(1 << ISC01);
   EICRA |= 1 << ISC00;
   #else
   #error "PulseDetector - Invalid configuration of channel A"
   #endif

   /* Clears interrupt */
   EIFR  =  1 << INTF0;

   #endif
   #endif

   /* Configures channel B if enabled */
   #ifdef PULSE_DETECTOR_CHANNEL_B_CONFIG
   #if (PULSE_DETECTOR_CHANNEL_B_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)

   /* Configures pins as input - pullup */
   DDRD &= ~(1 << DDD3);
   PORTD |= 1 << PORTD3;

   /* Disables interrupt */
   EIMSK &= ~(1 << INT1);

   /* Configures edge interrupt */
   #if (PULSE_DETECTOR_CHANNEL_B_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_RISING_EDGE)
   EICRA |= (1 << ISC11) | (1 << ISC10);
   #elif (PULSE_DETECTOR_CHANNEL_B_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_FALLING_EDGE)
   EICRA &= ~(1 << ISC10);
   EICRA |= 1 << ISC11;
   #elif (PULSE_DETECTOR_CHANNEL_B_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_ANY_EDGE)
   EICRA &= ~(1 << ISC11);
   EICRA |= 1 << ISC10;
   #else
   #error "PulseDetector - Invalid configuration of channel B"
   #endif

   /* Clears interrupt */
   EIFR  =  1 << INTF1;

   #endif
   #endif

   /* Configures channel C if enabled */
   #ifdef PULSE_DETECTOR_CHANNEL_C_CONFIG
   #if (PULSE_DETECTOR_CHANNEL_C_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
   #if (PULSE_DETECTOR_CHANNEL_C_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_ANY_EDGE)

   /* Configures pins as input - pullup */
   #if (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 0)
   DDRB &= ~(1 << DDB0);
   PORTB |= 1 << PORTB0;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 1)
   DDRB &= ~(1 << DDB1);
   PORTB |= 1 << PORTB1;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 2)
   DDRB &= ~(1 << DDB2);
   PORTB |= 1 << PORTB2;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 3)
   DDRB &= ~(1 << DDB3);
   PORTB |= 1 << PORTB3;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 4)
   DDRB &= ~(1 << DDB4);
   PORTB |= 1 << PORTB4;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 5)
   DDRB &= ~(1 << DDB5);
   PORTB |= 1 << PORTB5;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 6)
   DDRB &= ~(1 << DDB6);
   PORTB |= 1 << PORTB6;
   #elif (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER == 7)
   DDRB &= ~(1 << DDB7);
   PORTB |= 1 << PORTB7;
   #else
   #error "PulseDetector - Invalid interrupt number of channel C"
   #endif

   /* Disables interrupt */
   PCICR &= ~(1 << PCIE0);

   /* Selects PIN */
   PCMSK0 = 1 << (PULSE_DETECTOR_CHANNEL_C_INT_NUMBER);

   /* Clears interrupt */
   PCIFR  =  1 << PCIF0;

   #else
   #error "PulseDetector - Invalid configuration of channel C"
   #endif
   #endif
   #endif

/* Configures channel D if enabled */
   #ifdef PULSE_DETECTOR_CHANNEL_D_CONFIG
   #if (PULSE_DETECTOR_CHANNEL_D_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
   #if (PULSE_DETECTOR_CHANNEL_D_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_ANY_EDGE)

   /* Configures pins as input - pullup */
   #if (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 8)
   DDRC &= ~(1 << DDB0);
   PORTC |= 1 << PORTC0;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 9)
   DDRC &= ~(1 << DDB1);
   PORTC |= 1 << PORTC1;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 10)
   DDRC &= ~(1 << DDB2);
   PORTC |= 1 << PORTC2;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 11)
   DDRC &= ~(1 << DDB3);
   PORTC |= 1 << PORTC3;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 12)
   DDRC &= ~(1 << DDB4);
   PORTC |= 1 << PORTC4;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 13)
   DDRC &= ~(1 << DDB5);
   PORTC |= 1 << PORTC5;
   #elif (PULSE_DETECTOR_CHANNEL_D_INT_NUMBER == 14)
   DDRC &= ~(1 << DDB6);
   PORTC |= 1 << PORTC6;
   #else
   #error "PulseDetector - Invalid interrupt number of channel D"
   #endif

   /* Disables interrupt */
   PCICR &= ~(1 << PCIE1);

   /* Selects PIN */
   PCMSK1 = 1 << ((PULSE_DETECTOR_CHANNEL_D_INT_NUMBER) - 8);

   /* Clears interrupt */
   PCIFR  =  1 << PCIF1;

   #else
   #error "PulseDetector - Invalid configuration of channel D"
   #endif
   #endif
   #endif

/* Configures channel E if enabled */
   #ifdef PULSE_DETECTOR_CHANNEL_E_CONFIG
   #if (PULSE_DETECTOR_CHANNEL_E_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
   #if (PULSE_DETECTOR_CHANNEL_E_CONFIG == PULSE_DETECTOR_CONFIG_CHANNEL_ANY_EDGE)

   /* Configures pins as input - pullup */
   #if (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 16)
   DDRD &= ~(1 << DDB0);
   PORTD |= 1 << PORTD0;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 17)
   DDRD &= ~(1 << DDB1);
   PORTD |= 1 << PORTD1;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 18)
   DDRD &= ~(1 << DDB2);
   PORTD |= 1 << PORTD2;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 19)
   DDRD &= ~(1 << DDB3);
   PORTD |= 1 << PORTD3;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 20)
   DDRD &= ~(1 << DDB4);
   PORTD |= 1 << PORTD4;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 21)
   DDRD &= ~(1 << DDB5);
   PORTD |= 1 << PORTD5;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 22)
   DDRD &= ~(1 << DDB6);
   PORTD |= 1 << PORTD6;
   #elif (PULSE_DETECTOR_CHANNEL_E_INT_NUMBER == 23)
   DDRD &= ~(1 << DDB7);
   PORTD |= 1 << PORTD7;
   #else
   #error "PulseDetector - Invalid interrupt number of channel E"
   #endif

   /* Disables interrupt */
   PCICR &= ~(1 << PCIE2);

   /* Selects PIN */
   PCMSK2 = 1 << ((PULSE_DETECTOR_CHANNEL_E_INT_NUMBER) - 16);

   /* Clears interrupt */
   PCIFR  =  1 << PCIF2;

   #else
   #error "PulseDetector - Invalid configuration of channel E"
   #endif
   #endif
   #endif
}

/****************************************************************************\
 * Gets state of pulse detector channel
 *
 * Context: User
 *
 * Return:
 *    true  - any pulse has been detected
 *    false - no pulse has been detected
 *
\****************************************************************************/
#ifdef PULSE_DETECTOR_CHANNEL_A_CONFIG
#if (PULSE_DETECTOR_CHANNEL_A_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_A(void)
{
   /* Copy of interrupt flags */
   uint8_t interrupts;

   /* Reads current interrupt state */
   interrupts = EIFR;

   /* Filter other flags */
   interrupts &= 1 << INTF0;

   /* Clears the interrupt flag only if it has been read as active */
   EIFR = interrupts;

   /* Returns result */
   return((interrupts != 0) ? (true) : (false));
}
#endif
#endif

/****************************************************************************\
 * Gets state of pulse detector channel
 *
 * Context: User
 *
 * Return:
 *    true  - any pulse has been detected
 *    false - no pulse has been detected
 *
\****************************************************************************/
#ifdef PULSE_DETECTOR_CHANNEL_B_CONFIG
#if (PULSE_DETECTOR_CHANNEL_B_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_B(void)
{
   /* Copy of interrupt flags */
   uint8_t interrupts;

   /* Reads current interrupt state */
   interrupts = EIFR;

   /* Filter other flags */
   interrupts &= 1 << INTF1;

   /* Clears the interrupt flag only if it has been read as active */
   EIFR = interrupts;

   /* Returns result */
   return((interrupts != 0) ? (true) : (false));
}
#endif
#endif

/****************************************************************************\
 * Gets state of pulse detector channel
 *
 * Context: User
 *
 * Return:
 *    true  - any pulse has been detected
 *    false - no pulse has been detected
 *
\****************************************************************************/
#ifdef PULSE_DETECTOR_CHANNEL_C_CONFIG
#if (PULSE_DETECTOR_CHANNEL_C_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_C(void)
{
   /* Copy of interrupt flags */
   uint8_t interrupts;

   /* Reads current interrupt state */
   interrupts = PCIFR;

   /* Filter other flags */
   interrupts &= 1 << PCIF0;

   /* Clears the interrupt flag only if it has been read as active */
   PCIFR = interrupts;

   /* Returns result */
   return((interrupts != 0) ? (true) : (false));
}
#endif
#endif

/****************************************************************************\
 * Gets state of pulse detector channel
 *
 * Context: User
 *
 * Return:
 *    true  - any pulse has been detected
 *    false - no pulse has been detected
 *
\****************************************************************************/
#ifdef PULSE_DETECTOR_CHANNEL_D_CONFIG
#if (PULSE_DETECTOR_CHANNEL_D_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_D(void)
{
   /* Copy of interrupt flags */
   uint8_t interrupts;

   /* Reads current interrupt state */
   interrupts = PCIFR;

   /* Filter other flags */
   interrupts &= 1 << PCIF1;

   /* Clears the interrupt flag only if it has been read as active */
   PCIFR = interrupts;

   /* Returns result */
   return((interrupts != 0) ? (true) : (false));
}
#endif
#endif

/****************************************************************************\
 * Gets state of pulse detector channel
 *
 * Context: User
 *
 * Return:
 *    true  - any pulse has been detected
 *    false - no pulse has been detected
 *
\****************************************************************************/
#ifdef PULSE_DETECTOR_CHANNEL_E_CONFIG
#if (PULSE_DETECTOR_CHANNEL_E_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_E(void)
{
   /* Copy of interrupt flags */
   uint8_t interrupts;

   /* Reads current interrupt state */
   interrupts = PCIFR;

   /* Filter other flags */
   interrupts &= 1 << PCIF2;

   /* Clears the interrupt flag only if it has been read as active */
   PCIFR = interrupts;

   /* Returns result */
   return((interrupts != 0) ? (true) : (false));
}
#endif
#endif

/****************************************************************************/
