invert(L, Res) :-
    invert_tr(L, [], Res).

invert_tr([], Acc, Acc).

invert_tr([X | Xs], Acc, Res) :-
    Acc_ = [X | Acc],
    invert_tr(Xs, Acc_, Res).
