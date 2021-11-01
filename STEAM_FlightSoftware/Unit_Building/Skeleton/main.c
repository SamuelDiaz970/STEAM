Static int scheduled_event;
Volatile int mode; // initialize to safe mode
/*
 * 0 for Nominal Power Mode
 * 1 for Reduced Power Mode
 * 2 for Safe Mode
 */

main()
{
	mode = 2;
	Initialize HMS
	Initialize timer for HMS check (set period to 3 sec)
	Enable interrupts for RS-422, timer

	mode = 0; // nominal power mode

	sxr_open
	While (MOSFET 1 current > 0.9);

	hxr_open
	While (MOSFET 2 current > 0.9);

	while(1)
	{
		If (scheduled_event & CHECK_HMS)
		{
			scheduled_event &= ~CHECK_HMS;
			check_hms();
		}

		If (scheduled_event & PARSE_COMMAND)
		{
			scheduled_event &= ~PARSE_COMMAND;
			return_packet = parse_punch_command(command);
			
		}

		If (scheduled_event & MODE_CHANGE)
// unless we want to do HMS handling differently
		{
			scheduled_event &= ~MODE_CHANGE;
			change_mode(mode);
		}

		// Spectrometer data has already been recorded in parameter table with shell
		// script, so fetch_spec_data() reads from this table
If (scheduled_event & FETCH_SPEC_DATA)
		{
			scheduled_event &= ~FETCH_SPEC_DATA;
			fetch_spec_data(spec); // specify SXR or HXR
		}

		If (scheduled_event & SPEC_CONFIG) //may be used if health interrupt received
		{
			scheduled_event &= ~SPEC_CONFIG;
			spec_config(spec); // specify SXR or HXR
		}
	}
}

// not sure if this is how we’re handling interrupts
rs422_irq_handler(void) {
	Clear interrupt flag
	command = RS-422 message;
	scheduled_event |= PARSE_COMMAND;
}

timer_irq_handler(void) {
	Clear interrupt flag
	scheduled_event |= CHECK_HMS;
}

