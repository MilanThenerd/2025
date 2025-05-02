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