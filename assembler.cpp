#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>


void lexfn( const char *src );
void outfn();


int main( int argc, char **argv )
{
    static uint8_t prog[512];
    prog[0] = 0x01;
    prog[1] = 0x06;
    prog[2] = 0x07;
    prog[3] = 0x08;

    if (argc != 2)
    {
        printf("Usage: assembler [filepath]\n");
        return 1;
    }

    std::ifstream t(argv[1]);

    if (!t.is_open())
    {
        printf("Could not open file \"%s\"\n", argv[1]);
        return 1;
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    const char *src = buffer.c_str();
    lexfn(src);
    
    outfn();
    // printf("0x%08x\n", ((uint32_t*)prog)[0]);
    // // prog[1] = 0x02;
    // // prog[2] = 0x03;
    // // prog[3] = 0x04;
    // // prog[4] = 0x00;
    // // prog[5] = 0x00;

    // std::ofstream stream("rom.bin", std::ios::binary);
    // stream.write((const char*)prog, 32);
    // stream.close();

    return 0;
}





#include <vector>
#include <string>


struct Token
{
    enum class Type: uint8_t
    {
        CMNT,
        IDNT,
        LIT,
        HASH,
        SEP
    };

    Token::Type type;
    std::string lexeme;
};


static std::vector<Token> tokens;
static const char *lex_src;
static char  tok_buf[64];
static char *tok_ptr;
static int   tok_len;


void lexpush( char ch )
{
    *(tok_ptr++) = ch;
}

void lexemit( Token::Type type )
{
    tokens.push_back({type, std::string(tok_buf)});
    std::cout << "\"" << tokens.back().lexeme << "\"\n";
    tok_ptr = tok_buf;
    *tok_ptr = '\0';
}


char peek()
{
    return *lex_src;
}

char match( char ch )
{
    if (*lex_src == ch)
        lex_src++;
    return ch;
}

char advance()
{
    return *(lex_src++);
}


void lexfn( const char *src2 )
{
    lex_src = src2;
    tok_ptr = tok_buf;
    tok_len = 0;

    state_initial:
    {
        while (peek()==' ' || peek()=='\n')
        {
            advance();
        }

        char ch = peek();
        if (ch == '#')   goto state_hash;
        if (ch == ';')   goto state_comment;
        if (isalpha(ch)) goto state_identifier;
        if (isdigit(ch)) goto state_literal;
        if (ch == ',')   goto state_separator;
    }
    goto state_end;


    state_comment:
    {
        char ch = advance();

        while (!(ch=='\n' || ch=='\0'))
        {
            ch = advance();
        }
    }
    goto state_initial;


    state_identifier:
    {
        while (isalpha(peek()))
            *(tok_ptr++) = advance();
        *(tok_ptr++) = '\0';
        lexemit(Token::Type::IDNT);
    }
    goto state_initial;


    state_literal:
    {
        while (isdigit(peek()))
            *(tok_ptr++) = advance();
        *(tok_ptr++) = '\0';
        lexemit(Token::Type::LIT);
    }
    goto state_initial;


    state_hash:
    {
        *(tok_ptr++) = advance();
        *(tok_ptr++) = '\0';
        lexemit(Token::Type::HASH);
    }
    goto state_initial;


    state_separator:
    {
        *(tok_ptr++) = advance();
        *(tok_ptr++) = '\0';
        lexemit(Token::Type::SEP);
    }
    goto state_initial;

    state_end:
        return;
}




static size_t tok_idx = 0;

bool tokmatch( Token::Type A )
{
    if (tok_idx+0 >= tokens.size())
        return false;
    bool cA = (tokens[tok_idx+0].type == A);
    return (cA);
}

bool tokmatch( Token::Type A, Token::Type B )
{
    if (tok_idx+1 >= tokens.size())
        return false;
    bool cA = (tokens[tok_idx+0].type == A);
    bool cB = (tokens[tok_idx+1].type == B);
    return (cA && cB);
}

bool tokmatch( Token::Type A, Token::Type B, Token::Type C )
{
    if (tok_idx+2 >= tokens.size())
        return false;
    bool cA = (tokens[tok_idx+0].type == A);
    bool cB = (tokens[tok_idx+1].type == B);
    bool cC = (tokens[tok_idx+2].type == C);
    return (cA && cB && cC);
}


Token tokadvance()
{
    return tokens[tok_idx++];
}


// ora #0x04
// ora 0x04
// ora 0x04, X
// ora 0x04
// ora 0x04, X
// ora 0x04, Y
// ora (0x04, X)
// ora (0x04), Y
// 
// match("ora", "")


void outfn()
{
    using ttype = Token::Type;
    tok_idx = 0;


    state_I: {
        if (tok_idx >= tokens.size())
        {
            goto state_end;
        }

        Token tok = tokadvance();

        if (tok.type != ttype::IDNT)
        {
            goto state_end;
        }

        if (tok.type == ttype::HASH)
        {
            std::string op = tok.lexeme;
            tok = tokadvance();

            if (tok.type == ttype::LIT)
            {

            }
            goto state_end;
        }
        

        state_IHL: {

        } goto state_I;

        state_IL: {

        } goto state_I;

    } goto state_end;



 
    state_end:
        return;
}

