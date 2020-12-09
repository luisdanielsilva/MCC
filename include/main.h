#ifndef MAIN_H_
#define MAIN_H_

/* What to put in headers:
The minimal set of #include directives that are needed to make the header compilable when the header is included in some source file.
Preprocessor symbol definitions of things that need to be shared and that can only accomplished via the preprocessor. Even in C,
preprocessor symbols are best kept to a minimum.
Forward declarations of structures that are needed to make the structure definitions, function prototypes, and global variable 
declarations in the body of the header compilable.
Definitions of data structures and enumerations that are shared amongst multiple source files.
Declarations for functions and variables whose definitions will be visible to the linker.
Inline function definitions, but take care here.

What doesn't belong in a header:
Gratuitous #include directives. Those gratuitous includes cause recompilation of things that don't need to be recompiled, and can at 
times make it so a system can't compile. Don't #include a file in a header if the header itself doesn't need that other header file.
Preprocessor symbols whose intent could be accomplished by some mechanism, any mechanism, other than the preprocessor.
Lots and lots of structure definitions. Split those up into separate headers.
Inline definitions of functions that require an additional #include, that are subject to change, or that are too big. Those inline 
functions should have little if any fan out, and if they do have fan out, it should be localized to stuff defined in the header.
 */

// FUNCTION PROTOTYPES
bool change_direction(bool direction);
void rotate_motor(unsigned long int motor_steps);
unsigned long int calculate_travel_mm(long int ftravel_mm);
bool read_direction();
void menu_print();
void info_print();
void variable_print();
void read_line();
void parse_line();
int execute();

//  Function declarations
// HELP
void cmd_help();
void help_help();

// INFO
void cmd_info();
void help_info();

// SETPINS
void cmd_setpins();
void help_setpins();

#endif