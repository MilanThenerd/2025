tenant(alice, tony).
tenant(tom, jane).
tenant(joe, tony).
tenant(mary, jane).
address(alice, pineStreet12).
address(tom, shillingLane15).
address(joe, duncanRoad6).
address(mary, shillingLane15).
address(jonathan, pineStreet12).

ownsSharedProperty(X) :-
    tenant(Tenant1, X),
    tenant(Tenant2, X),
    Tenant1 \= Tenant2,                
    address(Tenant1, Addr),        
    address(Tenant2, Addr).  

:- ownsSharedProperty(X), writeln(X).


countNonMatching(_, [], 0).
countNonMatching(E, [H|T], C) :-
    H = E,
    countNonMatching(E, T, C).

countNonMatching(E, [H|T], C) :-
    H \= E,
    countNonMatching(E, T, C1),
    C is C1 + 1.

:- countNonMatching(a, [a, a], X) , writeln(X).
:- countNonMatching(a, [a, b, a, c, d], X) , writeln(X). 