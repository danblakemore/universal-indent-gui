#include <a.h>
#include <a.h>
#include <a.h>

using namespace a.b.c a;

PREPROCESSOR()

BEGIN_MESSAGE_MAP()
    ON_COMMAND()
END_MESSAGE_MAP()

extern struct x y;

static const class Example :
    Int1, Int2, Int3
{
public:
    Example::~Example() :
        S1(),
        S2(),
        S3() {
        // if statements with empty braces
        if( x ) { }
        else if( x ) { }
        else { }

        // if statements with exactly one braced statement
        if( x ) {
            statement;
        }
        else if( x ) {
            statement;
        }
        else {
            statement;
        }

        // special 'if' cases
        if( x ) {
            statement;
        }
        else {
            statement;
        }

        if( x ) {
            statement;

            statement;
        }
        else {
            statement;

            statement;
        }

        // if statements with a single implicit substatement
        if( x )
            statement;

        else if( x )
            statement;

        else
            statement;

        // if statements with multiple statements
        if( x ) {
            statement;

            statement;
        }
        else if( x ) {
            statement;

            statement;
        }
        else {
            statement;

            statement;
        }

        // while statements with a single implicit substatement
        while( x )
            statement;

        // while statements with a single implicit 'if' substatement
        while( x )
            if( x )
                statement;

        // while with multiple statements
        while( x ) {
            statement;

            statement;
        }

        // labeled statement
        label:
        statement;

        // for statements with a single braced statement
        for ( x; x; x ) {
            statement;
        }

        // do statements with a single braced substatement
        do {
            statement;
        } while ( false );

        // do statement with an empty block
        do { } while ( x );

        // local blocks
        {
        statement;
        }

        /* Switch blocks:
         *
         * You can have case substatements be aligned by giving an example like:
         *
         * case 1: statement;
         * 
         *         statement;
         * 
         *         statement;
         * 
         *         etc...
         */
        switch( c ) {
            case 1:
            case 2:
            case 3:
                statement;

                statement;

                statement;

            case 4:
                break; // case with exactly one substatement

            default:
                break;
        }
    }

    void method( const myClass<item> &x, int [][][] c, ... ) {
        // try-catch-finally with empty bodies
        try { }
        catch(Throwable e) { }
        finally { }

        // try-catch-finally with exactly one statement
        try {
            statement;
        }
        catch( Throwable t ) {
            statement;
        }
        finally {
            statement;
        }

        // try-catch-finally with multiple statements 
        try {
            statement;

            statement;
        }
        catch( Throwable e ) {
            statement;

            statement;
        }
        finally {
            statement;

            statement;

            statement;
        }
    }
};

// enum statement
static typedef enum x
{
x,
y,
z,
};

// simple typedef
typedef interface static short int x;

namespace x
{
// template header
template <class T>
x y z v() const;

// pure virtual function, using c-style formal parameters with double parens
void v(()) = 0;

// function with one single line statement and c-style formal parameters
void v(( int i )) {
    statement;
};

// function with no statements
myClass<myType>::method() { }
};

template <T> class x
{
public:

    // operator declarations
    int operator +();

    int operator []();

    // template method
    static void A<x>::method() [][][] {
        asm
            {
            - Assembler statements -
            
            The outside braces are formatted but the asm code is passed through
            unchanged.
            }

        asm Single line assembler statements are also just passed through
    }

    extern void oldStyleFunction()
        int a;
        int b;
        int c; {
        // various simple statements
        long int a, b, c;

        long double [] i;

        goto x;

        delete [] x;

        delete [][][] x;

        return x;

        continue label;

        throw e;

        // c-style function calls with double parens
        b((a, b, c));

        a(());

        // expressions 
        new Object()->field.method();

        s = "string"
                "split across lines";

        method(a, B::C, 'd');

        z = j[0][0][0] || k * 3 >> ++i + "0" > i++ & (i) == !j;

        int *v;

        int &v;

        x = x * *x;

        (int *)x;

        int (*functionPointer)( x, y, z );

        h[0] += a ? b : ((int)c).d;

        new Handler();
    }
} a, b, c; // struct instances

class Class2
{
/* Array creation with multiple non-array elements.
 * 
 * If you give this example with the elements on the same line, then 
 * Polystyle will automatically vertically align them into a grid when it 
 * fits your code to the page. An alternate style is to have each
 * element on its own line, like this:
 * {
 * x,
 * y
 * z
 * }
 */
boolean *bools1 =
    {
    x, y, z
    };

// array creation with a single element
boolean bools2 = { x };

// array creation with no elements
boolean bools3 = { };

// multidimensional array creation 
const int *** array =
    {
    { 1, 2, 3 },
    { 1, 2, 3 },
    { 1, 2, 3 },
    };
};

#if x

#define x

#elif a

#define x

#else

#define x

#define x

#define x

#endif

// see if multi line macros are safely handled:
#define multilinemacro do { x= x+5; } while (0); \
printf("a multilinemacro"); \
printf("a multilinemacro2");
