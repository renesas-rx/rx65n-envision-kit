* (1)Download Renesas Flash Programmer from following site.
  * https://www.renesas.com/software-tool/renesas-flash-programmer-programming-gui
* (2)Install Renesas Flash Programmer on your PC.
* (3)Execute chip_erase.rpj on following path.
  * \rx65n-envision-kit\initial_firmware\01_chip_erase
* (4)Set SW1-1 (on your RX65N Envision Kit board) to ON (debug mode)
* (5)Push Start on your Renesas Flash Programmer to erase all flash memory that built in your RX65N.
* (6)Connect your USB cable from PC(USB) to RX65N Envision Kit(CN9)
* (7)Execute rx65n_envisionkit_test.rpj on following path.
  * \rx65n-envision-kit\initial_firmware\02_rx65n_envisionkit_test
* (8)Push Start on your Renesas Flash Programmer to write flash memory that built in your RX65N.
* (9)Set SW1-1 (on your RX65N Envision Kit board) to OFF (single chip mode)
* (10)You can confirm initial firmware on your RX65N Envision Kit
