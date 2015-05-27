/* 
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
#define W 4*Q

void delay_ms(uint16_t count) {
	while(count--) {
		_delay_ms(1);

	}
}

uint16_t imperial_march[250] = {A4, Q, Q, A4, Q, Q, A4, Q, Q, F4, E+S, E+S, C5, S, S, A4, Q, Q,
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
								A4, H, H};

#endif /* PERSONALITY_H_ */