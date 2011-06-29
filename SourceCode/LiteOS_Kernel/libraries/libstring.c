/** @file libstring.c
       @brief The functional implementation for the string API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/



#include "libstring.h"
#include "../types/types.h"


uint16_t lib_hex2value(uint8_t hex)
{
  char a = (char)hex;
  uint16_t high;

  if ((a>='0')&&(a<='9'))
	     high = a-'0';
  else
		 high = a-'A'+ 10;
  return high;

}


int lib_string_length(char* s) {
	int count=0;
	while(s[count] != '\0')
		count++;
	return count;
}




void lib_mystrncpy(char *dest, const char *src, uint16_t n)
{
	uint16_t i;
	for (i=0;i<n;i++)
		dest[i] = src[i];
}


void lib_mystrcpy(char *dest, const char *src)
{
	int i;
	i = 0;
	while (src[i]!='\0')
	{ dest[i] = src[i];
	i++;
	}
	dest[i] = '\0';
}


char lib_dec2char(uint8_t value) {

	if (/*value >= 0) &&*/(value <=9))
		return (char) (value + 0x30);
	else
		return 0;
}


char *lib_string_int_to_string(int num)
{
	static char temp[7];
	int internal;
	uint8_t length;
	uint8_t i;
	uint8_t offset = 5;
	uint8_t remainder;

	if (num <0)
		internal = -num;
	else
		internal = num;

	temp[6] = '\0';

	do
	{
		remainder = internal%10;
		temp[offset]= lib_dec2char(remainder);
		internal = internal/10;
		offset--;
	}
	while (internal!=0);

	if (num<0)
	{temp[offset] = '-';
	 if (offset ==0)
		 return temp;
	 else
		 offset--;

	}




	{
		length = 7-offset-1;
		for (i=0;i<length;i++)
			temp[i] = temp[i+offset+1];
	}
	return temp;
}

void lib_string_append(char *base, char *string)
{
	int length1, length2;
	uint8_t i;
	length1 = lib_string_length(base);
	length2 = lib_string_length(string);
	for (i=0;i<length2;i++)
	{
		base[i+length1] = string[i];

	}
	base[length1+length2] = '\0';
}

char* lib_string_split(char ** string, char delimiter)
{
	uint16_t i;
	char* output = (*string);

	for (i = 0; output[i] != 0; i++) 
		if (output[i] == delimiter) {
			output[i] = 0;
			*string = output + i + 1;
			return output;
		}

	*string = output + i;
	return output;
}

