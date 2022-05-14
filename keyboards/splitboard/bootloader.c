//DO NOT TOUCH THIS SHIT UNLESS YOU KNOW WHAT YOU'RE DOING!


//To enter bootloader, simply hold down the bonus-key on the right hand and the B key.


void bootmagic_lite(void){
	DDRD  &= ~(1<<PD2 | 1<<PD3);		//Initiate the 2 cols that will be used for boot.
	PORTD |=  (1<<PD2 | 1<<PD3);
	
	DDRC  &= ~(1<<PC6 | 1<<PC7);		//Initiates all the outputs.
	PORTC &= ~(1<<PC6 | 1<<PC7);
	DDRE  &= ~(1<<PE6);
	PORTE &= ~(1<<PE6);
	DDRF  &= ~(1<<PF5 | 1<<PF6 | 1<<PF7);
	PORTF &= ~(1<<PF5 | 1<<PF6 | 1<<PF7);
	
	DDRF  |=  (1<<PF7);					//Selects row 3
	PORTF &= ~(1<<PF7);
	
	wait_ms(1);
	
	uint8_t temp = !(PIND&(1<<PD2)); //Read column 8 (BK on right hand)
	if(temp){
		DDRF  &= ~(1<<PF7);		//Unselect the row from earlier.
		PORTF |=  (1<<PF7);
		
		DDRF  |=  (1<<PF6);		//Select row 4
		PORTF &= ~(1<<PF6);
		wait_ms(1);
		
		temp = !(PIND&(1<<PD3));	//Read column 9 (B on right hand (if QWERTY))
		if(temp){
			//eeprom_set_valid(false);
			eeconfig_disable();
			bootloader_jump();
		}
	}
}