# FORT Language Rules

## 1. Program Structure
- Κάθε πρόγραμμα πρέπει να τελειώνει με `END`
- Υποστηρίζονται υπορουτίνες (SUBROUTINE) και συναρτήσεις (FUNCTION)

## 2. Data Types
- INTEGER: Ακέραιοι αριθμοί
- REAL: Αριθμοί κινητής υποδιαστολής
- COMPLEX: Μιγαδικοί αριθμοί
- LOGICAL: Λογικές τιμές (.true., .false.)
- CHARACTER: Χαρακτήρες
- STRING: Συμβολοσειρές
- LIST: Λίστες

## 3. Declarations
- Οι δηλώσεις πρέπει να προηγούνται των εκτελέσιμων εντολών
- Σύνταξη: `type variable_list`
- Υποστηρίζονται πίνακες με σταθερά όρια
- Παράδειγμα: `integer x(10), y, z`

## 4. DATA Statements
- Χρησιμοποιούνται για αρχικοποίηση μεταβλητών
- Σύνταξη: `DATA variable_list/value_list/`
- Υποστηρίζονται επαναλήψεις με `*`
- Οι τιμές πρέπει να ταιριάζουν με τον τύπο των μεταβλητών

## 5. COMMON Blocks
- Επιτρέπουν το μοίρασμα μεταβλητών μεταξύ υπορουτινών
- Σύνταξη: `COMMON /name/ variable_list`

## 6. Expressions
### Αριθμητικοί Τελεστές
- `+`: Πρόσθεση
- `-`: Αφαίρεση
- `*`: Πολλαπλασιασμός
- `/`: Διαίρεση
- `**`: Ύψωση σε δύναμη

### Λογικοί Τελεστές
- `.and.`: Λογικό ΚΑΙ
- `.or.`: Λογικό Η
- `.not.`: Λογική άρνηση

### Συγκριτικοί Τελεστές
- `.gt.`: Μεγαλύτερο από
- `.lt.`: Μικρότερο από
- `.eq.`: Ίσο με
- `.le.`: Μικρότερο ή ίσο

## 7. Control Structures
### IF Statements
- Απλό IF: `IF (condition) statement`
- IF-THEN-ENDIF: `IF (condition) THEN ... ENDIF`
- IF-THEN-ELSE: `IF (condition) THEN ... ELSE ... ENDIF`

### DO Loops
- Σύνταξη: `DO variable = start, end [, step]`
- Πρέπει να τελειώνουν με `ENDDO`
- Το step είναι προαιρετικό (default=1)

### GOTO Statements
- Απλό GOTO: `GOTO label`
- Computed GOTO: `GOTO (label_list), index`

## 8. Input/Output
- READ: Για είσοδο δεδομένων
- WRITE: Για έξοδο δεδομένων
- Σύνταξη: `READ/WRITE [(unit,format)] variable_list`

## 9. Subroutines & Functions
- CALL: Για κλήση υπορουτινών
- RETURN: Για επιστροφή από υπορουτίνα/συνάρτηση
- Παράμετροι περνιούνται by reference

## 10. Comments
- Ξεκινούν με `$`
- Καταλαμβάνουν ολόκληρη γραμμή

## 11. Constants
- Ακέραιοι: Δεκαδικοί (123), Δεκαεξαδικοί (0x7B)
- Πραγματικοί: Δεκαδική υποδιαστολή (123.45), Εκθετική μορφή (1.2345E2)
- Συμβολοσειρές: Σε διπλά εισαγωγικά ("string")
- Λογικές: .true., .false.

## 12. Identifiers
- Πρέπει να ξεκινούν με γράμμα
- Μπορούν να περιέχουν γράμματα, αριθμούς και underscore
- Δεν υπάρχει περιορισμός στο μήκος 