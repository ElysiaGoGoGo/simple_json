#include "utf8handler.hpp"
#include <array>
     std::tuple <size_t,u_int32_t> UTF8Adaptor::decode_one_char(std::string_view::const_iterator it) 
      {
        auto sum=[it](u_int32_t size){
            auto iter=it;
            auto fill_bit_piece=[](u_int32_t & integer,size_t start_of_integer_to_fill,char c,size_t start_of_valid_bitpiece){
                u_char the_byte=c;
                the_byte&=((u_char)0b1111'1111>>start_of_valid_bitpiece);

                integer|= ((static_cast<u_int32_t>(the_byte)) <<(24-start_of_integer_to_fill+start_of_valid_bitpiece)) ;
                return integer;
            };
            /*
            u_int32_t test=0b1110'1000'0000'0000'0000'0000'0000'0000;
        cout<<"fill_bit_piece test:\n"<<"fill_bit_piece(0b1110'0000,3,(char)0b1101'1100,3)   result:  "<<bitset<32>( fill_bit_piece(test,29,(char)0b1101'0101,5))<<"expected:"<<"0b111'111010"<<"\n\n\n";
*/
            //将一个字节的特定bit片段拼凑到另外一个32整形的某个位置a
            //start_of_valid_bitpiece表示有效bit片段的起始位置(index from 0 to 7),并且从高位到低位方向，直到最低位a,and indicate the number of bits are ignored which are in the front of the byte
            u_int32_t result=0;
            size_t prefix_bits_count=0;
            fill_bit_piece(result,prefix_bits_count,*iter,size+1);
            prefix_bits_count+=8-(size+1);
            for(int i=1;i<size;i++)
            {
                ++iter;
                fill_bit_piece(result,prefix_bits_count,*iter,2);
                prefix_bits_count+=6;
  
            }
  //          cout<<"prefix_bits_count:"<<prefix_bits_count<<"result of sum :"<<std::bitset<32>(result)<<"\n";
            return result>>(32-size*8);

        };
        u_char c=*it;
        if(c>=0b1111'0000)
        {    
            return {4,sum(4)};
        }
        if(c>=0b1110'0000)
        {
            return {3,sum(3)};
        }
        if(c>=0b1100'0000)
        {
            return {2,sum(2)};
        }
        //ascii
        return {1,c};

    }

    bool UTF8Adaptor::IsValidUTF8(std::string_view str)
{

    bool is_current_byte_start = true;
    u_int32_t bytes_rested_for_current_char = 0;
    for (auto c : str)
    {

        if (is_current_byte_start)
        {
#define EASY_READ 1
#if EASY_READ
            constexpr u_int8_t FIRST_BIT_MASK = 0b1000'0000;
            if (c & FIRST_BIT_MASK)
            {
                // non-ascii
                c <<= 1; // second bit
                if ((c & FIRST_BIT_MASK) == 0)
                {
                    // 10xx'xxxx such format not allowed in a start byte for a utf-8 char!
                    return false;
                }
                c <<= 1;        // third bit
                int offset = 2; // the number of bits which are already read in this byte,also show s how many 1 are continoueous in the front of this byte
                for (; offset < 5; ++offset)
                {
                    if ((c & FIRST_BIT_MASK) == 0)
                    {
                        bytes_rested_for_current_char = offset - 1;
                        is_current_byte_start = false;
                        continue;
                    }
                }
                // utf8 allows only up to 4 continoueous 1 in the front of a byte!
                return false;
            }
            else
            { // ascii
                // do nothing
                is_current_byte_start = true;
                bytes_rested_for_current_char = 0;
            }

#else
            constexpr u_int8_t FIRST_BIT_MASK = 0b1000'0000;
            constexpr std::array<u_int8_t, 8> UTF8_FIRST_BYTE_MARK{0b1000'0000, 0b1100'0000, 0b1110'0000, 0b1111'0000, 0b1111'1000, 0b1111'1100, 0b1111'1110, 0b1111'1111};
            int i;
            for (i = 0; i < 4; ++i)
            {
                if (!(c & FIRST_BIT_MASK)) // first bit is not 1?
                {

                    bytes_rested_for_current_char = i - 1;
                    break;
                }
                c <<= 1;
            }
            if (i == 1)
            {
                // no 10xx'xxxx such format in utf-8!
                return false;
            }
            if (i == 4) // all bits are 1
            {
                bytes_rested_for_current_char = 3;
            }
            is_current_byte_start = bytes_rested_for_current_char == -1;

#endif
        }
        else
        {
            bytes_rested_for_current_char--;
            if (bytes_rested_for_current_char == 0)
            {
                is_current_byte_start = true;
            }
            if ((c & 0b1100'0000) != 0b1000'0000)
            {
                // non-continuation byte 
                return false;
            }
        }
    }

    return true;
}

std::string UTF8Adaptor::encode(std::vector<u_int32_t> vec)
{
    std::string result;
    result.reserve(vec.size()*4);
    for(auto code:vec)
    {
        //TODO
    }
    
}
std::vector<u_int32_t> UTF8Adaptor::decode(std::string_view str)
{
    std::vector<u_int32_t> result;
    result.reserve(str.size());
       auto iter=str.begin();
    while(iter!=str.end())
    {
        auto [size,code]=decode_one_char(iter);
        iter+=size;
        result.emplace_back(code);
    }
    return result;

}