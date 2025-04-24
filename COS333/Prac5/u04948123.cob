       IDENTIFICATION DIVISION.
       PROGRAM-ID. Stats.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  Arr.
           05  Value1         PIC 9(5)V9(2).
           05  Value2         PIC 9(5)V9(2).
           05  Value3         PIC 9(5)V9(2).

       01  Mean              PIC 9(5)V9(4).
       01  Range             PIC 9(5)V9(2).
       01  Variance          PIC 9(5)V9(4).
       01  Temp              PIC 9(5)V9(4).
       01  Total             PIC 9(6)V9(4).
       01  MaxVal            PIC 9(5)V9(2).
       01  MinVal            PIC 9(5)V9(2).
       01  i                 PIC 9 VALUE 1.

       PROCEDURE DIVISION.

       Main-Pgm.
           PERFORM Read-Data
           PERFORM Find-Mean
           PERFORM Find-Range
           PERFORM Find-Variance

           DISPLAY "Range: " Range
           DISPLAY "Mean: " Mean
           DISPLAY "Variance: " Variance
           STOP RUN.

       Read-Data.
           DISPLAY "Enter value 1:"
           ACCEPT Value1
           DISPLAY "Enter value 2:"
           ACCEPT Value2
           DISPLAY "Enter value 3:"
           ACCEPT Value3.

       Find-Mean.
           COMPUTE Total = Value1 + Value2 + Value3
           COMPUTE Mean = Total / 3.

       Find-Range.
           MOVE Value1 TO MinVal
           MOVE Value1 to MaxVal

           IF Value2 < minVal THEN
               MOVE Value2 TO MinVal
           END-IF
           IF Value2 > maxVal THEN
               MOVE Value2 TO MaxVal
           END-IF

           IF Value3 < minVal THEN
               MOVE Value3 TO MinVal
           END-IF
           IF Value3 > maxVal THEN
               MOVE Value3 TO MaxVal
           END-IF

           COMPUTE Range = MaxVal - minVal.

       Find-Variance.
           COMPUTE Total = (Value1 - Mean) ** 2
           COMPUTE Temp = (Value2 - Mean) ** 2
           COMPUTE Total = Total + Temp
           COMPUTE Temp = (Value3 - Mean) ** 2
           COMPUTE Total = Total + Temp

           COMPUTE Variance ROUNDED = Total / 2.

