	component nios_II_led is
		port (
			clk_clk    : in  std_logic                    := 'X'; -- clk
			led_export : out std_logic_vector(9 downto 0)         -- export
		);
	end component nios_II_led;

	u0 : component nios_II_led
		port map (
			clk_clk    => CONNECTED_TO_clk_clk,    -- clk.clk
			led_export => CONNECTED_TO_led_export  -- led.export
		);

