person(peter).
person(mary).
person(tom).
person(lilly).
person(joseph).
person(rob).
married(peter, mary).
married(lilly, joseph).
invited(mary, tom).

liveTogether(X, Y) :- married(X, Y).
liveTogether(X, Y) :- married(Y, X).

visiting(X, Y) :- invited(Y, X).
visiting(X, Y) :- invited(Z, X), liveTogether(Z, Y).

livesAlone(X) :- person(X), liveTogether(X, _), invited(_, X), visiting(X, _).

getZeroValues([], []) :- !.
getZeroValues([0 | Tail], [0 | ZeroTail]) :-
    getZeroValues(Tail, ZeroTail), !.
getZeroValues([_ | Tail], ZeroTail) :-
    getZeroValues(Tail, ZeroTail).

monotonicallyDecreasing([]).
monotonicallyDecreasing([_]).
monotonicallyDecreasing([X, Y | Tail]) :-
    X >= Y,
    monotonicallyDecreasing([Y | Tail]).