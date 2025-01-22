# FORT Language Specification

## 1. Program Structure

### 1.1 Basic Units
- Programs consist of a main program unit and optional subprogram units (functions and subroutines)
- Each unit must start with appropriate declaration (PROGRAM, FUNCTION, or SUBROUTINE)
- Each unit must end with END statement
- No global variables - communication through parameters or COMMON blocks

### 1.2 Program Organization
- Main program must start with PROGRAM declaration
- Functions must start with FUNCTION declaration
- Subroutines must start with SUBROUTINE declaration
- Each unit can contain:
  - Declarations
  - COMMON blocks
  - DATA statements
  - Executable statements

## 2. Lexical Elements

### 2.1 Keywords (case-insensitive)
- Program Structure: PROGRAM, FUNCTION, SUBROUTINE, END
- Data Types: INTEGER, REAL, COMPLEX, LOGICAL, CHARACTER, STRING, LIST
- Control Flow: IF, THEN, ELSE, ENDIF, DO, ENDDO
- I/O: READ, WRITE
- Other: COMMON, DATA, CONTINUE, GOTO, CALL, STOP, RETURN

### 2.2 Identifiers
- Must start with alphabetic character
- Can contain letters, digits, and underscores
- Rules for underscores:
  - No consecutive underscores
  - If identifier contains underscore, must end with underscore
  - Cannot end with underscore unless it contains one

### 2.3 Constants
1. Integer Constants:
   - Decimal: sequence of digits
   - Binary: sequence of 0,1 followed by B
   - Octal: sequence of 0-7 followed by O
   - Hex: sequence of 0-9,A-F followed by H

2. Real Constants:
   - Must contain decimal point
   - Optional exponent (E followed by optional sign and digits)
   - Examples: 3.14, .5, 2.0E-3

3. Logical Constants:
   - .true.
   - .false.

4. Character Constants:
   - Single character in single quotes
   - Example: 'A'

5. String Constants:
   - Sequence of characters in double quotes
   - Maximum length 255 characters

### 2.4 Operators
1. Arithmetic:
   - +, -, *, /, **

2. Relational:
   - .gt., .lt., .eq., .le., .ge., .ne.

3. Logical:
   - .and., .or., .not.

4. List:
   - [] for construction
   - + for concatenation

5. Complex:
   - (:) for construction

### 2.5 Comments
- Start with $ character
- Extend to end of line
- Must contain text (not just whitespace)
- Alternative: Line starting with C or * in column 1

## 3. Data Types

### 3.1 Basic Types
1. INTEGER
   - Whole numbers
   - Used for array indices and loop control

2. REAL
   - Floating point numbers
   - Implicit conversion from INTEGER

3. COMPLEX
   - Two REAL components (real and imaginary)
   - Built-in functions: re(), im()

4. LOGICAL
   - Boolean values (.true. or .false.)
   - Result of relational operations

5. CHARACTER
   - Single character
   - Used in strings

6. STRING
   - Fixed length of 256 characters
   - Null-terminated
   - Unused positions retain previous values

### 3.2 Composite Types
1. Arrays
   - Fixed size at declaration
   - Multi-dimensional
   - 1-based indexing
   - Bounds checking required
   - Contiguous storage

2. Lists
   - Dynamic allocation
   - Homogeneous elements
   - Special functions: car(), cdr(), cadr(), cddr()
   - Built-in functions: Length(), New()

## 4. Declarations

### 4.1 Variable Declarations
- Must precede usage
- Format: type variable_list
- Example: INTEGER i, j, k

### 4.2 Array Declarations
- Fixed dimensions at declaration
- Format: type name(dim1,dim2,...)
- Example: REAL matrix(10,10)

### 4.3 COMMON Blocks
- Shared memory between units
- Named or unnamed blocks
- Static allocation
- Type matching not required

### 4.4 DATA Statements
- Initial values for variables
- Format: DATA var1,var2/value1,value2/
- Must match variable type

## 5. Statements

### 5.1 Assignment
1. Simple Assignment
   - Left side: variable or array element
   - Right side: expression of compatible type
   - Implicit conversion INTEGER to REAL

2. String Assignment
   - Copies characters to fixed 256-byte space
   - Null terminator after string
   - Preserves remaining positions

3. List Assignment
   - Only between compatible types
   - Assigns reference to first element
   - May require garbage collection

### 5.2 Control Flow

1. IF Statements
   a) Arithmetic IF:
      - Format: IF (expr) label1,label2,label3
      - Jumps based on negative, zero, positive
   
   b) Logical IF:
      - Format: IF (logical_expr) statement
      - Executes statement if true
   
   c) Block IF:
      - Format: IF (expr) THEN ... [ELSE ...] ENDIF
      - Creates new scope

2. DO Loops
   - Format: DO var=start,end[,step]
   - Control variable must be INTEGER
   - Start, end, step must be INTEGER
   - Step must be positive
   - Control variable cannot be modified inside
   - Executes at least once
   - Creates new scope

3. GOTO Statements
   a) Simple GOTO:
      - Format: GOTO label
      - Jumps to labeled statement
   
   b) Computed GOTO:
      - Format: GOTO i,(label1,label2,...)
      - i selects label from list (1-based)
      - No jump if i out of range

### 5.3 I/O Statements

1. READ Statement
   - Reads from standard input
   - Format: READ var_list
   - Supports implied DO loops

2. WRITE Statement
   - Writes to standard output
   - Format: WRITE expr_list
   - One line per WRITE
   - Supports implied DO loops

### 5.4 Subprogram Calls

1. Function Calls
   - Must assign value to function name
   - Return through function name assignment
   - Arguments passed by value

2. Subroutine Calls
   - Called with CALL statement
   - Arguments can be expressions
   - Parameters passed by reference

### 5.5 Special Statements

1. CONTINUE
   - No-op statement
   - Used with labels

2. RETURN
   - Returns from subprogram
   - Must exist in subprograms

3. STOP
   - Terminates program execution
   - No more statements executed

## 6. Expressions

### 6.1 Type Rules
- Strong type checking
- No implicit type conversion except INTEGER to REAL
- Complex operations require COMPLEX operands
- List operations require compatible element types

### 6.2 Operator Precedence (highest to lowest)
1. ** (exponentiation)
2. Unary minus
3. *, /
4. +, -
5. Relational operators
6. .not.
7. .and.
8. .or.

### 6.3 Scope Rules
- Static binding
- Each block creates new scope
- Inner declarations hide outer ones
- Labels visible in entire containing unit

## 7. Error Handling

### 7.1 Required Checks
- Array bounds checking
- Type compatibility
- Undefined variables/functions
- Label visibility
- Parameter count/type matching

### 7.2 Error Recovery
- Should continue after errors when possible
- Must report line numbers
- Should provide helpful messages
- Should maintain symbol table integrity

## 8. Implementation Requirements

### 8.1 Memory Management
- Static allocation for arrays
- Dynamic allocation for lists
- Garbage collection for unreachable lists
- COMMON block management

### 8.2 Runtime Support
- Array bounds checking
- Type checking
- I/O handling
- List operations
- Built-in functions 