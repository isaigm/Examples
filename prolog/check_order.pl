use_module(library(clpfd)).

order([X]).
order([X, Y | Xs]) :-
    X #=< Y,
    order([Y | Xs]).
