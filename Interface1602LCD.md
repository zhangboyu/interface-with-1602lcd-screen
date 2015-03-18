# This page includes following things. #

![http://www.sure-electronics.net/mcu,display/DE-LM016_1_b.jpg](http://www.sure-electronics.net/mcu,display/DE-LM016_1_b.jpg)

## [Interface1602LCD#1.Basic\_information\_about\_1602\_LCD.](Interface1602LCD#1.Basic_information_about_1602_LCD..md) ##

## [Interface1602LCD#2.How\_to\_interface\_1602\_LCD\_if\_we\_do\_not\_have\_an\_arduino.](Interface1602LCD#2.How_to_interface_1602_LCD_if_we_do_not_have_an_arduino..md) ##

## [Interface1602LCD#3.How\_to\_interface\_1602\_LCD\_with\_an\_arduino.](Interface1602LCD#3.How_to_interface_1602_LCD_with_an_arduino..md) ##

**---------------------------------------------------------------------------------------------------------------------**

## 1.Basic information about 1602 LCD. ##

A Liquid-Crystal Display (LCD) is a flat panel display that uses the light modulating properties of liquid crystals. By themselves, LCD can be difficult to drive because they require multiplexing, AC drive wave forms,and special voltages. LCD modules make this driving simpler by attaching hardware to the raw glass LCD to assist in some or all of these rudimentary driving tasks. LCD modules can be split into two groups: those that have built-in controller and driver chips, and those that have only driver chips. The category of display modules that have built-in controllers can be split again into character LCD modules and graphic LCD modules. The 1602 LCD I used here is a character LCD with built-in controller and driver. The controller in this module is Hitachi HD44780, and [here](http://pdf1.alldatasheet.com/datasheet-pdf/view/63673/HITACHI/HD44780.html) is the full datasheet for this chip.

Luckily, we do not need to read this lengthy datasheet, because it already connected with the raw LCD screen and we don't need to know how they get connected. The only two things we need to consider are:

1).The layout of then LCD screen. According to its name (1602) we can know that this module has two rows and each row can display 16 characters, and each character is shown by a 5\*8 dot matrix.

2).The pin of this entire module. There are 16 or 14 pins (the additional 2 pins are for back-light power) on this module. Their name and function description can be found at [here](https://www.sparkfun.com/datasheets/LCD/GDM1602K-Extended.pdf). Don't worry if you can not fully understand it, I'll illustrate them in the second part.

## 2.How to interface 1602 LCD if we do not have an arduino. ##

Before the advent of arduino, there are many types of microcontrollers which were not advanced like arduino (like Intel MCS-51, also known as 8051). Using them to interface with 1602 LCD is much more complex than using an arduino. However, I feel it is necessary for us to know how it works so that we can have a very solid foundation to start with.
The 1602 LCD is mainly used to show something, so we need to tell it what to display, and this kind of information is data. We also need to tell it where or how to display, this kind of information is command. These actions are "write" since we are actually something to the LCD. The opposite direction, reading, is also important, because we need to know the current status of the LCD so that we can decide if it is a right time to write data/command. As I mentioned in the first section, this module has 14/16 pins, and these pins provide us the way to communicate with LCD module. There are two ways to communicate with the LCD, one uses all the eight data lines and another one uses only four data lines (D7-D4). There are no substantial difference between these two ways, the later one simply write/read the data in two clock cycles: first cycle write/read the most four significant bits (D7-D4) and the second cycle read write/read the least four significant bits (D3-D0) but with the same physical pin. Due to this reason, I will only show how to use the eight data lines version at here.

The typical communication sequence is like this:

1).Read the current status of 1602 LCD to see if it is busy. If not, go to the second step, else, continue to check it until it is not;

2).Write command. (Before tell it what to display, you have to tell it where and how to display it.)(Information is sent to LCD at the negative edge of the enable bit.)

3).Read the current data of 1602 LCD. (We usually do not use this.)

4).Write data. (Tell it what to display.)(Information is sent to LCD at the negative edge of the enable bit.)

Now, let's follow this sequence to get it work. [This](http://www.trash.net/~luethi/microchip/datasheets/lcd/the_lcd_data_sheet.pdf) pdf file tells us all the instructions we need to interface the 1602 LCD. At the very beginning, we need to initialize the LCD (VERY IMPORTANT!). This action is done by the "Function Set" entry in the file. For example, if we set the RS and RW bit low, write 0x38 on the data line, we actually set the LCD with 8 bit interface (D7-D0), two lines on the screen, and using 5\*7 dots to display a character. After initializing the LCD, we can use other instructions to set the way cursor shift or set whether or not the cursor will be blinking. It is suggested that to use the "Clear Display" instruction after all the settings are down so that we can make sure the screen is clean and the cursor is at upperleft corner.

After set all these "environment", it always better to check if the LCD is busy or not (using the "Busy Flay & Address" instruction). Wait until it is not busy, we can continue. Now it's time to introduce the "Set DDRAM Address" instruction. First, what is DDRAM? DDRAM stands for Display Data RAM. We can simply regard it as the memory to store the data which will be shown on the screen. For every character cell on the screen, there is a part of DDRAM to store the data that will be displayed in this cell. The address of these parts of DDRAM are listed on the eighth page of [this](https://www.sparkfun.com/datasheets/LCD/GDM1602K-Extended.pdf) file . By using the "Set DDRAM" instruction, we tell the LCD which part of the DDRAM we want to change so that change the display of the corresponding character cell. For example, if we set RS and RW bit low, and write 0x81 on the data lines, we tell the LCD that we will do something for the second character cell in the first line on the screen.

So, the next question is, what we want to do? In most cases, we just want to change the character displayed in this cell. But how? Now it comes to the "Write Data" instruction. We simply set the RS bit high, set the RW bit low, and write the data we want on the data lines. Then the data in the corresponding DDRAM will be changed and the display on the corresponding cell will change. But how can we know what pattern of binary code we should send to generate desired character? There is something called "CGROM" in this module, which contains 160 predefined characters including English letters, numbers, punctuations, and even some Japanese letters. What we need to do is simply send character code to the data line and the character you want will appears on the location you defined in the former step. [Here](http://www.trash.net/~luethi/microchip/datasheets/lcd/lcd_data_sheets.html) is the table between character codes and character patterns. For example, if you set RS high, RW low, and write 0x31 on the data lines, you will see "1" appears on the second cell of the first line of the screen. You may notice that there is a instruction called "Set CGRAM Address" and there is a area in the table I mentioned above called "CGRAM." This instruction and this memory area are used to create your own character patterns (but only 8 due to the limited space). The details of how to create and use your own character pattern are also included in the website above.

Following these steps, we can let the screen display every thing (actually not "everything") we want on the screen at the desired place.

I also wrote an arduino sketch which contains some functions that implemented almost all of the functionalities in the built-in library and even some detailed control abilities. Just with several minor modifications, you can apply this sketch to other microprocessors. There are some limitations with my code, anyway, it is a good starting point. I also wrote a lot of comments with the code just to make it easy to understand and modified. You can find this sketch in the source section. It's called "basic\_method."

## 3.How to interface 1602 LCD with an arduino. ##

Fortunately, the steps I mentioned in the second part have been significantly simplified in the arduino environment. There is a built-in library called "LiquidCrystal" in the arduino IDE. After include it at the head of your sketch, you can use all the functions in this library, and this is a far more easy way to interface with the LCD. First, we need to create a variable of type LiquidCrystal called "lcd"(you can name it whatever you want) with proper parameters. Like this: LiquidCrystal lcd(12,11,10,5,4,3,2);. (This is like a class instantiation in Java.) Then at the setup block or the any other blocks you can use the function calls like: lcd.begin(16,2); (This function implements the initialization process in the second section) or lcd.setCursor(0,0);lcd.write(0x31); (These two functions will write "1" at first cell of the first line.) [Here](http://arduino.cc/en/Reference/LiquidCrystal#.Uwg-c_RDt8H) is the list of all the functions in this library and their description.

Sometimes the 1602 LCD module we get may have an additional chip on it. The core of this additional chip is PCF8574, which is a remote 8-bit I/O expander for IIC bus. The information about IIC bus can be found at [here](http://en.wikipedia.org/wiki/I%C2%B2C), but simply speaking, the IIC bus enables arduino interface with several peripherals with only two lines. In our case, this additional chip gives the LCD module an address and enables LCD module interface with arduino with only 4 wires. However, we cannot use the former library anymore, because the LiquidCrystal library is not designed for this kind of communication. The way we solve this problem is by replacing the original library by a modified library, which enables us interface with IIC bus and gives us more functionalities. [Here](https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads) is the website to download it and [here](http://arduino.cc/en/Guide/Libraries#.UwhCzPRDt8E) is how to install a library to our IDE. In the "docs" folder inside the .zip file you get from the first URL, you will see a list of functions and their descriptions, just like what you see before for the original library.

There are two things we need to pay special attention to if we want to use the IIC LCD module.

1).At the very beginning of our sketch, we must include the Wire library. In other words, write: #include<Wire.h>; at the beginning of sketch. This library allows you to communicate with IIC/TWI devices.

2).We have to use specific pins to communicate with the IIC devices. On Arduino UNO, the SDA corresponds to analog input pin A4, and SCL corresponds to analog input A5. On Mega2560, SDA corresponds to pin 20 and SCL sorresponds to pin 21.

Here is a simple demo.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=WTGCe5ztNoE' target='_blank'><img src='http://img.youtube.com/vi/WTGCe5ztNoE/0.jpg' width='425' height=344 /></a>

Here are other demos that combine the ultrasonic sensor with the LCD screen. The first one shows the distance detected by the ultrasonic sensor. The distance is showed in number and by the blocks in the second line. There is a little battery animation on the upper right corner, just for fun. The second on made an animation of a moving sine wave. The speed of the moving wave is associated with the distance detected by the ultrasonic sensor. The code of both demo is in the source folder. The wire diagram are also included in the source folder (too big to show at here). (Note that the wire diagram for demo1 is slightly different from the diagram that I actually used, because I can't find the LCD 1602 module that have an I2C chip with it. The code for demo1 will work perfectly for this wire diagram after some minor modifications.)

Demo2

<a href='http://www.youtube.com/watch?feature=player_embedded&v=hBfCu1Bcdxw' target='_blank'><img src='http://img.youtube.com/vi/hBfCu1Bcdxw/0.jpg' width='425' height=344 /></a>

Sine wave demo

<a href='http://www.youtube.com/watch?feature=player_embedded&v=to-kpUAIAJ0' target='_blank'><img src='http://img.youtube.com/vi/to-kpUAIAJ0/0.jpg' width='425' height=344 /></a>

I think this is all we need for the 1602 LCD module. Have fun!