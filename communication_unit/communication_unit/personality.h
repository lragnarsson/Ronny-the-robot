﻿/* 
 * personality.h
 * Ronny-the-robot/communication_unit
 * ------------------------------
 * This file contains the defines required for playing songs.
 * ------------------------------ 
 * Author: L. Ragnarsson, F. Östman
 */ 

#ifndef PERSONALITY_H_
#define PERSONALITY_H_

#define F_CPU 18432000UL
#include <util/delay.h>

#define TIMER3_CLEAR_ON_MATCH_H (0<<WGM13) | (1<<WGM12)
#define TIMER3_CLEAR_ON_MATCH_L (0<<WGM11) | (0<<WGM10)
#define TIMER3_PRESCALE_64 (0<<CS12) | (1<<CS11) | (1<<CS10)
#define TIMER3_MATCH_FREQUENCY_100HZ F_CPU / 1000 / 64		// = 288
#define TIMER3_INTERRUPT_ENABLE (1<<OCIE1A)

#define C4 8806
#define Db4 8312
#define D4 7846
#define Eb4 7405
#define E4 6990
#define F4 6597
#define Gb4 6227
#define G4 5878
#define Ab4 5548
#define A4 5236
#define Bb4 4943
#define B4 4665
#define C5 4403
#define Db5 4156
#define D5 3923
#define Eb5 3703
#define E5 3495
#define F5 3299
#define Gb5 3114
#define G5 2939
#define Ab5 2774
#define A5 2618
#define Bb5 2471
#define B5 2332
#define C6 2202
#define Db6 2078
#define D6 1961
#define Eb6 1851
#define E6 1747
#define F6 1649
#define Gb6 1557
#define G6 1496
#define Ab6 1387
#define A6 1309
#define Bb6 1236
#define B6 1166
#define C7 1101
#define Db7 1039
#define D7 981
#define Eb7 926
#define E7 874
#define F7 825
#define Gb7 778
#define G7 735
#define Ab7 693
#define A7 655
#define Bb7 618
#define B7 583
#define C8 550

#define BPM 200
#define H 2*Q
#define Q 60000/BPM
#define E Q/2
#define S Q/4
#define X Q/8
#define W 4*Q
#define T Q/3

uint8_t current_sound;
uint8_t sound_index;
uint8_t sound_phase;
uint16_t sound_clock;


uint16_t music[10][250] = {{Q, A4, Q, Q, A4, Q, Q, A4, Q, Q, F4, E+S, E+S, C5, S, S, A4, Q, Q,
							F4, E+S, E+S, C5, S, S, A4, H, H, E5, Q, Q, E5, Q, Q, E5, Q, Q,
							F5, E+S, E+S, C5, S, S, Ab4, Q, Q, F4, E+S, E+S, C5, S, S,
							A4, H, H, A5, Q, Q, A4, E+S, E+S, A4, S, S, A5, Q, Q, Ab5, E+S, E+S,
							G5, S, S, Gb5, S, S, E5, S, S, F5, E, E+Q, Bb4, E, E, Eb5, Q, Q,
							D5, E+S, E+S, Db5, S, S, C5, S, S, B4, S, S, C5, E, E+Q, F4, E, E,
							Ab4, Q, Q, F4, E+S, E+S, A4, S, S, C5, Q, Q, A4, E+S, E+S, C5, S, S,
							E5, H, H, A5, Q, Q, A4, E+S, E+S, A4, S, S, A5, Q, Q, Ab5, E+S, E+S,
							G5, S, S, Gb5, S, S, E5, S, S, F5, E, E+Q, Bb4, E, E, Eb5, Q, Q,
							D5, E+S, E+S, Db5, S, S, C5, S, S, B4, S, S, C5, E, E+Q, F4, E, E,
							Ab4, Q, Q, F4, E+S, E+S, C5, S, S, A4, Q, Q, F4, E+S, E+S, C5, S, S,
							A4, H, H},
							{Q, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
							    Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S,
								Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S, Bb6, H, S, F5, H, S},
							{Q, G4, T, X, C4, T, X, E4, T, X, G5, T, X, C5, T, X, E5, T, X, 
							 G6, S+ E, X, E5, Q, S, Ab4, T, X, C4, T, X, Eb4, T, X, Ab5, T, X,
							 C5, T, X, Eb5, T, X, Ab6, S + E, X, Eb6, Q, S, Bb4, T, X, D5, T, X,
							 F5, T, X, Bb5, T, X, D6, T, X, F6, T, X, Bb6, E + S, S, Bb6, T, X,
							 Bb6, T, X, Bb6, T, X, C7, H, H}};

#endif /* PERSONALITY_H_ */