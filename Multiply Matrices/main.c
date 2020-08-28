#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "Header.h"

#pragma warning (disable:4996)

#define INPUT_BREAK			255
#define MATRIX_DIMENSION	10
#define FOREVER				1
#define MAX_INPUT			5

#define END_OF_BUFFER		1
#define NUMBER_TOO_BIG      2
#define NOT_DECIMAL         3


typedef unsigned long long matrix10[MATRIX_DIMENSION][MATRIX_DIMENSION];

char screen_header[3][sizeof(MAIN_HEADER_1)]	= { MAIN_HEADER_1,MAIN_HEADER_2,MAIN_HEADER_3 };
char input_header[3][sizeof(INPUT_HEADER_1)]	= { INPUT_HEADER_1,INPUT_HEADER_2,INPUT_HEADER_3 };
char output_header[3][sizeof(OUTPUT_HEADER_1)]	= { OUTPUT_HEADER_1,OUTPUT_HEADER_2,OUTPUT_HEADER_3 };
char col_header[3][sizeof(COL_HEADER_1)]		= { COL_HEADER_1,COL_HEADER_2,COL_HEADER_3 };
char main_menu[8][sizeof(MAIN_MENU_1)]			= { MAIN_MENU_1,MAIN_MENU_2,MAIN_MENU_3,MAIN_MENU_4,MAIN_MENU_5,MAIN_MENU_6,MAIN_MENU_7,MAIN_MENU_8 };

int getinput(char* input);
int parse_buffer_for_int(char* buffer, int next_ch);
void check_end_of_buffer(char eob);
void get_matrix_input (matrix10 array, int matrix_num);
void clear_input_buffer();
void multiply_matrices(matrix10 first_matrix, matrix10 second_matrix, matrix10 product_matrix);
int is_decimal(char ch);
void display_header (void);
int display_main_menu();
void display_input_screen(void);
void display_output(matrix10 product);
int int_to_chr(unsigned long long dec, char* ch);
int check_for_long_numbers(char* input_buffer);
char modify_an_entry (matrix10 matrix, int matrix_num);
char action_after_display(void);
void pause_input (int seconds);


static int input_break_cause;
static char input_buffer[100];
static int end_of_buffer = 0;
static matrix10 matrix_1;
static matrix10 matrix_2;
static matrix10 product;

int main()
{
	int selection;
	int matrices_entered = 0;
	int i;
	char action;

	while (FOREVER)
	{
		selection = display_main_menu();

		switch (selection)
		{
			case 1:
				for (i = 0; i < 2; i++)
				{
					display_input_screen();
					if (i==0)
					{ 
						get_matrix_input(matrix_1, 1);
						display_output(matrix_1);
						action = action_after_display(); 

						while (action != '3')
						{
							switch (action)
							{
							case '1' :
								action = modify_an_entry(matrix_1, 1);
								break;
							case '2':
								display_input_screen();
								get_matrix_input(matrix_1, 1);
								display_output(matrix_1);
								action = action_after_display();
								break;
							case '3':
								break;
							default:
								break;
							}
						}
					}
					else
					{
						get_matrix_input(matrix_2, 2);
						display_output(matrix_2);
						action = action_after_display();

						while (action != '3')
						{
							switch (action)
							{
							case '1':
								action = modify_an_entry(matrix_2, 2);
								break;
							case '2':
								display_input_screen();
								get_matrix_input(matrix_2, 2);
								display_output(matrix_2);
								action = action_after_display();
								break;
							case '3':
								break;
							default:
								break;
							}
						}
					}
				}
				matrices_entered = 1;
				break;

			case 2:
				if (matrices_entered == 1)
				{
					multiply_matrices(matrix_1, matrix_2, product);
					display_output(product);
					printf("\n\n                 Press any other  key to contine.");

					getch();
				}
				else
				{
					printf("\nPlease enter matrices to be muliplied\n");
					pause_input(2);
				}
				break;

			case 3:
				matrices_entered = 0;
				printf("\nMatrices have been reset");
				pause_input(2);
				break;

			case 4:
				return 0;

			default:
				printf("Invalid selection please try again");
				break;
		}
	}
	
	return 0;
}



char  modify_an_entry(matrix10 matrix, int matrix_num)
{
	char action = '1';
	unsigned int i, j;
	unsigned int new_value;
	unsigned int entry;
	int input_ch;
	int next_char = 1;
	int in_range;
	int input_count=0;
	int restart=0;


	while (action == '1')
	{
		while (input_count <= 2 && restart == 0)
		{
			display_output(matrix);
			next_char = 1;
			restart = 0;
			printf("Enter the row number, col number and new value : ");
			input_ch = getinput(&input_buffer[0]);

			end_of_buffer = 0;
			in_range = check_for_long_numbers(&input_buffer[0]);

			if (in_range == 1)
			{
				while (input_count <= 2 && restart == 0)
				{
					entry = parse_buffer_for_int(&input_buffer[0], next_char);
					if (input_count == 0 || input_count == 1)
					{
						if (entry > 9)
						{
							printf("\rYou have entered a value for the matrix row/co which is not in the range 0 - 9, please try again.\r");
							pause_input(2);
							next_char = 1;
							restart = 1;
						}
					}

					if (restart == 0)
					{
						switch (input_count)
						{
						case 0:
							i = entry;
							break;
						case 1:
							j = entry;
							break;
						case 2:
							new_value = entry;
							break;
						default:
							break;
						}
					}
					
					input_count++;
					next_char++;
				}


			}
			else
			{
				printf("\rYou have entered a value for the matrix entry which is too, long, please try again.\r");
				pause_input(2);
				next_char = 1;
				restart = 1;
			}
		}

		if (restart == 0)
		{
			matrix[i][j] = new_value;
		}

		display_output(matrix);

		printf("Select action : \n\n");
		printf("                 1 : Modify another entry\n");
		printf("                 Press any other key to finish.");

		action = getch();
		if (action != '1')
		{
			action = '3';
		}
		else 
		{
			input_count = 0;
			next_char = 1;
			restart = 0;
		}
	}

	return action;
}

void pause_input(int seconds)
{
	time_t start_time;
	time_t stop_time;
	time_t timer;

	start_time = time(NULL);
	stop_time = start_time + seconds;

	timer = time(NULL);
	while(timer < stop_time)
	{
		timer = time(NULL);
	}
}

void get_matrix_input(matrix10 array, int matrix_num)
{
	int i = 0, j = 0;
	unsigned int entry;
	int input_ch;
	int next_char = 1;
	int in_range;

	while (i < 10)
	{
		next_char = 1;
		printf("Enter content for Matrix %d, Column %d - 10 decimal entries :",matrix_num,i);
		input_ch = getinput(&input_buffer[0]);

		while (j < 10)
		{
			end_of_buffer = 0;

			in_range = check_for_long_numbers(&input_buffer[0]);

			if (in_range == 1)
			{
				entry = parse_buffer_for_int(&input_buffer[0], next_char);

				while (j <= 10 && entry != INPUT_BREAK && input_buffer[0] != '\n')
				{
					array[i][j] = entry;
					j++;
					next_char++;
					entry = parse_buffer_for_int(&input_buffer[0], next_char);
				}
			}
			else
			{
				entry = INPUT_BREAK;
				input_break_cause = NUMBER_TOO_BIG;
			}

			if (entry == INPUT_BREAK)
			{
				switch (input_break_cause)
				{
				case 1:
					break;

				case 2:
					printf("You have entered a number which is too large.\n");
					break;

				case 3:
					printf("You have entered a non-decimal number.\n");
					break;

				default:
					break;
				}
			}

			if (j < 10)
			{
				printf("Input Error - Please enter %d more entries:", 10 - j);
				input_ch = getinput(&input_buffer[0]);
				next_char = 1;
			}
		}
		i++;
		j = 0;
	}
}

int check_for_long_numbers(char* input_buffer)
{
	int ch_count=0;
	int i;
	int is_ok = 1;

	for (i = 0; input_buffer[i] != '\n'; i++)
	{
		if (input_buffer[i] != ' ')
		{
			ch_count++;
		}
		else
		{
			ch_count = 0;
		}

		if (ch_count > 9)
		{
			is_ok = 0;
			return is_ok;
		}
	}
	return is_ok;
}


int getinput(char* input)
{
	char ch;
	int space_count = 0;
	int ch_count = 0;

	clear_input_buffer();

	ch = getchar();			//Strip out leading spaces
	if (ch_count == 0 && ch == ' ')
	{
		while (ch == ' ')
		{
			ch = getchar();
		}
	}

	while (ch != '\n' && ch_count < sizeof(input_buffer))
	{
		if (space_count <= 1)
		{
			input_buffer[ch_count] = ch;
			ch_count++;
		}
		ch = getchar();

		if (ch == ' ')
			space_count++;
		else
			space_count = 0;
	}

	ch_count--;  //back one

	while (ch_count >= 0 && is_decimal(input_buffer[ch_count]) == 0)
	{
		ch_count--;
	}
	
	ch_count++;
	input_buffer[ch_count] = '\n';

	return ch_count;
}


int parse_buffer_for_int (char* buffer, int next_ch)
{
	int input_number=0;
	char input_character;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 1;
	int m;

	if (end_of_buffer == 1)
	{
		input_break_cause = END_OF_BUFFER;
		return INPUT_BREAK;
	}


	while (j < next_ch) //parse forward to required character
	{
		input_character = buffer[i];
		while (input_character != ' ' && input_character != '\n' && end_of_buffer == 0)
		{
			i++;
			input_character = buffer[i];
			check_end_of_buffer(input_character);
		}
		j++;
		i++;
	}

	if (next_ch != 1)
	{
		i=i-2;
		input_character = buffer[i];

		while (input_character != ' ')
		{
			i--;
			input_character = buffer[i];
			k++;
		}

		m = k;
		while (m != 0)
		{
			i++;
			m--;
		}
	}
	else
	{
		i--;
		k = i;
		i--;
	}

	while (k > 0)
	{
		if ((buffer[i] >= 48) && (buffer[i] <= 57))
		{
			input_number = input_number + ((buffer[i] - 48)*l);
			l = l*10;
		}
		else
		{
			input_number = INPUT_BREAK;
			input_break_cause = NOT_DECIMAL;
			return input_number;
		}
		k--;
		i--;
	}
	
	return input_number;
}

void check_end_of_buffer(char eob)
{
	if (eob == '\n')
		end_of_buffer = 1;
	else
		end_of_buffer = 0;
}

void clear_input_buffer()
{
	int i=0;

	while (i < sizeof(input_buffer))
	{
		input_buffer[i] = '\0';
		i++;
	}
}

void multiply_matrices(matrix10 first_matrix, matrix10 second_matrix, matrix10 product_matrix)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < MATRIX_DIMENSION; i++)
	{
		for (j = 0; j < MATRIX_DIMENSION; j++)
		{
			product_matrix[i][j] = first_matrix[i][j] * second_matrix[i][j];
		}
	}
}

int is_decimal(char ch)
{
	int flag;

	if (ch >= 48 && ch <= 57)
		flag = 1;
	else
		flag = 0;

	return flag;
}

void display_header(void)
{
	int i = 0;

	for (i = 0; i < 3; i++)
		printf(screen_header[i]);

}

int display_main_menu()
{
	int i;
	int select;
	char ch;

	system("CLS");
	display_header();
	printf("\n");

	for (i = 0; i < 8; i++)
		printf(main_menu[i]);
	
	printf("Enter here : ");

	ch = getch();
	select = ch - 48;

	return select;
}

void display_input_screen(void)
{
	int i;

	system("CLS");
	display_header();
	printf("\n");

	for (i=0;i<3;i++)
		printf(input_header[i]);
}

void display_output(matrix10 product)
{
	int i;
	int a = 0, b = 0;
	char output_string[sizeof(COL_HEADER_1)] = { ' ',' ',' ',' ',' ',' ',' ',' ' };
	int ch_count = 1;
	char output_int[10] = { '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0' };
	char column[COL_SIZE];
	int d;
	int col_i;


	system("CLS");
	display_header();
	printf("\n");

	for (i = 0; i < 3; i++)
		printf(col_header[i]);

	for (i = 0; i < 3; i++)
		printf(output_header[i]);

	for (a = 0; a < MATRIX_DIMENSION; a++)
	{
		ch_count = COL_MARGIN_L;
		for (b = 0; b < MATRIX_DIMENSION; b++)
		{
			for (i = 0; i < COL_SIZE; i++)
				column[i] = ' ';

			d = int_to_chr(product[a][b], &output_int[0]);

			col_i = COL_SIZE - 1;
			d--;
			while (d >= 0)
			{
				column[col_i] = output_int[d];
				d--;
				col_i--;
			}

			for (i = 0; i < COL_SIZE; i++)
			{
				output_string[ch_count] = column[i];
				ch_count++;
			}
		}
		output_string[ch_count] = '\n';

		output_string[0] = 'R';
		output_string[1] = 'O';
		output_string[2] = 'W';
		output_string[3] = a + 48;

		ch_count++;
		printf(output_string);
	}
}

char action_after_display()
{
	char action;

	printf("\n\nSelect action : \n\n");
	printf("                 1 : Modify an entry\n");
	printf("                 2 : Restart\n\n\n");
	printf("                 Press any other  key to contine.");

	action = getch();

	if (action != '1' && action != '2')
	{
		action = '3';
	}

	return action;
}

int int_to_chr(unsigned long long dec, char* return_ch)
{
	int ch_count=1;
	unsigned long long divisor;
	int p;
	int int_chars;
	unsigned long long digit;
	double power;
	char overflow[10] = { "OVERFLOW  " };
	int sci = 0;
	char exponent_0 = '0';
	char exponent_1;
	

	if (dec < 0)
	{
		int_chars = sizeof(overflow);
		for (p = 0; p < int_chars; p++)
			return_ch[p] = overflow[p];
		return int_chars;
	}

	divisor = dec / 10;
	while (divisor != 0)
	{
		ch_count++;
		divisor = divisor / 10;
	}

	p = 0;

	if (ch_count > 8)
	{

		sci = 1;

		if (ch_count <= 10)
		{
			dec = dec / 100000;
			ch_count = ch_count - 5;
			exponent_1 = '5';
		}
		else if (ch_count <= 12)
		{
			dec = dec / 10000000;
			ch_count = ch_count - 7;
			exponent_1 = '7';
		}
		else
		{
			dec = dec / 1000000000000;
			ch_count = ch_count - 12;
			exponent_0 = '1';
			exponent_1 = '2';
		}
	}

	int_chars = ch_count;

	while (ch_count > 1)
	{
		power = pow((double)10, (double)(ch_count-(double)1));
		digit = dec / (unsigned long long)power;
		return_ch[p] = (int)digit + 48;
		dec = dec - (digit * (unsigned long long)power);
		ch_count--;
		p++;
	}
	return_ch[p] = dec % 10 + 48;
	p++;

	if (sci == 1)
	{
		return_ch[p] = 'e';
		return_ch[p + 1] = exponent_0;
		return_ch[p + 2] = exponent_1;

		int_chars = int_chars + 3;
	}

	return int_chars;
}