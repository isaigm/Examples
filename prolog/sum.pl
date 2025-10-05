use_module(library(clpfd)).

sum(L, Res) :-
    sum_tr(L, 0, Res).

sum_tr([], Acc, Acc).

sum_tr([X | Xs], Acc, Res) :-
    Acc_ #= X + Acc,
    sum_tr(Xs, Acc_, Res).
