#define POS_SIG	2
#define NEG_SIG	3

void setupTimer1() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // 800 Hz (16000000/((2499+1)*8))
  OCR1A = 2499;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 8
  TCCR1B |= (1 << CS11);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void setup() {
	pinMode(POS_SIG, OUTPUT);
	pinMode(NEG_SIG, OUTPUT);
	setupTimer1();
}

void loop() {
}

inline void set_posi_sig() __attribute__((always_inline));
inline void set_nego_sig() __attribute__((always_inline));

void set_posi_sig() {
	PORTD = PORTD &~ (1 << POS_SIG) | (1 << NEG_SIG);
}

void set_nego_sig() {
	PORTD = PORTD &~ (1 << NEG_SIG) | (1 << POS_SIG);
}

ISR(TIMER1_COMPA_vect) {
	static boolean blink = true;
	blink = ((blink) ? false : true);
	if (blink) {
		set_posi_sig();
	} else {
		set_nego_sig();
	}
}
