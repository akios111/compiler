# FORT Compiler Project

A compiler implementation for the FORT programming language, written in C using Flex and Bison.

## Project Overview

This project implements a compiler for FORT, translating FORT source code into C. The compiler is built using standard tools:
- Flex for lexical analysis
- Bison for parsing
- C for the implementation

## Current Status

The compiler currently implements:
- Complete lexical analysis with support for:
  - Keywords and identifiers
  - Multiple number formats (decimal, hex, octal, binary)
  - String and character literals
  - Comments and operators
- Syntax analysis featuring:
  - Full FORT grammar implementation
  - Proper operator precedence handling
  - IF-THEN-ELSE structures
  - DO-loop constructs
  - Array declarations and operations

## Project Structure

```
.
├── src/           # Source code files
├── include/       # Header files
├── tests/         # Test cases
└── docs/          # Documentation
```

## Recent Updates

- Enhanced grammar with improved error handling
- Added support for multiple numeric formats
- Implemented comprehensive state transitions in lexical analyzer
- Resolved IF-THEN-ELSE conflicts
- Added operator precedence rules

## Building and Running

### Prerequisites
- GCC compiler
- Flex (Fast Lexical Analyzer)
- Bison (Parser Generator)
- Make

### Build Instructions
```bash
make clean
make
```

### Running the Compiler
```bash
./fort input.fort -o output.c
```

## Testing

Run the test suite:
```bash
make test
```

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Authors

- [Your Name/Team]

## Acknowledgments

- FORT language specification
- Flex and Bison documentation
- Compiler construction resources

## Δομή Project

### Lexical Analysis
- `fort.l`: Ορισμοί λεκτικής ανάλυσης (flex)
  - Αναγνώριση tokens (keywords, identifiers, constants, operators)
  - Χειρισμός σχολίων
  - Μέτρηση γραμμών
  - Παραγωγή `lex.yy.c`

### Syntax Analysis
- `fort.y`: Γραμματική και συντακτικοί κανόνες (bison)
  - Ορισμός γραμματικής FORT
  - Κανόνες παραγωγής για όλες τις δομές
  - Σύνδεση με σημασιολογικές ενέργειες
  - Παραγωγή `fort.tab.c` και `fort.tab.h`

### Symbol Table Management
- `symtab.h`/`symtab.c`: Διαχείριση πίνακα συμβόλων
  - Hash table για γρήγορη αναζήτηση συμβόλων
  - Διαχείριση COMMON blocks
  - Διαχείριση DATA statements
  - Έλεγχος τύπων και εμβέλειας

### Scope Management
- `scope.h`/`scope.c`: Διαχείριση εμβέλειας
  - Ιεραρχική δομή εμβελειών
  - Είσοδος/έξοδος από blocks
  - Σύνδεση συμβόλων με εμβέλειες
  - Έλεγχος ορατότητας μεταβλητών

### Type System
- `types.h`/`types.c`: Σύστημα τύπων
  - Ορισμοί βασικών τύπων
  - Έλεγχοι συμβατότητας τύπων
  - Μετατροπές τύπων
  - Υποστήριξη σύνθετων τύπων

### Intermediate Code
- `intermediate.h`/`intermediate.c`: Ενδιάμεσος κώδικας
  - Δομές δεδομένων για IR
  - Παραγωγή τριών διευθύνσεων κώδικα
  - Διαχείριση προσωρινών μεταβλητών
  - Σύνδεση με βελτιστοποιήσεις

### Code Generation
- `codegen.h`/`codegen.c`: Παραγωγή τελικού κώδικα
  - Παραγωγή assembly (x86)
  - Διαχείριση καταχωρητών
  - Υλοποίηση εντολών
  - Χειρισμός κλήσεων συναρτήσεων

### Optimization
- `optimization.h`/`optimization.c`: Βελτιστοποιήσεις
  - Διάδοση αντιγράφων
  - Απαλοιφή νεκρού κώδικα
  - Βελτιστοποίηση βρόχων
  - Στατιστικά βελτιστοποίησης

### Error Handling
- `error.h`/`error.c`: Διαχείριση σφαλμάτων
  - Αναφορά συντακτικών λαθών
  - Αναφορά σημασιολογικών λαθών
  - Ανάκαμψη από σφάλματα
  - Debugging πληροφορίες

### Main Program
- `main.c`: Κύριο πρόγραμμα
  - Επεξεργασία παραμέτρων γραμμής εντολών
  - Συντονισμός φάσεων μεταγλώττισης
  - Διαχείριση αρχείων εισόδου/εξόδου
  - Επιλογές βελτιστοποίησης

## Build System
- `Makefile`: Αυτοματοποίηση μεταγλώττισης
  - Κανόνες για flex/bison
  - Εξαρτήσεις αρχείων
  - Επιλογές μεταγλώττισης
  - Καθαρισμός προσωρινών αρχείων

## Test Files
- `test1.f`, `test2.f`: Προγράμματα ελέγχου
  - Έλεγχος όλων των χαρακτηριστικών
  - Περιπτώσεις σφαλμάτων
  - Παραδείγματα χρήσης

## Ροή Μεταγλώττισης
1. Λεκτική ανάλυση (fort.l → lex.yy.c)
2. Συντακτική ανάλυση (fort.y → fort.tab.c)
3. Δημιουργία πίνακα συμβόλων
4. Σημασιολογική ανάλυση
5. Παραγωγή ενδιάμεσου κώδικα
6. Βελτιστοποιήσεις
7. Παραγωγή τελικού κώδικα

## Σημαντικά Χαρακτηριστικά
- Πλήρης υποστήριξη FORT
- Διαχείριση εμβέλειας
- Έλεγχος τύπων
- Βελτιστοποιήσεις
- Χειρισμός σφαλμάτων
- Παραγωγή debugging πληροφοριών

## Μελλοντικές Επεκτάσεις
- Πλήρης υλοποίηση βελτιστοποιήσεων βρόχων
- Υποστήριξη περισσότερων αρχιτεκτονικών
- Βελτίωση διαχείρισης μνήμης
- Επέκταση συστήματος τύπων 