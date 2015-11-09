/*
 * gps.c
 *
 *  Created on: Sep 23, 2014
 *      Author: Erik Moore, Kyler Callahan
 *
 *  Takes in GPS data and parses it for information
 */

#include <stdio.h>
#define Switches (volatile int *) 0x01001030
#define LEDs 	 (int *) 0x01001040
//Variables
	char current_char;    //create a char to use as the scanned character
	char chk_high;        //value for high char of the check sum
	char chk_low;         //value for low char of the check sum
	char xors;
	char gps_string[80];  //character array for inputed GPS data
	int count;            //length of data in the array
	int pos;              //a position dummy variable
	char hour_high;
	char hour_low;
	int invalid_flag = 0;
	float elev;
	float degrees_latitude;
	float degrees_longitude;
	char DN = 'A';

//Functions
void mountain_time() //converts UTC to Mountain Time (-6 hours)
{
	hour_high = gps_string[6];
	hour_low = gps_string[7];
	if((hour_high =='0')&&(hour_low=='0'))
	{
		gps_string[6] = '0';
		gps_string[7] = '6';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='1'))
	{
		gps_string[6] = '0';
		gps_string[7] = '7';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='2'))
	{
		gps_string[6] = '0';
		gps_string[7] ='8';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='3'))
	{
		gps_string[6] = '0';
		gps_string[7] = '9';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='4'))
	{
		gps_string[6] = '1';
		gps_string[7] = '0';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='5'))
	{
		gps_string[6] = '1';
		gps_string[7] = '1';
		DN = 'P';
	}
	else if((hour_high =='0')&&(hour_low=='6'))
	{
		gps_string[6] = '1';
		gps_string[7] = '2';
		DN = 'A';
	}
	else if((hour_high =='0')&&(hour_low=='7'))
	{
		gps_string[6] = '0';
		gps_string[7] = '1';
		DN = 'A';
	}
	else if((hour_high =='0')&&(hour_low=='8'))
	{
		gps_string[6] = '0';
		gps_string[7] = '2';
		DN = 'A';
	}
	else if((hour_high =='0')&&(hour_low=='9'))
	{
		gps_string[6] = '0';
		gps_string[7] = '3';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='0'))
	{
		gps_string[6] = '0';
		gps_string[7] = '4';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='1'))
	{
		gps_string[6] = '0';
		gps_string[7] = '5';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='2'))
	{
		gps_string[6] = '0';
		gps_string[7] = '6';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='3'))
	{
		gps_string[6] = '0';
		gps_string[7] = '7';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='4'))
	{
		gps_string[6] = '0';
		gps_string[7] = '8';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='5'))
	{
		gps_string[6] = '0';
		gps_string[7] = '9';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='6'))
	{
		gps_string[6] = '1';
		gps_string[7] = '0';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='7'))
	{
		gps_string[6] = '1';
		gps_string[7] = '1';
		DN = 'A';
	}
	else if((hour_high =='1')&&(hour_low=='8'))
	{
		gps_string[6] = '1';
		gps_string[7] = '2';
		DN = 'P';
	}
	else if((hour_high =='1')&&(hour_low=='9'))
	{
		gps_string[6] = '0';
		gps_string[7] = '1';
		DN = 'P';
	}
	else if((hour_high =='2')&&(hour_low=='0'))
	{
		gps_string[6] = '0';
		gps_string[7] = '2';
		DN = 'P';
	}
	else if((hour_high =='2')&&(hour_low=='1'))
	{
		gps_string[6] = '0';
		gps_string[7] = '3';
		DN = 'P';
	}
	else if((hour_high =='2')&&(hour_low=='2'))
	{
		gps_string[6] = '0';
		gps_string[7] = '4';
		DN = 'P';
	}
	else if((hour_high =='2')&&(hour_low=='3'))
	{
		gps_string[6] = '0';
		gps_string[7] = '5';
		DN = 'P';
	}
}

int main()
{
	FILE *lcd; //set the file
	lcd = fopen("/dev/lcd_display", "w"); //open the lcd to write mode

	while(1){//top while

		current_char=getchar();//get next char
		if(current_char == '$')//New start of new line, which begins with '$'
		{//scan for $ if
			count = 0;
			current_char=getchar();//get next char
			gps_string[count]=current_char;//add to next position
			xors = current_char;
			count++;//advance position
			current_char=getchar();//get next char

			while(current_char != '*')//while not at the checksum separator
			{

				gps_string[count]=current_char;//add to next position
				xors ^= current_char;
				count++;//advance position
				current_char=getchar();//get next char
			}

			// printf("high is %i",calc_high);
			chk_high=getchar();
			chk_low=getchar();

			char y1 = xors;
			y1 >>= 4;
			char y2 = xors;
			y2 &= 0x0F;
			if(y1 <= 9)
			{
				y1 += 48;
			}
			else
			{
				y1 += 55;
			}
			if(y2 <= 9)
			{
				y2 += 48;
			}
			else
			{
				y2 += 55;
			}
			//printf("given: %c%c, calc %c%c \n",chk_high,chk_low,y1,y2);
			if((chk_high == y1)&&(chk_low == y2))//if chksum passes
			{//chksum if
				//if a GPGGA
				if(gps_string[3] == 'G')
				{//GPGGA if
					switch (*Switches)
					{
						case 0x1://If switch 0 is on
						    fprintf(lcd, "\nLat: %c%c%c%c %c\n",gps_string[16],gps_string[17],gps_string[18],gps_string[19],gps_string[27]);//display latitude
						    fprintf(lcd, "Long: %c%c%c%c%c %c\n",gps_string[29],gps_string[30],gps_string[31],gps_string[32],gps_string[33],gps_string[41]);//display longitude
						    break;
						default://Switch 0 is off
							elev = ((gps_string[53]-48)*1000) + ((gps_string[54]-48)*100) +((gps_string[55]-48)*10) +((gps_string[56]-48))+ ((gps_string[58]-48)*.1);//get elevation from string
							elev *= 3.28;//convert to feet
						    fprintf(lcd, "\nElev: %f\n",elev);//display elevation
						    mountain_time();
						    fprintf(lcd, "Time: %c%c:%c%c:%c%c%cM\n",gps_string[6],gps_string[7],gps_string[8],gps_string[9],gps_string[10],gps_string[11],DN);//display time
							break;
					}
					degrees_latitude = ((gps_string[16]-48)*1000) + ((gps_string[17]-48)*100) +((gps_string[18]-48)*10) +((gps_string[19]-48))+ ((gps_string[21]-48)*.1)+ ((gps_string[22]-48)*.01)+ ((gps_string[23]-48)*.001)+ ((gps_string[24]-48)*.0001)+ ((gps_string[25]-48)*.00001);//get lat from string
					degrees_longitude = ((gps_string[29]-48)*10000) +((gps_string[30]-48)*1000) + ((gps_string[31]-48)*100) +((gps_string[32]-48)*10) +((gps_string[33]-48))+ ((gps_string[35]-48)*.1)+ ((gps_string[36]-48)*.01)+ ((gps_string[37]-48)*.001)+ ((gps_string[38]-48)*.0001)+ ((gps_string[39]-48)*.00001);//get long from string
					printf("\nLatitude = %f degrees\n",degrees_latitude);//print latitude
					printf("Longitude = %f degrees\n",degrees_longitude);//print longitude
				}//GPGGA if close
				//else if a GPGSA
				else if((gps_string[3] == 'S')&&(gps_string[4]=='A'))
				{
					int dum_count = 1;
					char val_1;
					char val_2;
					*LEDs = 0x0;
					printf("\nSat Id numbers: ");//print Satellite ID numbers
					for(pos = 10;pos<30;pos++)
					{
						printf("%c",gps_string[pos]);
						if(dum_count == 1)
						{
							val_1 = gps_string[pos];
						}
						else if(dum_count == 2)
						{
							val_2 = gps_string[pos];
							if(val_1 == '0')
							{
								if(val_2 == '1')
								{
									*LEDs |= 0x0001;
								}
								else if(val_2 == '1')
								{
									*LEDs |= 0x0002;
								}
								else if(val_2 == '2')
								{
									*LEDs |= 0x0004;
								}
								else if(val_2 == '3')
								{
									*LEDs |= 0x0008;
								}
								else if(val_2 == '4')
								{
									*LEDs |= 0x0010;
								}
								else if(val_2 == '5')
								{
									*LEDs |= 0x0020;
								}
								else if(val_2 == '6')
								{
									*LEDs |= 0x0040;
								}
								else if(val_2 == '7')
								{
									*LEDs |= 0x0080;
								}
								else if(val_2 == '8')
								{
									*LEDs |= 0x0100;
								}
								else if(val_2 == '9')
								{
									*LEDs |= 0x0200;
								}
							}
							else if (val_1 == '1')
							{
								if(val_2 == '0' )
								{
									*LEDs |= 0x0400;
								}
								else if(val_2 == '1')
								{
									*LEDs |= 0x0800;
								}
								else if(val_2 == '2')
								{
									*LEDs |= 0x1000;
								}
								else if(val_2 == '3')
								{
									*LEDs |= 0x2000;
								}
								else if(val_2 == '4')
								{
									*LEDs |= 0x4000;
								}
								else if(val_2 == '5')
								{
									*LEDs |= 0x8000;
								}
								else if(val_2 == '6')
								{
									*LEDs |= 0x10000;
								}
								else if(val_2 == '7')
								{
									*LEDs |= 0x20000;
								}
							}
						}
						else
						{
							dum_count = 0;
						}
						dum_count++;
					}
				}
			}//chksum if close
			else//if chksum fails
			{
					fprintf(lcd, "\nGPS Unit is not ready.\n");
					fprintf(lcd, "Data is invalid\n");
			}

		}//scan for $ if close
		else
		{
			if (invalid_flag == 0)
			{
				fprintf(lcd, "\nGPS Unit is not ready.\n");
				fprintf(lcd, "Data is invalid\n");
				invalid_flag = 1;
			}
		}
	}//top while close
	fclose( lcd );//close connection to lcd
	return 0;
}//main close
