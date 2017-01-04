#ifndef _BASE_64_ENCODER_H_
#define _BASE_64_ENCODER_H_

class Base64Coder
{
public:
   Base64Coder();
   ~Base64Coder();

   /** Returns a temporary buffer, content may change after another
   call to any method of the class
   
   return a null terminated string
   n_out is the same as strlen( <return> )
   the buffer size is actualy n_out + 1 cause of the added null character.

   The null caracter is added for printing issues but is NOT part of the code, for decoding use n_out and not n_out+1.
   */
   unsigned char* encode( const unsigned char* bytes_in, int n_in, int& n_out  );

   /** Returns a temporary buffer, content may change after another
   call to any method of the class
   */
   unsigned char* decode( const unsigned char* bytes_in, int n_in, int& n_out );

private:
   unsigned char* _output_buffer;
   int _buffer_size;
};

#endif