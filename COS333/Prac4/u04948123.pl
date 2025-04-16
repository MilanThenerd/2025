stripPositivesAbsNegativesIncZeros([], []).

stripPositivesAbsNegativesIncZeros([H|T], [NewH|ResultTail]) :-
    H = 0,
    NewH is H + 1,  % Increment zeros by 1
    stripPositivesAbsNegativesIncZeros(T, ResultTail).

stripPositivesAbsNegativesIncZeros([H|T], [AbsH|ResultTail]) :-
    H < 0,
    AbsH is abs(H),
    stripPositivesAbsNegativesIncZeros(T, ResultTail).

stripPositivesAbsNegativesIncZeros([H|T], ResultTail) :-
    H > 0,
    stripPositivesAbsNegativesIncZeros(T, ResultTail).

:- stripPositivesAbsNegativesIncZeros([], X), writeln(X).
:- stripPositivesAbsNegativesIncZeros([1, 2, 3], X), writeln(X).
:- stripPositivesAbsNegativesIncZeros([-2, -3 , -4], X), writeln(X).
:- stripPositivesAbsNegativesIncZeros([0, 2 , -2, 3,  -3 , 4 , 0 , -6], X), writeln(X).
