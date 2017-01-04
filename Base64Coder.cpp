#include "Base64Coder.h"
#include "string.h"
#include "math.h"

//const unsigned char* tester_text = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
const char* code_table_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
const unsigned char* code_table = (unsigned char*)code_table_char;

const unsigned char decode_table[123] =
{
   /*  0 a 42 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   /* 43      */ 62,
   /* 44 a 46 */ 0, 0, 0,
   /* 47      */ 63,
   /* 48 a 57 */ 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
   /* 58 a 64 */ 0,0,0,0,0,0,0,
   /* 65 a 90 */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
   /* 91 a 96 */ 0,0,0,0,0,0,
   /* 97 a 122*/ 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 
};


void code34( const unsigned char* in,  unsigned char*out );
void decode43( const  unsigned char* in,  unsigned char*out );

Base64Coder::Base64Coder()
{
   _buffer_size = 0;
   _output_buffer = 0;
}


Base64Coder::~Base64Coder()
{
   delete[] _output_buffer;
   _buffer_size = 0;
   _output_buffer = 0;
}


unsigned char* Base64Coder::encode( const unsigned char* bytes_in, int n_in, int& n_out  )
{
   n_out = (int)(ceil(((double)n_in / 3))) ;
   n_out *= 4;

   if( n_out > _buffer_size)
   {
      delete[] _output_buffer;
      _output_buffer = new  unsigned char[n_out+1];
      _buffer_size = n_out;
   }

    unsigned char temp_1 = 0;
    unsigned char temp_2 = 0;

   int o = 0;
   int i = 0;
   int n_in2 = n_in-2;

   for( i=0, o=0 ; o<n_out && i<(n_in2) ; i+=3, o+=4 )
   {
      code34( bytes_in+i, _output_buffer+o );
   }

   if( i < n_in )
   {
      unsigned char temp[3];
      temp[0] = temp[1] = temp[2] = 0;

      int j = 0;
      while( i<n_in )
      {
         temp[j] = bytes_in[i];
         i++;
         j++;
      }

      code34( temp, _output_buffer+o ); 
      o+=4;
   }

   _output_buffer[o] = 0;
   if( n_in%3 == 2 )
   {
      _output_buffer[o-1] = '=';
   }
   else if( n_in%3 == 1 )
   {
      _output_buffer[o-1] = '=';
      _output_buffer[o-2] = '=';
   }

   _output_buffer[ n_out ] = 0;
   return _output_buffer;
}


unsigned char* Base64Coder::decode( const  unsigned char* bytes_in, int n_in, int& n_out )
{
   if( n_in % 4 != 0 )
      return 0;

   n_out = (n_in * 3) / 4;

   if( n_out > _buffer_size)
   {
      delete[] _output_buffer;
      _output_buffer = new  unsigned char[n_out+1];
      _buffer_size = n_out;
   }

   int o = 0;
   int i = 0;

   for( i=0, o=0 ; o<n_out && i<n_in ; i+=4, o+=3 )
   {
      decode43( bytes_in+i, _output_buffer+o );
   }


   if( bytes_in[n_in-1] == '=')
   {
      _output_buffer[n_out-1] = 0;
      n_out--;

      if( bytes_in[n_in-2] == '=')
      {
         _output_buffer[n_out-1] = 0;
         n_out--;
      }
   }

   return _output_buffer;
}



void code34( const unsigned char* in,  unsigned char*out )
{
    unsigned char temp_1 = 0 ;
    unsigned char temp_2 = 0 ;

   out[0] = in[0] >> 2;     

   temp_1 = in[0] << 6 ;
   temp_1 = temp_1 >>  2;
   temp_2 = in[1] >> 4 ;
   out[1] = temp_1 | temp_2;

   temp_1 = in[1] & 0x0f ;
   temp_1 = temp_1 << 2;
   temp_2 = in[2] >> 6 ;
   out[2] = temp_1 | temp_2;

   out[3] = in[2] & 0x3f ;

   out[0] = code_table[ out[0] ];
   out[1] = code_table[ out[1] ];
   out[2] = code_table[ out[2] ];
   out[3] = code_table[ out[3] ];
}


void decode43( const  unsigned char* in,  unsigned char*out )
{ 
   unsigned char temp_in[4] ;
   temp_in[0] = decode_table[ in[0] ];
   temp_in[1] = decode_table[ in[1] ];
   temp_in[2] = decode_table[ in[2] ];
   temp_in[3] = decode_table[ in[3] ];

    unsigned char temp_1 = 0 ;
    unsigned char temp_2 = 0 ;

   temp_1 = temp_in[0] << 2 ;
   temp_2 = temp_in[1] >> 4 ;
   out[0] = temp_1 | temp_2 ;     

   temp_1 = temp_in[1] << 4 ;
   temp_2 = temp_in[2] >> 2 ;
   out[1] = temp_1 | temp_2 ;  


   temp_1 = temp_in[2] << 6 ;
   temp_2 = temp_in[3];
   out[2] = temp_1 | temp_2 ;  
}