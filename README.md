# FORT Compiler

Compiler για τη γλώσσα FORT, μια FORTRAN-like γλώσσα προγραμματισμού.

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