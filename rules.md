### **FORT Programming Language: Complete Grammar and Syntax Rules**

#### **General Overview**
- FORT resembles FORTRAN but introduces modern programming features such as structured commands and recursion.
- The language supports complex data types like lists (inspired by LISP) and character arrays.
- FORT is case-insensitive, except in cases where characters are part of `CCONST` or `SCONST`.
- Programs are structured into a main unit and subprograms, with subprograms declared globally.

---

### **Lexical Units**

#### **Keywords**
- Reserved words in FORT:
  - FUNCTION, SUBROUTINE, END, COMMON, INTEGER, REAL, COMPLEX, LOGICAL, CHARACTER, STRING, LIST, DATA, CONTINUE, GOTO, CALL, READ, WRITE, LENGTH, NEW, IF, THEN, ELSE, ENDIF, DO, ENDDO, STOP, RETURN.

#### **Identifiers (ID)**
- Must begin with an alphabetic character followed by alphanumeric characters or underscores (`_`).
- Underscore rules:
  - No consecutive underscores.
  - If containing an underscore, it must end with an underscore.
- Examples:
  - Valid: `a100_version2_`
  - Invalid: `100version2`, `a100__version2_`

#### **Constants**
- **Integer Constants (`ICONST`)**:
  - Decimal: `0`, `123`.
  - Hexadecimal: `0X9F0`.
  - Octal: `0O67`.
  - Binary: `0B101`.
- **Real Constants (`RCONST`)**:
  - Examples: `.5`, `180.`, `7.2E-5`, `0XA.B`.
  - Includes optional fractional part and exponent.
- **Logical Constants (`LCONST`)**:
  - `.TRUE.`, `.FALSE.`
- **Character Constants (`CCONST`)**:
  - ASCII characters enclosed in single quotes (`'`).
  - Special characters: `\n`, `\t`, `\r`.
  - Examples: `'a'`, `'\n'`.
- **String Constants (`SCONST`)**:
  - Enclosed in double quotes (`"`).
  - Escape sequences: `\"`, `\\`.
  - Examples: `"Hello World"`, `"String with \"quotes\""`.

#### **Operators**
- **Arithmetic**: `+`, `-`, `*`, `/`, `**` (power).
- **Logical**: `.OR.`, `.AND.`, `.NOT.`
- **Relational**: `.GT.`, `.GE.`, `.LT.`, `.LE.`, `.EQ.`, `.NE.`
- **Special**:
  - List composition: `[ ]`
  - Complex number creation: `( : )`
  - Parentheses: `( )`

#### **Special Symbols**
- Delimiters and independent units:
  - `(` (LPAREN), `)` (RPAREN), `,` (COMMA), `=` (ASSIGN), `:` (COLON), `[` (LBRACK), `]` (RBRACK).

#### **Comments**
- Begin with `$` and extend to the end of the line.

---

### **Grammar Rules**

#### **Program Structure**
- **Main Grammar Rule**:
  - `program → body END subprograms`
- **Body**:
  - `body → declarations statements`
- **Subprograms**:
  - `subprograms → subprograms subprogram | ε`
- **Subprogram**:
  - `subprogram → header body END`
  - `header → type listspec FUNCTION ID LPAREN formal_parameters RPAREN`
  - `header → SUBROUTINE ID LPAREN formal_parameters RPAREN`
  - `formal_parameters → type vars COMMA formal_parameters | type vars`

#### **Declarations**
- General Rule:
  - `declarations → declarations type vars`
  - `declarations → declarations COMMON cblock_list`
  - `declarations → declarations DATA vals`
  - `declarations → ε`
- **Variable Declaration**:
  - `type → INTEGER | REAL | COMPLEX | LOGICAL | CHARACTER | STRING`
  - Examples: `INTEGER i, j, k`, `REAL x(10)`, `LIST a`.
- **Array Dimensions**:
  - `dim → ICONST | ID`
- **Lists**:
  - `listspec → LIST | ε`

#### **Statements**
- General Structure:
  - `statements → statements labeled_statement`
  - `labeled_statement → label statement`
  - `label → ICONST`
- **Simple Statements**:
  - Assignment: `assignment → variable ASSIGN expression`
  - Control: `STOP`, `RETURN`, `CONTINUE`
  - Subroutine call: `subroutine_call → CALL variable`
- **Compound Statements**:
  - Branching: `IF LPAREN expression RPAREN THEN body ELSE body ENDIF`
  - Loops: `DO ID ASSIGN iter_space body ENDDO`

#### **Expressions**
- **Arithmetic Expressions**:
  - `expression → expression ADDOP expression`
  - `expression → expression MULOP expression`
  - Examples: `a + b`, `c * (d + e)`.
- **Logical Expressions**:
  - `expression → expression ANDOP expression`
  - `expression → NOTOP expression`
  - Examples: `.NOT. x .OR. y`.
- **Relational Expressions**:
  - `expression → expression RELOP expression`
  - Examples: `a .GT. b`, `c .EQ. d`.
- **List Expressions**:
  - `listexpression → LBRACK expressions RBRACK`
  - `listexpression → LBRACK RBRACK`

#### **Control Structures**
- **If Statements**:
  - Syntax:
    - `if_statement → IF LPAREN expression RPAREN simple_statement`
    - `if_statement → IF LPAREN expression RPAREN label COMMA label COMMA label`
  - Examples: `IF (a .GT. b) THEN ... ENDIF`.
- **Loops**:
  - `loop_statement → DO ID ASSIGN iter_space body ENDDO`
  - Examples:
    - `DO i = 1, 10 ... ENDDO`

#### **Subroutines and Functions**
- **Functions**:
  - Return a value using their name as a variable.
  - Example: `REAL FUNCTION compute(A, B)`.
- **Subroutines**:
  - Called using `CALL`.
  - Example: `CALL process_data(X)`.
- Parameter Passing:
  - By value or by reference.

---

### **Semantic Rules**

#### **Type Rules**
- **Basic Types**:
  - Integer, Real, Complex, Logical, Character.
  - Derived Types: Lists and Arrays.
- Type Compatibility:
  - Implicit conversions between `INTEGER` and `REAL`.
  - No compatibility between numeric and logical/character types.

#### **Variable Scope**
- Variables are static.
- Local scope for variables; subroutine variables are global.

#### **Memory Management**
- Arrays: Contiguously allocated in memory.
- Lists: Dynamically allocated with linked pairs (content, next address).

#### **Control Flow**
- Execution starts with the first statement in the main program body.
- Explicit termination with `STOP`.

---

This detailed output includes all rules and constructs in the FORT programming language, following the provided document comprehensively.

